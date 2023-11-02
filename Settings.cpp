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

    // Get the db path reading from the config file
    QFile config_file(this->config_file);
    config_file.open(QIODeviceBase::ReadOnly);

    QString db_path = config_file.readAll();

    QMap<QString, QString> setting;
    setting["name"] = "db_path";
    setting["value"] = db_path;

    list.append(setting);

    return list;
}

bool Settings::store(const QMap<QString, QString>& setting)
{
    QSqlQuery query = QSqlQuery(this->db);

    if (setting["name"] == "backup_interval") {
        query.prepare("UPDATE " + this->table + " SET `value` = \"" + setting["value"] + "\" WHERE name = \"backup_interval\"");

        if (!query.exec()) {
            return false;
        }
    }
    else if (setting["name"] == "backups_to_keep") {
        query.prepare("UPDATE " + this->table + " SET `value` = \"" + setting["value"] + "\" WHERE name = \"backups_to_keep\"");

        if (!query.exec()) {
            return false;
        }
    }
    return true;
}

/********** PRIVATE FUNCTIONS **********/
