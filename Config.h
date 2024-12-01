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
     * Sets the database file into the config file
     * 
     * @param   const QString& db_file    - DB file path
     *
     * @return void
     */
    void setDbFile(const QString& db_file);

    /*
     * Gets the database file path from the config file
     *
     * @return QString the database file path
     */
    QString getDbFile();

    /*
     * Sets the archive's folder path into the config file
     *
     * @param   const QString& archive_path    - Archive folder path
     *
     * @return void
     */
    void setArchivePath(const QString& archive_path);

    /*
     * Gets the archive path from the config file
     *
     * @return QString the archive path
     */
    QString getArchivePath();

    /*
     * Sets the sculptures' folder path into the config file
     *
     * @param   const QString& sculptures_path    - Sculptures folder path
     *
     * @return void
     */
    void setSculpturesPath(const QString& sculptures_path);

    /*
     * Gets the sculptures' folder path from the config file
     *
     * @return QString the sculptures' folder path
     */
    QString getSculpturesPath();

    /*
     * Initializes a new configuration file
     * 
     * @return bool true on success, false on failure
     */
    bool initConfig();

private:
    QString config_file = "config.cfg";
    QString default_db_file = "./funeraria.db";
    QString default_archive_path = "./lapidi";
    QString default_sculptures_path = "./";

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
     * @param   const QJsonObject& config   - Configuration to be stored
     *
     * @return bool true on success, false on failure
     */
    bool storeConfig(const QJsonObject& config);
};

