#include "Config.h"

/********** CONSTRUCTOR **********/

Config::Config()
{
    QFile config_file("config.cfg");
    if (!config_file.exists()) {
        // Try to initialize a new config file
        if (!this->initConfig()) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Non è possibile aprire il file di configurazione.\n"
                "Assicurarsi che la cartella di installazione non sia protetta da scrittura.\n"
                "E' necessario riavviare il programma.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("E' stato creato un file di configurazione con i valori di default.\n"
                "Modificare i percorsi del DB e dell'Archivio come necessario.");
            message.exec();
        }
    }

    this->loaded = true;
}

/********** DENSTRUCTOR **********/

Config::~Config()
{
}

/********** PUBLIC FUNCTIONS **********/

void Config::setDbPath(QString& db_path)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("db_path");
    config.insert("db_path", db_path);

    this->storeConfig(config);
}

QString Config::getDbPath()
{
    //Then get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    //Access the wanted value
    QString db_path = config_content.value("db_path").toString();

    return db_path;
}

void Config::setArchivePath(QString& archive_path)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("archive_path");
    config.insert("archive_path", archive_path);

    this->storeConfig(config);
}

QString Config::getArchivePath()
{
    //Then get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    //Access the wanted value
    QString db_path = config_content.value("archive_path").toString();

    return db_path;
}

/********** PRIVATE FUNCTIONS **********/

bool Config::initConfig()
{
    QJsonObject config;

    config.insert("db_path", this->default_db_path);
    config.insert("archive_path", this->default_archive_path);

    this->storeConfig(config);

    return true;
}

QJsonObject Config::getConfigFileContent()
{
    QFile config_file("config.cfg");
    config_file.open(QIODevice::ReadOnly);
    QString config_content = config_file.readAll();
    config_file.close();

    QByteArray jsonData = config_content.toUtf8();
    if (jsonData.isEmpty() == true) {
        QJsonObject empty_object;
        return empty_object;
    }

    //Assign the json text to a JSON object
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
    if (jsonDocument.isObject() == false) {
        QJsonObject empty_object;
        return empty_object;
    }

    return jsonDocument.object();
}

bool Config::storeConfig(QJsonObject& config)
{
    QJsonDocument json_config;
    QFile config_file(this->config_file);
    config_file.open(QIODevice::WriteOnly);

    if (!config_file.isOpen()) {
        return false;
    }

    json_config.setObject(config);

    QTextStream outStream(&config_file);
    outStream << json_config.toJson();
    config_file.close();

    return true;
}
