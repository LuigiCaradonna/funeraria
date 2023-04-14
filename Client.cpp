#include "Client.h"

Client::Client(const QSqlDatabase& db, QWidget* parent)
	: db(db), parent(parent)
{
}

Client::~Client()
{
}

QStringList Client::getNames()
{
    QStringList names{};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM clients ORDER BY position ASC;");

    if (!query.exec()) {
        // TODO: Remove the message box and use the string to output the message somewhere
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Errore: " + query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        names.append(query.value("name").toString());
    }

    return names;
}

QList<QMap<QString, QString>> Client::getDetails()
{
    QList<QMap<QString, QString>> list{};

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM clients;");

    if (!query.exec()) {
        // TODO: Remove the message box and use the string to output the message somewhere
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Errore: " + query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["name"] = query.value("name").toString();
        row["email"] = query.value("email").toString();
        row["address"] = query.value("address").toString();
        row["phone"] = query.value("phone").toString();

        list.append(row);
    }
    
    return list;
}

QMap<QString, QString> Client::getDetails(const QString& name)
{
    QMap<QString, QString> map;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM clients WHERE name = :name;");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Errore: " + query.lastError().text());
        message.exec();

        map["error"] = query.lastError().text();
        return map;
    }

    if (query.next()) {
        map["name"] = query.value("name").toString();
        map["email"] = query.value("email").toString();
        map["address"] = query.value("address").toString();
        map["phone"] = query.value("phone").toString();
        map["active"] = query.value("active").toString();
    }
    else {
        // TODO: Remove the message box and use the string to output the message somewhere
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText("Nessun record trovato");
        message.exec();

        map["error"] = "No record found";
        return map;
    }

    return map;
}
