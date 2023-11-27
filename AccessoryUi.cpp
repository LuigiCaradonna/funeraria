#include "AccessoryUi.h"

/********** CONSTRUCTOR **********/

AccessoryUi::AccessoryUi(const QSqlDatabase& db, const QString& table, QWidget* parent)
    : db(db), table(table), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    if (table == "material") {
        this->setWindowTitle("Inserimento materiale");
    }
    else if (table == "vase") {
        this->setWindowTitle("Inserimento vaso");
    }
    else if (table == "lamp") {
        this->setWindowTitle("Inserimento lume");
    }
    else if (table == "flame") {
        this->setWindowTitle("Inserimento fiamma");
    }

    this->connect(this->ui.btnOk, &QPushButton::clicked, this, &AccessoryUi::slotAddAccessoryUi);
    this->connect(this->ui.btnCancel, &QPushButton::clicked, this, &AccessoryUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

AccessoryUi::~AccessoryUi()
{ }

/********** PRIVATE SLOTS **********/

void AccessoryUi::slotAddAccessoryUi()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice è obbligatorio");
        message.exec();

        return;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome è obbligatorio");
        message.exec();

        return;
    }

    Accessory* new_accessory = new Accessory(this->db, this->table);

    QString result = new_accessory->store(this->ui.leCode->text().trimmed(), this->ui.leName->text().trimmed());

    delete new_accessory;

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
    this->ui.leCode->setText("");
    this->ui.leName->setText("");
    // Close the dialog
    this->close();
}

void AccessoryUi::slotCloseDialog()
{
    this->close();
}
