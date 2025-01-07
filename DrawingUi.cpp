#include "DrawingUi.h"

/********** CONSTRUCTOR **********/

DrawingUi::DrawingUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, const QString& images_folder, QWidget* parent)
    : db(db), css_folder(css_folder), icons_folder(icons_folder), images_folder(images_folder), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon(this->icons_folder + "funeraria.png"));

    // Load the stylesheet for the UI
    QString style = Helpers::getStyle(this->css_folder);

    if (!style.isEmpty()) {
        this->setStyleSheet(style);
    }

    // code initialization
    this->code = "0";

    // Set chbAllowEdit as deselected by default
    this->ui.chbAllowEdit->setChecked(false);

    // UI elements' binding
    this->connect(this->ui.btnImgPath, &QPushButton::clicked, this, &DrawingUi::slotSelectImage);
    this->connect(this->ui.chbAllowEdit, &QCheckBox::stateChanged, this, &DrawingUi::slotSwitchEnableState);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &DrawingUi::slotCloseDialog);
    // Save button binding is set inside slotSave() and slotUpdate()
}

/********** DESTRUCTOR **********/

DrawingUi::~DrawingUi()
{
}

/********** PUBLIC FUNCTIONS **********/

void DrawingUi::setCode(const QString& code)
{
    this->code = code;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

void DrawingUi::slotSelectImage()
{
    Config* config = new Config();
    QString path;
    path = QFileDialog::getOpenFileName(parent, "Apri file", config->getDrawingPath(), "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!path.isNull()) {
        QString image_file = QFileInfo(path).fileName();
        this->ui.leImgPath->setText(image_file);
    }

    delete config;
}

void DrawingUi::slotSwitchEnableState()
{
    this->ui.leCode->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leName->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leWidth->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leHeight->setEnabled(this->ui.chbAllowEdit->isChecked());
}

void DrawingUi::slotSave()
{
    if (this->checkForm()) {
        Drawing* drawing = new Drawing(this->db);

        if (drawing->store(
            this->ui.leCode->text().trimmed(),
            this->ui.leName->text().trimmed(),
            this->ui.leImgPath->text().trimmed(),
            this->ui.leWidth->text().trimmed(),
            this->ui.leHeight->text().trimmed())
            ) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Immagine inserita.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Inserimento immagine non riuscito.");
            message.exec();
        }

        delete drawing;
        // Close the dialog
        this->close();
    }
}

void DrawingUi::slotUpdate()
{
    if (this->checkForm()) {
        Drawing* drawing = new Drawing(this->db);

        if (drawing->update(
            this->code,
            this->ui.leCode->text().trimmed(),
            this->ui.leName->text().trimmed(),
            this->ui.leImgPath->text().trimmed(),
            this->ui.leWidth->text().trimmed(),
            this->ui.leHeight->text().trimmed())
            ) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Immagine modificata.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Aggiornamento immagine non riuscito.");
            message.exec();
        }

        delete drawing;
        // Close the dialog
        this->close();
    }
}

void DrawingUi::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

void DrawingUi::updateForm()
{
    Drawing* drawing = new Drawing(this->db);
    // Get the selected drawing's data
    QMap<QString, QString> drawing_details = drawing->getByCode(this->code);

    if (!drawing_details.isEmpty()) {
        this->setWindowTitle("Modifica immagine");

        Config* config = new Config();

        // Fill the form fields with the selected drawing's data
        this->ui.leCode->setText(drawing_details["code"]);
        this->ui.leName->setText(drawing_details["name"]);
        this->ui.leImgPath->setText(drawing_details["img"]);
        this->ui.leWidth->setText(drawing_details["width"]);
        this->ui.leHeight->setText(drawing_details["height"]);

        this->ui.leCode->setEnabled(false);
        this->ui.leName->setEnabled(false);
        this->ui.leImgPath->setEnabled(false);
        this->ui.leWidth->setEnabled(false);
        this->ui.leHeight->setEnabled(false);

        QString pic_path = config->getDrawingPath() + "/" + drawing_details["img"];

        // Set the not found image if the provided one is missing
        QFile img_file(pic_path);
        if (!img_file.exists()) {
            pic_path = this->images_folder + "notfound.jpg";
        }

        QPixmap pic(pic_path);
        QPixmap resized;

        // If the image is vertical or squared
        if (pic.height() >= pic.width()) {
            // Resize the image to an height of 225px, the container size
            resized = pic.scaledToHeight(225);
        }
        else {
            // Resize the image to a width of 430px, the container size
            resized = pic.scaledToWidth(430);
        }

        this->ui.lblImgPreview->setText("");
        this->ui.lblImgPreview->setScaledContents(false);
        this->ui.lblImgPreview->setPixmap(resized);

        // Set again chbAllowEdit as deselected to reset any previous change
        this->ui.chbAllowEdit->setChecked(false);

        // Set the save button text
        this->ui.btnSave->setText("Aggiorna");
        // Disconnect previous bindings or multiple signals will be emitted
        this->ui.btnSave->disconnect();
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &DrawingUi::slotUpdate);

        delete config;
    }
    else {
        // Drawing not found means we are asking to insert a new one
        this->setWindowTitle("Inserisci immagine");

        // The image field must not be manually edited
        this->ui.leImgPath->setEnabled(false);

        // Reset the form fields
        this->ui.leCode->setText("0");
        this->ui.leName->setText("");
        this->ui.leImgPath->setText("");
        this->ui.leWidth->setText("");
        this->ui.leHeight->setText("");

        // Set the save button text
        this->ui.btnSave->setText("Aggiungi");
        // Disconnect previous bindings or multiple signals will be emitted
        this->ui.btnSave->disconnect();
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &DrawingUi::slotSave);

        this->ui.lblImgPreview->setText("");
        this->ui.lblImgPreview->setScaledContents(false);
        this->ui.lblImgPreview->clear();
    }

    delete drawing;
}

bool DrawingUi::checkForm()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice dell'a scultur'immagine è obbligatorio.");
        message.exec();

        return false;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome dell'a scultura'immagine è obbligatorio.");
        message.exec();

        return false;
    }

    if (this->ui.leImgPath->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'immagine è obbligatoria.");
        message.exec();

        return false;
    }

    if (!Helpers::isInt(this->ui.leWidth->text().trimmed()) || this->ui.leWidth->text().trimmed().toInt() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("La larghezza deve essere un numero intero maggiore di zero.");
        message.exec();

        return false;
    }

    if (!Helpers::isInt(this->ui.leHeight->text().trimmed()) || this->ui.leHeight->text().trimmed().toInt() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'altezza deve essere un numero intero maggiore di zero.");
        message.exec();

        return false;
    }

    return true;
}