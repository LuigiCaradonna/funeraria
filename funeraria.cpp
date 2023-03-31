#include "funeraria.h"

Funeraria::Funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->config = new Config(this->config_file, this->db_path);

    database = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
    database.setDatabaseName(this->db_path);

    if (!database.open()) {
        // TODO: Show a popup to select a db file, create a new db or close the program
        qDebug() << "Error: Unable to open database...";
    }
    else {
        qDebug() << "Database open successfully...";
    }
}

Funeraria::~Funeraria()
{
    database.close();
    QSqlDatabase::removeDatabase(database.connectionName());
    delete this->config;
}

/********** INIT FUNCTIONS **********/

void Funeraria::initOptions()
{
    // Initialize a new config file with the default values
    this->config->initConfigFile();

    // Now a new valid config file exists, call again this function to load the updated file
    // this->initOptions();
}
