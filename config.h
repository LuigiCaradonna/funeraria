#pragma once

#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDir>

class Config
{
public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Config object.
     *
     */
    Config();

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Config object.
     */
    ~Config();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Checks if the content of the config file is valid.
     *
     * @return  void
     */
    void checkConfigFile();

    /*
     * Initializes a new config file.
     *
     * @return  void
     */
    void initConfigFile();

    /*
     * Resets the config file to the default values.
     *
     * @return  void
     */
    void resetConfigFile();

    /*
     * Updates the config file.
     *
     * @param   const std::string&      key     - Name of the option to change.
     * @param   const std::string&      value   - Value to set.
     *
     * @return  void
     */
    void updateConfigFile(const QString& key, const QString& value);

    // Path to the db file, updated by the loadConfig() method
    QString db_path = QDir::currentPath() + "/funeraria.db";

private:
    /********** PRIVATE FUNCTIONS **********/

    /*
     * Loads the values contained into the config file.
     *
     * @return  void
     */
    void loadConfig();

    QJsonObject getJsonObject(QFile& file);

    // Name of the configuration file
    const QString config_file = QDir::currentPath() + "/config.cfg";
};

