#include "Accessory.h"

/********** CONSTRUCTOR **********/

Accessory::Accessory(const QSqlDatabase& db, const QString& table)
    : db(db), table(table)
{}

/********** DESTRUCTOR **********/

Accessory::~Accessory()
{}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Accessory::get()
{
    QList<QMap<QString, QString>> accessories;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code, name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return accessories;
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["code"] = query.value("code").toString();
        row["name"] = query.value("name").toString();

        accessories.append(row);
    }

    return accessories;
}

QString Accessory::getCode(const QString& name)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code FROM " + this->table + " WHERE name = :name");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return "";
    }

    if (query.next()) {
        return query.value("code").toString();
    }

    return "";
}

QString Accessory::getNameFromCode(const QString& code)
{
    QString name;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " WHERE code = :code");
    query.bindValue(":code", code);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return "";
    }

    if (query.next()) {
        return query.value("name").toString();
    }

    return "";
}

QStringList Accessory::getNames()
{
    QStringList accessories = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return accessories;
    }

    while (query.next()) {
        accessories.append(query.value("name").toString());
    }

    return accessories;
}

bool Accessory::store(const QString& code, const QString& name)
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("INSERT INTO " + this->table + " (code, name, created_at, edited_at) VALUES (:code, :name, :created_at, :edited_at);");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":created_at", date);
    query.bindValue(":edited_at", date);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Accessory::update(const QString& old_code, const QString& code, const QString& name)
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("UPDATE " + this->table + " SET code = :code, name = :name, edited_at = :edited_at WHERE code = :old_code; ");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":edited_at", date);
    query.bindValue(":old_code", old_code);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Accessory::remove(const QString& code)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE code = :code; ");
    query.bindValue(":code", code);

    if (!query.exec()) {
        return false;
    }

    return true;
}

