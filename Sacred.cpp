#include "Sacred.h"

/********** CONSTRUCTOR **********/

Sacred::Sacred(const QSqlDatabase& db)
    : db(db)
{
}

/********** DESTRUCTOR **********/

Sacred::~Sacred()
{
}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Sacred::getListByCode(const QString& code)
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
        message.setText("Sacred: " + query.lastError().text());
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

QList<QMap<QString, QString>> Sacred::getListByName(const QString& name)
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
        message.setText("Sacred: " + query.lastError().text());
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

QMap<QString, QString> Sacred::getByCode(const QString& code)
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
        message.setText("Sacred: " + query.lastError().text());
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

QList<QString> Sacred::getNames()
{
    QStringList names = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " ORDER BY name ASC");

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Sacred: " + query.lastError().text());
        message.exec();

        return names;
    }

    while (query.next()) {
        names.append(query.value("name").toString());
    }

    return names;
}

QString Sacred::getName(const QString& code)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " WHERE code = :code");
    query.bindValue(":code", code);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Sacred: " + query.lastError().text());
        message.exec();

        return "";
    }

    if (query.next()) {
        return query.value("name").toString();
    }

    return "";
}

QList<QString> Sacred::getCodes()
{
    QStringList codes = {};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Sacred: " + query.lastError().text());
        message.exec();

        return codes;
    }

    while (query.next()) {
        codes.append(query.value("code").toString());
    }

    return codes;
}

QString Sacred::getCode(const QString& name)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT code FROM " + this->table + " WHERE name = :name");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Sacred: " + query.lastError().text());
        message.exec();

        return "";
    }

    if (query.next()) {
        return query.value("code").toString();
    }

    return "";
}

bool Sacred::store(
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
        " (code, name, img, width, height, depth, created_at, edited_at)"
        " VALUES (:code, :img, :width, :height, :created_at, :edited_at)");
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

bool Sacred::update(
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
        " WHERE id = :id;");
    query.bindValue(":code", code);
    query.bindValue(":name", name);
    query.bindValue(":img", img);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":edited_at", edited_at);
    query.bindValue(":code", old_code);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Sacred::remove(const QString& code)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE code = :code;");
    query.bindValue(":code", code);

    if (!query.exec()) {
        return false;
    }

    return true;
}
