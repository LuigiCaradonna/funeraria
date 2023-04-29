#include "Accessory.h"

Accessory::Accessory(QSqlDatabase* db, const QString& table, QWidget* parent)
    : db(db), table(table), parent(parent)
{
    this->ui.setupUi(this);

    this->connect(this->ui.okButton, &QPushButton::clicked, this, &Accessory::slotAddAccessory);
    this->connect(this->ui.cancelButton, &QPushButton::clicked, this, &Accessory::slotCloseDialog);
}

Accessory::~Accessory()
{
}

QList<QStringList> Accessory::get()
{
    QList<QStringList> accessories;
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT code, name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QStringList name{ query.value("code").toString(), query.value("name").toString() };
        accessories.append(name);
    }

    return accessories;
}

void Accessory::update(const QString& code, const QString& name)
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("UPDATE " + this->table + " SET code = :code, name = :name, edited_at = :edited_at WHERE code = :codew; ");
    query.bindValue(":code", code.trimmed());
    query.bindValue(":name", name.trimmed());
    query.bindValue(":edited_at", date);
    query.bindValue(":codew", code.trimmed());

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

void Accessory::remove(const QString& code)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE code = :code; ");
    query.bindValue(":code", code);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

void Accessory::slotAddAccessory()
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

    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("INSERT INTO " + this->table + " (code, name, created_at, edited_at) VALUES (:name, :created_at, :edited_at);");
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

    // Reset the input fields
    this->ui.leCode->setText("");
    this->ui.leName->setText("");
    // Close the dialog
    this->close();
}

void Accessory::slotCloseDialog()
{
    this->close();
}
