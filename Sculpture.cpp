#include "Sculpture.h"

/********** CONSTRUCTOR **********/

Sculpture::Sculpture(const QSqlDatabase& db)
    : db(db)
{
}

/********** DESTRUCTOR **********/

Sculpture::~Sculpture()
{
}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Sculpture::get(const QString& code)
{
    QList<QMap<QString, QString>> list{};

    QSqlQuery query = QSqlQuery(this->db);

    QString query_string = "SELECT * FROM " + this->table + " WHERE 1=1 ";

    if (code.trimmed() != "") {
        query_string += " AND code LIKE \"%" + code + "%\"";
    }

    query_string += " ORDER BY name ASC";

    query.prepare(query_string);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["code"] = query.value("code").toString();
        row["img"] = query.value("img").toString();
        row["name"] = query.value("name").toString();
        row["width"] = query.value("width").toString();
        row["height"] = query.value("height").toString();
        row["depth"] = query.value("depth").toString();
        row["created_at"] = query.value("created_at").toString();
        row["edited_at"] = query.value("edited_at").toString();

        list.append(row);
    }

    return list;
}

QMap<QString, QString> Sculpture::getByCode(const QString& code)
{
    QMap<QString, QString> map;

    // If no name is provided, just return the empty map
    if (code == "") return map;

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM " + this->table + " WHERE code = :code;");
    query.bindValue(":code", code);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        map["code"] = query.value("code").toString();
        map["img"] = query.value("img").toString();
        map["name"] = query.value("name").toString();
        map["width"] = query.value("width").toString();
        map["height"] = query.value("height").toString();
        map["depth"] = query.value("depth").toString();
        map["created_at"] = query.value("created_at").toString();
        map["edited_at"] = query.value("edited_at").toString();
    }

    return map;
}

QList<QString> Sculpture::getNames()
{
    QStringList names = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " ORDER BY name ASC");

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

QString Sculpture::getName(const QString& code)
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

QList<QString> Sculpture::getCodes()
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
        codes.append(query.value("code").toString());
    }

    return codes;
}

QString Sculpture::getCode(const QString& name)
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

bool Sculpture::store(
    const QString& code,
    const QString& name,
    const QString& img,
    const QString& width,
    const QString& height,
    const QString& depth
)
{
    QString created_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("INSERT INTO " + this->table + " "
        " (code, name, img, width, height, depth, created_at, edited_at)"
        " VALUES (:code, :img, :width, :height, :depth, :created_at, :edited_at)");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":img", img);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":depth", depth);
    query.bindValue(":created_at", created_at);
    query.bindValue(":edited_at", created_at);

    if (!query.exec()) {
        return false;
    }
    return true;
}

bool Sculpture::update(
    const QString& old_code,
    const QString& code,
    const QString& name,
    const QString& img,
    const QString& width,
    const QString& height,
    const QString& depth
)
{
    QString edited_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("UPDATE " + this->table + ""
        " SET code = :code, name = :name, img = :img, width = :width,"
        " height = :height, depth = :depth, edited_at = :edited_at "
        " WHERE id = :id;");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":img", img);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":depth", depth);
    query.bindValue(":edited_at", edited_at);
    query.bindValue(":code", old_code);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Sculpture::remove(const int id)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE id = :id;");
    query.bindValue(":id", id);

    if (!query.exec()) {
        return false;
    }

    return true;
}
