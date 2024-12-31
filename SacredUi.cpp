#include "SacredUi.h"

/********** CONSTRUCTOR **********/

SacredUi::SacredUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, const QString& images_folder, QWidget* parent)
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
    this->connect(this->ui.btnImgPath, &QPushButton::clicked, this, &SacredUi::slotSelectImage);
    this->connect(this->ui.chbAllowEdit, &QCheckBox::stateChanged, this, &SacredUi::slotSwitchEnableState);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &SacredUi::slotCloseDialog);
    // Save button binding is set inside slotSave() and slotUpdate()
}

/********** DESTRUCTOR **********/

SacredUi::~SacredUi()
{
}

/********** PUBLIC FUNCTIONS **********/

void SacredUi::setCode(const QString& code)
{
    this->code = code;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

void SacredUi::slotSelectImage()
{
    Config* config = new Config();
    QString path;
    path = QFileDialog::getOpenFileName(parent, "Apri file", config->getSacredPath(), "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!path.isNull()) {
        QString image_file = QFileInfo(path).fileName();
        this->ui.leImgPath->setText(image_file);
    }

    delete config;
}

void SacredUi::slotSwitchEnableState()
{
    this->ui.leCode->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leWidth->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leHeight->setEnabled(this->ui.chbAllowEdit->isChecked());
}

void SacredUi::slotSave()
{
    if (this->checkForm()) {
        Sacred* sacred = new Sacred(this->db);

        if (sacred->store(
            this->ui.leCode->text().trimmed(),
            this->ui.leName->text().trimmed(),
            this->ui.leImgPath->text().trimmed(),
            this->ui.leWidth->text().trimmed(),
            this->ui.leHeight->text().trimmed())
            ) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Scultura inserita.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Inserimento scultura non riuscito.");
            message.exec();
        }

        delete sacred;
        // Close the dialog
        this->close();
    }
}

void SacredUi::slotUpdate()
{
    if (this->checkForm()) {
        Sacred* sacred = new Sacred(this->db);

        if (sacred->update(
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
            message.setText("Scultura modificata.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Aggiornamento scultura non riuscito.");
            message.exec();
        }

        delete sacred;
        // Close the dialog
        this->close();
    }
}

void SacredUi::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

void SacredUi::updateForm()
{
    Sacred* sacred = new Sacred(this->db);
    // Get the selected sacred's data
    QMap<QString, QString> sacred_details = sacred->getByCode(this->code);

    if (!sacred_details.isEmpty()) {
        this->setWindowTitle("Modifica scultura");

        Config* config = new Config();

        // Fill the form fields with the selected sacred's data
        this->ui.leCode->setText(sacred_details["code"]);
        this->ui.leName->setText(sacred_details["name"]);
        this->ui.leImgPath->setText(sacred_details["img"]);
        this->ui.leWidth->setText(sacred_details["width"]);
        this->ui.leHeight->setText(sacred_details["height"]);

        this->ui.leCode->setEnabled(false);
        this->ui.leName->setEnabled(false);
        this->ui.leImgPath->setEnabled(false);
        this->ui.leWidth->setEnabled(false);
        this->ui.leHeight->setEnabled(false);

        QString pic_path = config->getSacredPath() + "/" + sacred_details["img"];

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
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &SacredUi::slotUpdate);

        delete config;
    }
    else {
        // Sacred not found means we are asking to insert a new one
        this->setWindowTitle("Inserisci scultura");

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
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &SacredUi::slotSave);

        this->ui.lblImgPreview->setText("");
        this->ui.lblImgPreview->setScaledContents(false);
        this->ui.lblImgPreview->clear();
    }

    delete sacred;
}

bool SacredUi::checkForm()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice della scultura è obbligatorio.");
        message.exec();

        return false;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome della scultura è obbligatorio.");
        message.exec();

        return false;
    }

    if (this->ui.leImgPath->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'immagine della scultura è obbligatoria.");
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