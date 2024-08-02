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
     * Constructs the config object.
     */
    Config();

    /********** DESTRUCTOR **********/

    /*
     * Destructs the config object.
     */
    ~Config();


    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the database path into the config file
     *
     * @return void
     */
    void setDbPath(QString& db_path);

    /*
     * Gets the database path from the config file
     *
     * @return QString the database path
     */
    QString getDbPath();

    /*
     * Sets the archive path into the config file
     *
     * @return void
     */
    void setArchivePath(QString& archive_path);

    /*
     * Gets the archive path from the config file
     *
     * @return QString the database path
     */
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

    /*
     * Gets the content of the config file formatted as a JSON
     *
     * @return QJsonObject consisting of the content of the config file
     */
    QJsonObject getConfigFileContent();

    /*
     * Stores/updates the settings into the config file
     *
     * @return bool true on success, false on failure
     */
    bool storeConfig(QJsonObject& config);
};

