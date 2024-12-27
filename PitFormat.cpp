#include "PitFormat.h"

/********** CONSTRUCTOR **********/

PitFormat::PitFormat(const QSqlDatabase& db)
    : db(db)
{
    this->settings = new Settings(db);
}

/********** DESTRUCTOR **********/

PitFormat::~PitFormat()
{
    delete this->settings;
}

/********** PUBLIC FUNCTIONS **********/

bool PitFormat::store(const QString& code, const QString& name)
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

bool PitFormat::update(const QString& old_code, const QString& code, const QString& name)
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

bool PitFormat::remove(const QString& code)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE code = :code; ");
    query.bindValue(":code", code);

    if (!query.exec()) {
        return false;
    }

    return true;
}

QList<QMap<QString, QString>> PitFormat::get()
{
    QList<QMap<QString, QString>> formats;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code, name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return formats;
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["code"] = query.value("code").toString();
        row["name"] = query.value("name").toString();

        formats.append(row);
    }

    return formats;
}

QList<QString> PitFormat::getNames()
{
    QStringList names = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return names;
    }

    while (query.next()) {
        names.append(query.value("name").toString());
    }

    return names;
}

QString PitFormat::getName(const QString& code)
{
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

QList<QString> PitFormat::getCodes()
{
    QStringList codes = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return codes;
    }

    while (query.next()) {
        codes.append(query.value("name").toString());
    }

    return codes;
}

QString PitFormat::getCode(const QString& name)
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
        return query.value("name").toString();
    }

    return "";
}