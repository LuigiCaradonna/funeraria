#include "config.h"

/********** CONSTRUCTOR **********/

Config::Config()
{
    this->checkConfigFile();
}

/********** DESTRUCTOR **********/

Config::~Config()
{
}


/********** PUBLIC FUNCTIONS **********/

void Config::checkConfigFile()
{
    bool config_errors = false;

    // If the config file exists
    if (QFile::exists(this->config_file)) 
    {
        // Create a QFile object
        QFile file(this->config_file);

        // If the file can't be opened
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        {
            this->resetConfigFile();
        }
        else 
        {
            QJsonObject jsonObject = this->getJsonObject(file);

            /****** CHECK FOR MISSING OR CORRUPTED DATA ******/
            if (!jsonObject.contains("db_path") || jsonObject["db_path"].toString().isEmpty())
            {
                config_errors = true;
            }
            // Check here any additional entry of the config file
            /*************************************************/
            
            // Close the file
            file.close();

            if (config_errors)
            {
                this->resetConfigFile();
                return;
            }

            // The config file is valid

            // Load the config data contained into the config file
            this->loadConfig();
        }
    }
    else 
    {
        // The config file does not exist, create one
        this->initConfigFile();
    }
}

void Config::initConfigFile()
{
    // Create a JSON object
    QJsonObject jsonObject;

    // Set the db_path value
    jsonObject["db_path"] = this->db_path;

    // Convert the JSON object to a JSON document
    QJsonDocument jsonDoc(jsonObject);

    // Create a QFile object
    QFile file(this->config_file);

    // Open the file to write the JSON data to
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    // Write the JSON data to the file
    file.write(jsonDoc.toJson());

    // Close the file
    file.close();

    // Load the config data contained into the config file
    this->loadConfig();
}

void Config::updateConfigFile(const QString& key, const QString& value)
{
    this->checkConfigFile(); 

    // Create a QFile object
    QFile file(this->config_file);

    /*
        It is not possible to call the method getJsonObject() here because
        we need to manipulate the QJsonDocument jsonDoc which wouldn't be available.
    */

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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) 
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Funeraria");
        msgBox.setText("The config file could not be updated.");
        msgBox.exec();

        return;
    }

    // Convert the JSON object back to a JSON document and write it to the file
    jsonDoc.setObject(jsonObject);
    file.write(jsonDoc.toJson());

    // Close the file
    file.close();
}

void Config::resetConfigFile()
{
    // Create a QFile object
    QFile file(this->config_file);

    // Delete the existing config file
    file.remove();

    // Initialize a new config file
    this->initConfigFile();
}

/********** PRIVATE FUNCTIONS **********/

void Config::loadConfig()
{
    /* 
        No need to check for the integrity of the config file
        this method is called after a successful check, 
        after a reset or after a fresh config file creation
    */

    // Create a QFile object
    QFile file(this->config_file);

    // If the file can't be opened
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->resetConfigFile();
    }
    else
    {
        QJsonObject jsonObject = this->getJsonObject(file);

        // Get the values from the JSON object
        this->db_path = jsonObject["db_path"].toString();

        // Close the file
        file.close();
    }
}

QJsonObject Config::getJsonObject(QFile& file)
{
    // Read the JSON data from the file
    QByteArray jsonData = file.readAll();

    // Parse the JSON data into a JSON document
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // Get the JSON object from the JSON document
    QJsonObject jsonObject = jsonDoc.object();

    return jsonObject;
}
