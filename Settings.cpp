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

bool Settings::store(const QList<QMap<QString, QString>>& settings)
{
    QString queryString;
    for (int i = 0; i < settings.size(); i++) {
        QSqlQuery query = QSqlQuery(this->db);
        if (settings[i]["name"] == "backup_interval") {
            queryString = "UPDATE " + this->table + " SET `value` = " + settings[i]["value"] + " WHERE name = \"backup_interval\"";
            query.prepare("UPDATE " + this->table + " SET `value` = " + settings[i]["value"] + " WHERE name = \"backup_interval\"");
        }
        else if (settings[i]["name"] == "backups_to_keep") {
            queryString = "UPDATE " + this->table + " SET `value` = " + settings[i]["value"] + " WHERE name = \"backups_to_keep\"";
            query.prepare("UPDATE " + this->table + " SET `value` = " + settings[i]["value"] + " WHERE name = \"backups_to_keep\"");
        }

        qDebug() << queryString;

        if (!query.exec()) {
            return false;
        }
    }

    return true;
}

/********** PRIVATE FUNCTIONS **********/
