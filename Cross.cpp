#include "Cross.h"

/********** CONSTRUCTOR **********/

Cross::Cross(const QSqlDatabase& db)
    : db(db)
{
}

/********** DESTRUCTOR **********/

Cross::~Cross()
{
}

/********** PUBLIC FUNCTIONS **********/

QMap<QString, QString> Cross::getByCode(const QString& code)
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
        message.setText("Cross: " + query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        map["code"] = query.value("code").toString();
        map["img"] = query.value("img").toString();
        map["name"] = query.value("name").toString();
        map["width"] = query.value("width").toString();
        map["height"] = query.value("height").toString();
        map["created_at"] = query.value("created_at").toString();
        map["edited_at"] = query.value("edited_at").toString();
    }

    return map;
}

QString Cross::getCode(const QString& name)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code FROM " + this->table + " WHERE name = :name");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Cross: " + query.lastError().text());
        message.exec();

        return "";
    }

    if (query.next()) {
        return query.value("code").toString();
    }

    return "";
}

QList<QString> Cross::getCodes()
{
    QStringList codes = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Cross: " + query.lastError().text());
        message.exec();

        return codes;
    }

    while (query.next()) {
        codes.append(query.value("code").toString());
    }

    return codes;
}

QList<QMap<QString, QString>> Cross::getListByCode(const QString& code)
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
        message.setText("Cross: " + query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["code"] = query.value("code").toString();
        row["img"] = query.value("img").toString();
        row["name"] = query.value("name").toString();
        row["width"] = query.value("width").toString();
        row["height"] = query.value("height").toString();
        row["created_at"] = query.value("created_at").toString();
        row["edited_at"] = query.value("edited_at").toString();

        list.append(row);
    }

    return list;
}

QList<QMap<QString, QString>> Cross::getListByName(const QString& name)
{
    QList<QMap<QString, QString>> list{};

    QSqlQuery query = QSqlQuery(this->db);

    QString query_string = "SELECT * FROM " + this->table + " WHERE 1=1 ";

    if (name.trimmed() != "") {
        query_string += " AND name LIKE \"%" + name + "%\"";
    }

    query_string += " ORDER BY name ASC";

    query.prepare(query_string);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Cross: " + query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["code"] = query.value("code").toString();
        row["img"] = query.value("img").toString();
        row["name"] = query.value("name").toString();
        row["width"] = query.value("width").toString();
        row["height"] = query.value("height").toString();
        row["created_at"] = query.value("created_at").toString();
        row["edited_at"] = query.value("edited_at").toString();

        list.append(row);
    }

    return list;
}

QString Cross::getName(const QString& code)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " WHERE code = :code");
    query.bindValue(":code", code);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Cross: " + query.lastError().text());
        message.exec();

        return "";
    }

    if (query.next()) {
        return query.value("name").toString();
    }

    return "";
}

QList<QString> Cross::getNames()
{
    QStringList names = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " ORDER BY name ASC");

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Cross: " + query.lastError().text());
        message.exec();

        return names;
    }

    while (query.next()) {
        names.append(query.value("name").toString());
    }

    return names;
}

bool Cross::remove(const QString& code)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE code = :code;");
    query.bindValue(":code", code);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Cross::store(
    const QString& code,
    const QString& name,
    const QString& img,
    const QString& width,
    const QString& height
)
{
    QString created_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("INSERT INTO " + this->table + " "
        " (code, name, img, width, height, created_at, edited_at)"
        " VALUES (:code, :name, :img, :width, :height, :created_at, :edited_at)");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":img", img);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":created_at", created_at);
    query.bindValue(":edited_at", created_at);

    if (!query.exec()) {
        return false;
    }
    return true;
}

bool Cross::update(
    const QString& old_code,
    const QString& code,
    const QString& name,
    const QString& img,
    const QString& width,
    const QString& height
)
{
    QString edited_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("UPDATE " + this->table + ""
        " SET code = :code, name = :name, img = :img, width = :width,"
        " height = :height, edited_at = :edited_at "
        " WHERE code = :old_code;");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":img", img);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":edited_at", edited_at);
    query.bindValue(":old_code", old_code);

    if (!query.exec()) {
        return false;
    }

    return true;
}
