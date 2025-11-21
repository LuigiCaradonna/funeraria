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
                "Modificare nelle impostazioni i percorsi alle varie cartelle come necessario.");
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

QString Config::getArchivePath()
{
    // Get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    // Access the wanted value
    QString archive_path = config_content.value("archive_path").toString();

    return archive_path;
}

QString Config::getCrossesPath()
{
    // Get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    // Access the wanted value
    QString crosses_path = config_content.value("crosses_path").toString();

    return crosses_path;
}

QString Config::getDrawingPath()
{
    // Get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    // Access the wanted value
    QString db_path = config_content.value("drawing_path").toString();

    return db_path;
}

QString Config::getDbFile()
{
    // Get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    // Access the wanted value
    QString db_file = config_content.value("db_file").toString();
    
    return db_file;
}

QString Config::getSculpturesPath()
{
    // Get the main JSON object and get the datas in it
    QJsonObject config_content = this->getConfigFileContent();

    // Access the wanted value
    QString sculptures_path = config_content.value("sculptures_path").toString();

    return sculptures_path;
}

void Config::setArchivePath(const QString& archive_path)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("archive_path");
    config.insert("archive_path", archive_path);

    this->storeConfig(config);
}

void Config::setCrossesPath(const QString& crosses_path)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("crosses_path");
    config.insert("crosses_path", crosses_path);

    this->storeConfig(config);
}

void Config::setDbFile(const QString& db_file)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("db_file");
    config.insert("db_file", db_file);

    this->storeConfig(config);
}

void Config::setDrawingPath(const QString& drawing_path)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("drawing_path");
    config.insert("drawing_path", drawing_path);

    this->storeConfig(config);
}

void Config::setSculpturesPath(const QString& sculptures_path)
{
    QJsonObject config;

    config = this->getConfigFileContent();
    config.remove("sculptures_path");
    config.insert("sculptures_path", sculptures_path);

    this->storeConfig(config);
}

/********** PRIVATE FUNCTIONS **********/

QJsonObject Config::getConfigFileContent()
{
    QFile config_file("config.cfg");
    QJsonObject empty_object;

    if (config_file.open(QIODevice::ReadOnly))
    {
        // Read all the file's content
        QByteArray data = config_file.readAll();
        config_file.close();

        // Parsing JSON
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Errore nella lettura del file di configurazione.");
            message.exec();
            return empty_object;
        }

        // Get the main json content
        QJsonObject obj = doc.object();

        return obj;
    }
    return empty_object;
}

bool Config::initConfig()
{
    QJsonObject config;

    config.insert("db_file", this->default_db_file);
    config.insert("archive_path", this->default_archive_path);
    config.insert("sculptures_path", this->default_sculptures_path);

    return this->storeConfig(config);
}

bool Config::storeConfig(const QJsonObject& config)
{
    QJsonDocument json_config;
    QFile config_file(this->config_file);
    if (!config_file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    if (!config_file.isOpen()) {
        return false;
    }

    json_config.setObject(config);

    QTextStream outStream(&config_file);
    outStream << json_config.toJson();
    config_file.close();

    return true;
}
