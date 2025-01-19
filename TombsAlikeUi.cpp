#include "TombsAlikeUi.h"

/********** CONSTRUCTOR **********/

TombsAlikeUi::TombsAlikeUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent)
    : db(db), css_folder(css_folder), icons_folder(icons_folder), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon(this->icons_folder + "funeraria.png"));

    // Load the stylesheet for the UI
    QString style = Helpers::getStyle(this->css_folder);

    if (!style.isEmpty()) {
        this->setStyleSheet(style);
    }

    this->init();

    this->connect(this->ui.btnSearch, &QPushButton::clicked, this, &TombsAlikeUi::slotValidateForm);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &TombsAlikeUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

TombsAlikeUi::~TombsAlikeUi()
{
}

/********** PUBLIC FUNCTIONS **********/

QString TombsAlikeUi::getClient()
{
    return this->ui.cbClient->currentText();
}

bool TombsAlikeUi::getCross()
{
    return this->ui.chbCross->isChecked();
}

bool TombsAlikeUi::getDrawing()
{
    return this->ui.chbDrawing->isChecked();
}

bool TombsAlikeUi::getInscription()
{
    return this->ui.chbInscription->isChecked();
}

int TombsAlikeUi::getEpAmount()
{
    return this->ui.leEpigraphs->text().toInt();
}

QString TombsAlikeUi::getMaterial()
{
    return this->ui.cbMaterial->currentText();
}

bool TombsAlikeUi::getMount()
{
    return this->ui.chbMount->isChecked();
}

int TombsAlikeUi::getPitsAmount()
{
    return this->ui.lePits->text().toInt();
}

bool TombsAlikeUi::getProvided()
{
    return this->ui.chbMatProvided->isChecked();
}

bool TombsAlikeUi::getRelief()
{
    return this->ui.chbEpRelief->isChecked();
}

bool TombsAlikeUi::getSculpture()
{
    return this->ui.chbSculpture->isChecked();
}

void TombsAlikeUi::reset()
{
    this->ui.cbClient->setCurrentIndex(0);
    this->ui.cbMaterial->setCurrentIndex(0);
    this->ui.leEpigraphs->setText("1");
    this->ui.lePits->setText("1");
    this->ui.chbCross->setChecked(false);
    this->ui.chbDrawing->setChecked(false);
    this->ui.chbMatProvided->setChecked(false);
    this->ui.chbEpRelief->setChecked(false);
    this->ui.chbInscription->setChecked(false);
    this->ui.chbMount->setChecked(true);
    this->ui.chbSculpture->setChecked(false);
}

/********** PROTECTED SLOTS **********/

void TombsAlikeUi::slotCloseDialog()
{
    this->reject();
}

void TombsAlikeUi::slotValidateForm()
{
    bool errors = false;

    if (!Helpers::isInt(this->ui.leEpigraphs->text()) || this->ui.leEpigraphs->text().toInt() < 0) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero di epigrafi deve essere un intero positivo.");
        message.exec();

        errors = true;
    }
    else if (!Helpers::isInt(this->ui.lePits->text()) || this->ui.lePits->text().toInt() < 0) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero di scavi deve essere un intero positivo.");
        message.exec();

        errors = true;
    }

    if (!errors)
        this->accept();
}

/********** PRIVATE FUNCTIONS **********/

void TombsAlikeUi::init()
{
    Client* client = new Client(this->db);
    QList<QString> client_names = client->getActiveNames();
    this->ui.cbClient->addItems(client_names);

    Accessory* material = new Accessory(this->db, "material");
    QList<QString> material_names = material->getNames();
    this->ui.cbMaterial->addItems(material_names);
}