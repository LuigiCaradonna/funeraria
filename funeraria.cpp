#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->config = new Config();

    db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
    db.setDatabaseName(this->config->db_path);

    if (!db.open()) {
        // TODO: Show a popup to select a db file, create a new db or close the program
        qDebug() << "Error: Unable to open database...";
    }
    else {
        qDebug() << "Database open successfully...";
    }
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
    delete this->config;
}

