#include "AccessoryUi.h"

/********** CONSTRUCTOR **********/

AccessoryUi::AccessoryUi(QSqlDatabase* db, const QString& table, QWidget* parent)
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
{
    delete this->db;
    delete this->parent;
}

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

    QString result = new_accessory->add(this->ui.leCode->text().trimmed(), this->ui.leName->text().trimmed());

    delete new_accessory;

    if (result != "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(result);
        message.exec();

        return;
    }

    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("INSERT INTO " + this->table + " (code, name, created_at, edited_at) VALUES (:code, :name, :created_at, :edited_at);");
    query.bindValue(":code", this->ui.leCode->text().trimmed());
    query.bindValue(":name", this->ui.leName->text().trimmed());
    query.bindValue(":created_at", date);
    query.bindValue(":edited_at", date);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
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
