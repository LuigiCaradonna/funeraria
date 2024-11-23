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

QList<QMap<QString, QString>> Sculpture::get()
{
    QList<QMap<QString, QString>> list{};

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM " + this->table + " ORDER BY code");

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["id"] = query.value("id").toString();
        row["code"] = query.value("code").toString();
        row["img"] = query.value("img").toString();
        row["width"] = query.value("width").toString();
        row["height"] = query.value("height").toString();
        row["depth"] = query.value("depth").toString();
        row["created_at"] = query.value("created_at").toString();
        row["edited_at"] = query.value("edited_at").toString();

        list.append(row);
    }

    return list;
}

QMap<QString, QString> Sculpture::getDetailsById(int id)
{
    QMap<QString, QString> map;

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM " + this->table + " WHERE id = :id;");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        map["id"] = query.value("id").toString();
        map["code"] = query.value("code").toString();
        map["img"] = query.value("img").toString();
        map["width"] = query.value("width").toString();
        map["height"] = query.value("height").toString();
        map["depth"] = query.value("depth").toString();
        map["created_at"] = query.value("created_at").toString();
        map["edited_at"] = query.value("edited_at").toString();
    }

    return map;
}

QMap<QString, QString> Sculpture::getDetailsByCode(const QString& code)
{
    QMap<QString, QString> map;

    // If no name is provided, just return the empty map
    if (code == "") return map;

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM " + this->table + " WHERE code = :code;");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        map["id"] = query.value("id").toString();
        map["code"] = query.value("code").toString();
        map["img"] = query.value("img").toString();
        map["width"] = query.value("width").toString();
        map["height"] = query.value("height").toString();
        map["depth"] = query.value("depth").toString();
        map["created_at"] = query.value("created_at").toString();
        map["edited_at"] = query.value("edited_at").toString();
    }

    return map;
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

bool Sculpture::store(
    const QString& code,
    const QString& img,
    const QString& width,
    const QString& height,
    const QString& depth
)
{
    QString created_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("INSERT INTO " + this->table + " "
        " (code, img, width, height, depth, created_at, edited_at)"
        " VALUES (:code, :img, :width, :height, :depth, :created_at, :edited_at)");
    query.bindValue(":code", code);
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
    const int id,
    const QString& code,
    const QString& img,
    const QString& width,
    const QString& height,
    const QString& depth
)
{
    QString edited_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("UPDATE " + this->table + ""
        " SET code = :code, img = :img, width = :width,"
        " height = :height, depth = :depth, edited_at = :edited_at "
        " WHERE id = :id;");
    query.bindValue(":code", code);
    query.bindValue(":img", img);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":depth", depth);
    query.bindValue(":edited_at", edited_at);
    query.bindValue(":id", id);

    if (!query.exec()) {
        return false;
    }

    return true;
}
