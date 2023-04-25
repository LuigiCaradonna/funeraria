#include "Tomb.h"

Tomb::Tomb(QSqlDatabase* db, QWidget* parent)
    : db(db), parent(parent)
{
}

Tomb::~Tomb()
{
}

QList<QStringList> Tomb::get(int client_id)
{
    QList<QStringList> tombs;
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare(
        "SELECT "
        "progressive, name, price, paid, notes, accessories_mounted,"
        "ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at "
        "FROM " + this->table + " WHERE client_id = :client_id ORDER BY ordered_at DESC"
    );

    query.bindValue(":client_id", client_id);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Errore: " + query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QStringList tomb{ 
            query.value("progressive").toString(),
            query.value("name").toString(),
            query.value("price").toString(),
            query.value("paid").toString(),
            query.value("notes").toString(),
            query.value("accessories_mounted").toString(),
            query.value("ordered_at").toString(),
            query.value("proofed_at").toString(),
            query.value("confirmed_at").toString(),
            query.value("engraved_at").toString(),
            query.value("delivered_at").toString() 
        };

        tombs.append(tomb);
    }

    return tombs;
}

bool Tomb::update(QString id, QString value)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("UPDATE " + this->table + " SET name = :name WHERE id = :id; ");
    query.bindValue(":name", value);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Errore: " + query.lastError().text());
        message.exec();

        return false;
    }

    return true;
}
