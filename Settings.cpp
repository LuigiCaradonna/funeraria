#include "Settings.h"

/********** CONSTRUCTOR **********/

Settings::Settings(const QSqlDatabase& db)
    : db(db)
{}

/********** DESTRUCTOR **********/

Settings::~Settings()
{}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Settings::get() {
    QList<QMap<QString, QString>> list;
    QSqlQuery query = QSqlQuery(this->db);

    query.prepare("SELECT name, value FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile recuperare i settaggi per la gestine dei backup.\n"
            "Sono stati caricati i valori predefiniti");
        message.exec();
        return list;
    }

    while (query.next()) {
        QMap<QString, QString> setting;
        setting["name"] = query.value("name").toString();
        setting["value"] = query.value("value").toString();

        list.append(setting);
    }

    return list;
}

bool Settings::store(const QMap<QString, QString>& setting)
{
    QSqlQuery query_check = QSqlQuery(this->db);

    // Check if the value to store already exists
    query_check.prepare("SELECT name FROM " + this->table + " WHERE name = :name");
    query_check.bindValue(":name", setting["name"]);

    if (!query_check.exec()) {
        return false;
    }
    else {
        QSqlQuery query = QSqlQuery(this->db);

        if (query_check.next()) {
            // This setting exists, it needs to be updated

            query.prepare("UPDATE " + this->table + " SET value = :value, updated_at = :updated_at WHERE name = :name");

            query.bindValue(":value", setting["value"]);
            query.bindValue(":updated_at", QDate::currentDate().toString("yyyy-MM-dd"));
            query.bindValue(":name", setting["name"]);
        }
        else {
            // This setting doesn't yet exist, it needs to be inserted

            query.prepare("INSERT INTO " + this->table + " (name, value, created_at) VALUES (:name, :value, :created_at)");

            query.bindValue(":name", setting["name"]);
            query.bindValue(":value", setting["value"]);
            query.bindValue(":created_at", QDate::currentDate().toString("yyyy-MM-dd"));
        }

        if (!query.exec()) {
            return false;
        }
    }

    return true;
}

int Settings::getBackupInterval()
{
    QSqlQuery query = QSqlQuery(this->db);

    query.prepare("SELECT value FROM settings WHERE name = 'backup_interval'");

    if (!query.exec() || !query.next()) {
        return -1;
    }

    return query.value("value").toInt();
}

int Settings::getBackupsToKeep()
{
    QSqlQuery query = QSqlQuery(this->db);

    query.prepare("SELECT value FROM settings WHERE name = 'backups_to_keep'");

    if (!query.exec() || !query.next()) {
        return -1;
    }

    return query.value("value").toInt();
}


/********** PRIVATE FUNCTIONS **********/
