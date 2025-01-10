#include "CrossUi.h"

/********** CONSTRUCTOR **********/

CrossUi::CrossUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, const QString& images_folder, QWidget* parent)
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
    this->connect(this->ui.btnImgPath, &QPushButton::clicked, this, &CrossUi::slotSelectImage);
    this->connect(this->ui.chbAllowEdit, &QCheckBox::stateChanged, this, &CrossUi::slotSwitchEnableState);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &CrossUi::slotCloseDialog);
    // Save button binding is set inside slotSave() and slotUpdate()
}

/********** DESTRUCTOR **********/

CrossUi::~CrossUi()
{
}

/********** PUBLIC FUNCTIONS **********/

void CrossUi::setCode(const QString& code)
{
    this->code = code;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

void CrossUi::slotCloseDialog()
{
    this->close();
}

void CrossUi::slotSave()
{
    if (this->checkForm()) {
        Cross* cross = new Cross(this->db);

        if (cross->store(
            this->ui.leCode->text().trimmed(),
            this->ui.leName->text().trimmed(),
            this->ui.leImgPath->text().trimmed(),
            this->ui.leWidth->text().trimmed(),
            this->ui.leHeight->text().trimmed())
            ) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Croce inserita.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Inserimento croce non riuscito.");
            message.exec();
        }

        delete cross;
        // Close the dialog
        this->close();
    }
}

void CrossUi::slotSelectImage()
{
    Config* config = new Config();
    QString path;
    path = QFileDialog::getOpenFileName(parent, "Apri file", config->getCrossesPath(), "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!path.isNull()) {
        QString image_file = QFileInfo(path).fileName();
        this->ui.leImgPath->setText(image_file);
    }

    delete config;
}

void CrossUi::slotSwitchEnableState()
{
    this->ui.leCode->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leName->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leWidth->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leHeight->setEnabled(this->ui.chbAllowEdit->isChecked());
}

void CrossUi::slotUpdate()
{
    if (this->checkForm()) {
        Cross* cross = new Cross(this->db);

        if (cross->update(
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
            message.setText("Croce modificata.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Aggiornamento croce non riuscito.");
            message.exec();
        }

        delete cross;
        // Close the dialog
        this->close();
    }
}

/********** PRIVATE FUNCTIONS **********/

bool CrossUi::checkForm()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice della croce è obbligatorio.");
        message.exec();

        return false;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome della croce è obbligatorio.");
        message.exec();

        return false;
    }

    if (this->ui.leImgPath->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'immagine della croce è obbligatoria.");
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

void CrossUi::updateForm()
{
    Cross* cross = new Cross(this->db);
    // Get the selected cross's data
    QMap<QString, QString> cross_details = cross->getByCode(this->code);

    if (!cross_details.isEmpty()) {
        this->setWindowTitle("Modifica croce");

        Config* config = new Config();

        // Fill the form fields with the selected cross's data
        this->ui.leCode->setText(cross_details["code"]);
        this->ui.leName->setText(cross_details["name"]);
        this->ui.leImgPath->setText(cross_details["img"]);
        this->ui.leWidth->setText(cross_details["width"]);
        this->ui.leHeight->setText(cross_details["height"]);

        this->ui.leCode->setEnabled(false);
        this->ui.leName->setEnabled(false);
        this->ui.leImgPath->setEnabled(false);
        this->ui.leWidth->setEnabled(false);
        this->ui.leHeight->setEnabled(false);

        QString pic_path = config->getCrossesPath() + "/" + cross_details["img"];

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
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &CrossUi::slotUpdate);

        delete config;
    }
    else {
        // Cross not found means we are asking to insert a new one
        this->setWindowTitle("Inserisci croce");

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
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &CrossUi::slotSave);

        this->ui.lblImgPreview->setText("");
        this->ui.lblImgPreview->setScaledContents(false);
        this->ui.lblImgPreview->clear();
    }

    delete cross;
}
