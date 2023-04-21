#include "Flame.h"

Flame::Flame(const QSqlDatabase& db, QWidget* parent)
	: db(db), parent(parent)
{
}

Flame::~Flame()
{
}

QList<QStringList> Flame::get()
{
    QList<QStringList> flames;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT id, name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Errore: " + query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QStringList names{ query.value("id").toString(), query.value("name").toString()};
        flames.append(names);
    }

    return flames;
}

bool Flame::update(QString id, QString value)
{
    QSqlQuery query = QSqlQuery(this->db);
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
