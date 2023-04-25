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
    query.prepare("SELECT id, name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QStringList names{ query.value("id").toString(), query.value("name").toString() };
        accessories.append(names);
    }

    return accessories;
}

void Accessory::update(const QString& id, const QString& name)
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("UPDATE " + this->table + " SET name = :name, edited_at = :edited_at WHERE id = :id; ");
    query.bindValue(":name", name.trimmed());
    query.bindValue(":edited_at", date);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

void Accessory::remove(const QString& id)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE id = :id; ");
    query.bindValue(":id", id);

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
    if (this->ui.accessoryName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome è obbligatorio");
        message.exec();

        return;
    }

    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("INSERT INTO " + this->table + " (name, created_at, edited_at) VALUES (:name, :created_at, :edited_at);");
    query.bindValue(":name", this->ui.accessoryName->text().trimmed());
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

    // Reset the input field
    this->ui.accessoryName->setText("");
    // Close the dialog
    this->close();
}

void Accessory::slotCloseDialog()
{
    this->close();
}
