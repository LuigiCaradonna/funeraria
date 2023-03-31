#include "funeraria.h"

funeraria::funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    database = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
    database.setDatabaseName("/funeraria.db");

    if (!database.open()) {
        // TODO: Show a popup to notify the problem and close the program
        qDebug() << "Error: Unable to open database...";
    }
    else {
        qDebug() << "Database open successfully...";
    }
}

funeraria::~funeraria()
{
    database.close();
    QSqlDatabase::removeDatabase(database.connectionName());
}
