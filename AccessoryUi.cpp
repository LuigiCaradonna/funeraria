#include "AccessoryUi.h"

/********** CONSTRUCTOR **********/

AccessoryUi::AccessoryUi(const QSqlDatabase& db, const QString& table, QWidget* parent)
    : db(db), table(table), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    this->connect(this->ui.btnCancel, &QPushButton::clicked, this, &AccessoryUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

AccessoryUi::~AccessoryUi()
{ }

/********** PUBLIC FUNCTIONS **********/

void AccessoryUi::updateForm(const QString& code, const QString& name)
{
    // TODO: non arriva l'old_code

    QString action = "";
    // No code means a new accessory to insert
    if (code == "") {
        action = "Inserimento";
        this->ui.btnSave->setText("Salva");
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &AccessoryUi::slotSave);
    }
    else {
        action = "Modifica";
        this->setCode(code);
        this->setName(name);
        this->old_code = code;
        // Set the save button text
        this->ui.btnSave->setText("Aggiorna");
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &AccessoryUi::slotEdit);
    }
    
    if (this->table == "material") {
        this->setWindowTitle(action + " materiale");
    }
    else if (this->table == "vase") {
        this->setWindowTitle(action + " vaso");
    }
    else if (this->table == "lamp") {
        this->setWindowTitle(action + " lume");
    }
    else if (this->table == "flame") {
        this->setWindowTitle(action + " fiamma");
    }
}

/********** PRIVATE SLOTS **********/

void AccessoryUi::slotSave()
{
    if (this->checkForm()) {
        Accessory* accessory = new Accessory(this->db, this->table);

        QString result = accessory->store(this->ui.leCode->text().trimmed(), this->ui.leName->text().trimmed());

        delete accessory;

        if (result != "") {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("L'inserimento non è riuscito.");
            message.exec();

            return;
        }

        // Insert successful

        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Inserimento eseguito.");
        message.exec();

        // Reset the input fields
        this->resetFields();
    }

    // Close the dialog
    this->close();
}

void AccessoryUi::slotEdit()
{
    if (this->checkForm()) {
        Accessory* accessory = new Accessory(this->db, this->table);

        QString result = accessory->update(this->old_code.trimmed(), this->ui.leCode->text().trimmed(), this->ui.leName->text().trimmed());

        delete accessory;

        if (result != "") {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText(result);// "L'aggiornamento non è riuscito.");
            message.exec();

            return;
        }

        // Insert successful

        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Aggiornamento eseguito.");
        message.exec();

        // Reset the input fields
        this->resetFields();
    }

    // Close the dialog
    this->close();
}

void AccessoryUi::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

void AccessoryUi::setCode(QString code)
{
    this->ui.leCode->setText(code);
}

void AccessoryUi::setName(QString name)
{
    this->ui.leName->setText(name);
}

bool AccessoryUi::checkForm()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice è obbligatorio");
        message.exec();

        return false;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome è obbligatorio");
        message.exec();

        return false;
    }

    return true;
}

void AccessoryUi::resetFields() {
    this->ui.leCode->setText("");
    this->ui.leName->setText("");
}