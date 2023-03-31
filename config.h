#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "Helpers.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class Config
{
public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Config object.
     *
     * @param   const QString&      file_name   - Config file name
     * @param   const QString&      db_path     - Database path
     *
     */
    Config(const QString& file_name, const QString& db_path);

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
     * Updates the config file.
     *
     * @param   const std::string&      key     - Name of the option to change.
     * @param   const std::string&      value   - Value to set.
     *
     * @return  bool
     */
    bool updateConfigFile(const QString& key, const QString& value);

private:
    // Name of the configuration file
    QString config_file;
    // Path to the db file, used to init a new config file
    QString db_path;
};

