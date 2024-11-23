#include "SculptureUi.h"

/********** CONSTRUCTOR **********/

SculptureUi::SculptureUi(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    // id initialization
    this->id = 0;

    // Set chbAllowEdit as deselected by default
    this->ui.chbAllowEdit->setChecked(false);

    // UI elements' binding
    this->connect(this->ui.btnImgPath, &QPushButton::clicked, this, &SculptureUi::slotSelectImage);
    this->connect(this->ui.chbAllowEdit, &QCheckBox::stateChanged, this, &SculptureUi::slotSwitchEnableState);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &SculptureUi::slotCloseDialog);
    // Save button binding is set inside slotSave() and slotUpdate()
    this->connect(this->ui.leTWidth, &QLineEdit::textChanged, this, &SculptureUi::slotReductionWXY);
    this->connect(this->ui.leTHeight, &QLineEdit::textChanged, this, &SculptureUi::slotReductionHXY);
    this->connect(this->ui.leTDepth, &QLineEdit::textChanged, this, &SculptureUi::slotReductionZ);
}

/********** DESTRUCTOR **********/

SculptureUi::~SculptureUi()
{
}

/********** PUBLIC FUNCTIONS **********/

void SculptureUi::setId(int id)
{
    this->id = id;
    this->updateForm();
}

