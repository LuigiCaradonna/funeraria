#include "config.h"

/********** CONSTRUCTOR **********/

Config::Config(const QString& file_name, const QString& db_path) :
    config_file(file_name), db_path(db_path)
{
    
}

/********** DESTRUCTOR **********/

Config::~Config()
{
}


/********** PUBLIC FUNCTIONS **********/

void Config::checkConfigFile()
{
    // Open the JSON file
    QFile file(this->config_file);

    // If the file does not exist
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Initialize a new config file
        this->initConfigFile();
    }
    else {
        // Read the JSON data from the file
        QByteArray jsonData = file.readAll();

        // Parse the JSON data into a JSON document
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        // Get the JSON object from the JSON document
        QJsonObject jsonObject = jsonDoc.object();

        /****** CHECK FOR MISSING OR CORRUPTED DATA ******/
        if (!jsonObject.contains("db_path")) {
            // Close the file
            file.close();
            // Initialize a new config file
            this->initConfigFile();
        }
        // Check here any additional entry of the config file
        /*************************************************/

        // Get the values from the JSON object
        // this->db_path = jsonObject["db_path"].toString();

        // Close the file
        file.close();
    }
}

void Config::initConfigFile()
{
    this->checkConfigFile();

    // Create a JSON object
    QJsonObject jsonObject;

    // Set the db_path value
    jsonObject["db_path"] = this->db_path;

    // Convert the JSON object to a JSON document
    QJsonDocument jsonDoc(jsonObject);

    // Create a file to write the JSON data to
    QFile file(this->config_file);

    file.open(QIODevice::WriteOnly | QIODevice::Text);

    // Write the JSON data to the file
    file.write(jsonDoc.toJson());

    // Close the file
    file.close();
}

bool Config::updateConfigFile(const QString& key, const QString& value)
{
    this->checkConfigFile(); 

    // Open the JSON file
    QFile file(this->config_file);

    // Read the JSON data from the file
    QByteArray jsonData = file.readAll();

    // Parse the JSON data into a JSON document
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // Get the JSON object from the JSON document
    QJsonObject jsonObject = jsonDoc.object();

    // Update the option
    jsonObject[key] = value;

    // Close the file
    file.close();

    // Open the file for writing
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "Failed to open file";
        return false;
    }

    // Convert the JSON object back to a JSON document and write it to the file
    jsonDoc.setObject(jsonObject);
    file.write(jsonDoc.toJson());

    // Close the file
    file.close();

    return true;
}
