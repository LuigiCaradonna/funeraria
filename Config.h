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
     * Gets the archive path from the config file
     *
     * @return QString the archive path
     */
    QString getArchivePath();

    /*
     * Gets the crosses' folder path from the config file
     *
     * @return QString the crosses' folder path
     */
    QString getCrossesPath();

    /*
     * Gets the drawings' folder path from the config file
     *
     * @return QString the drawings' folder path
     */
    QString getDrawingPath();

    /*
     * Gets the database file path from the config file
     *
     * @return QString the database file path
     */
    QString getDbFile();

    /*
     * Gets the sculptures' folder path from the config file
     *
     * @return QString the sculptures' folder path
     */
    QString getSculpturesPath();

    /*
     * Sets the archive's folder path into the config file
     *
     * @param   const QString& archive_path    - Archive folder path
     *
     * @return void
     */
    void setArchivePath(const QString& archive_path);

    /*
     * Sets the crosses' folder path into the config file
     *
     * @param   const QString& crosses_path    - Crosses folder path
     *
     * @return void
     */
    void setCrossesPath(const QString& crosses_path);

    /*
     * Sets the database file into the config file
     *
     * @param   const QString& db_file    - DB file path
     *
     * @return void
     */
    void setDbFile(const QString& db_file);

    /*
     * Sets the drawings' folder path into the config file
     *
     * @param   const QString& drawing_path    - Drawing folder path
     *
     * @return void
     */
    void setDrawingPath(const QString& drawing_path);

    /*
     * Sets the sculptures' folder path into the config file
     *
     * @param   const QString& sculptures_path    - Sculptures folder path
     *
     * @return void
     */
    void setSculpturesPath(const QString& sculptures_path);

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
     * Initializes a new configuration file
     *
     * @return bool true on success, false on failure
     */
    bool initConfig();

    /*
     * Stores/updates the settings into the config file
     *
     * @param   const QJsonObject& config   - Configuration to be stored
     *
     * @return bool true on success, false on failure
     */
    bool storeConfig(const QJsonObject& config);
};