void SculptureUi::setCode(const QString& code)
{
    this->code = code;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

void SculptureUi::slotSelectImage()
{
    Config* config = new Config();
    QString path;
    path = QFileDialog::getOpenFileName(parent, "Apri file", config->getSculpturesPath(), "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!path.isNull()) {
        QString image_file = QFileInfo(path).fileName();
        this->ui.leImgPath->setText(image_file);
    }

    delete config;
}

void SculptureUi::slotSwitchEnableState()
{
    this->ui.leCode->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leOWidth->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leOHeight->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.leODepth->setEnabled(this->ui.chbAllowEdit->isChecked());
}

void SculptureUi::slotSave()
{
    if (this->checkForm()) {
        Sculpture* sculpture = new Sculpture(this->db);

        if (sculpture->store(
            this->ui.leCode->text().trimmed(),
            this->ui.leImgPath->text().trimmed(),
            this->ui.leOWidth->text().trimmed(),
            this->ui.leOHeight->text().trimmed(),
            this->ui.leODepth->text().trimmed())
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

        delete sculpture;
        // Close the dialog
        this->close();
    }
}

void SculptureUi::slotUpdate()
{
    if (this->checkForm()) {
        Sculpture* sculpture = new Sculpture(this->db);

        if (sculpture->update(
            this->id,
            this->ui.leCode->text().trimmed(),
            this->ui.leImgPath->text().trimmed(),
            this->ui.leOWidth->text().trimmed(),
            this->ui.leOHeight->text().trimmed(),
            this->ui.leODepth->text().trimmed())
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

        delete sculpture;
        // Close the dialog
        this->close();
    }
}

void SculptureUi::slotCloseDialog()
{
    this->close();
}

void SculptureUi::slotReductionWXY()
{
    if (!Helpers::isDecimal(this->ui.leTWidth->text()) || this->ui.leTWidth->text().toFloat() < 0) {
        this->ui.lblRedXYVal->setText("---");
        return;
    }

    float redXY = Helpers::scaleFactor(this->ui.leOWidth->text().toFloat(), this->ui.leTWidth->text().toFloat());

    this->ui.lblRedXYVal->setText(QString::number(redXY));
}

void SculptureUi::slotReductionHXY()
{
    if (!Helpers::isDecimal(this->ui.leTHeight->text()) || this->ui.leTHeight->text().toFloat() < 0) {
        this->ui.lblRedXYVal->setText("---");
        return;
    }

    float redXY = Helpers::scaleFactor(this->ui.leOHeight->text().toFloat(), this->ui.leTHeight->text().toFloat());

    this->ui.lblRedXYVal->setText(QString::number(redXY));
}

void SculptureUi::slotReductionZ()
{
    if (!Helpers::isDecimal(this->ui.leTDepth->text()) || this->ui.leTDepth->text().toFloat() < 0) {
        this->ui.lblRedZVal->setText("---");
        return;
    }

    float redXY = Helpers::scaleFactor(this->ui.leODepth->text().toFloat(), this->ui.leTDepth->text().toFloat());

    this->ui.lblRedZVal->setText(QString::number(redXY));
}

/********** PRIVATE FUNCTIONS **********/

void SculptureUi::updateForm()
{
    Sculpture* sculpture = new Sculpture(this->db);
    // Get the selected sculpture's data
    QMap<QString, QString> sculpture_details = sculpture->getDetailsById(this->id);

    if (!sculpture_details.isEmpty()) {
        this->setWindowTitle("Modifica scultura");

        Config* config = new Config();

        // Fill the form fields with the selected sculpture's data
        this->ui.leId->setText(sculpture_details["id"]);
        this->ui.leCode->setText(sculpture_details["code"]);
        this->ui.leImgPath->setText(sculpture_details["img"]);
        this->ui.leOWidth->setText(sculpture_details["width"]);
        this->ui.leOHeight->setText(sculpture_details["height"]);
        this->ui.leODepth->setText(sculpture_details["depth"]);

        this->ui.leId->setEnabled(false);
        this->ui.leCode->setEnabled(false);
        this->ui.leImgPath->setEnabled(false);
        this->ui.leOWidth->setEnabled(false);
        this->ui.leOHeight->setEnabled(false);
        this->ui.leODepth->setEnabled(false);

        this->ui.leTWidth->setText("-");
        this->ui.leTHeight->setText(QString::number(this->default_height));
        this->ui.leTDepth->setText(QString::number(this->default_depth));

        // Set the reduction values using the dafault parameters
        float redZ = Helpers::scaleFactor(sculpture_details["depth"].toFloat(), this->default_depth);
        float redXY = Helpers::scaleFactor(sculpture_details["height"].toFloat(), this->default_height);

        this->ui.lblRedZVal->setText(QString::number(redZ));
        this->ui.lblRedXYVal->setText(QString::number(redXY));

        QString pic_path = config->getSculpturesPath() + "/" + sculpture_details["img"];

        // Set the not found image if the provided one is missing
        QFile img_file(pic_path);
        if (!img_file.exists()) {
            pic_path = "./assets/img/notfound.jpg";
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
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &SculptureUi::slotUpdate);

        delete config;
    }
    else {
        // Sculpture not found means we are asking to insert a new one
        this->setWindowTitle("Inserisci scultura");

        // The image field must not be manually edited
        this->ui.leImgPath->setEnabled(false);

        // Reset the form fields
        this->ui.leId->setText("0");
        this->ui.leCode->setText("");
        this->ui.leImgPath->setText("");
        this->ui.leOWidth->setText("");
        this->ui.leOHeight->setText("");
        this->ui.leODepth->setText("");

        this->ui.leId->setEnabled(false);

        // Set the save button text
        this->ui.btnSave->setText("Aggiungi");
        // Disconnect previous bindings or multiple signals will be emitted
        this->ui.btnSave->disconnect();
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &SculptureUi::slotSave);
    }

    delete sculpture;
}

bool SculptureUi::checkForm()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice o nome della scultura è obbligatorio.");
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

    if (!Helpers::isInt(this->ui.leOWidth->text().trimmed()) || this->ui.leOWidth->text().trimmed().toInt() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("La larghezza deve essere un numero intero maggiore di zero.");
        message.exec();

        return false;
    }
    
    if (!Helpers::isInt(this->ui.leOHeight->text().trimmed()) || this->ui.leOHeight->text().trimmed().toInt() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'altezza deve essere un numero intero maggiore di zero.");
        message.exec();

        return false;
    }
    
    if (!Helpers::isDecimal(this->ui.leODepth->text().trimmed()) || this->ui.leODepth->text().trimmed().toFloat() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("La profondità deve essere un numero intero maggiore di zero.");
        message.exec();

        return false;
    }

    return true;
}