#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>

class Config : public QObject
{
    Q_OBJECT

public:
    bool loaded = false;

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the accessory object.
     */
    Config();

    /********** DESTRUCTOR **********/

    /*
     * Destructs the accessory object.
     */
    ~Config();


    /********** PUBLIC FUNCTIONS **********/

    void setDbPath(QString& db_path);
    QString getDbPath();
    void setArchivePath(QString& archive_path);
    QString getArchivePath();
    
    /*
     * Initializes a new configuration file
     * 
     * @return bool true on success, false on failure
     */
    bool initConfig();

private:
    QString config_file = "config.cfg";
    QString default_db_path = "./funeraria.db";
    QString default_archive_path = "./lapidi";

    /********** PRIVATE FUNCTIONS **********/

    QJsonObject getConfigFileContent();

    bool storeConfig(QJsonObject& config);
};

