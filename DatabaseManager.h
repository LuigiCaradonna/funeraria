#pragma once
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include "Helpers.h"

class DatabaseManager
{
public:
    bool db_connected;
    QString db_path;

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the DatabaseManager object.
     *
     * @param	const QString&	db_path	- Path to the database file
     */
    DatabaseManager(const QString& db_path, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the DatabaseManager object.
     */
    ~DatabaseManager();

    /********** PUBLIC FUNCTIONS **********/

    QSqlQueryModel* getClients();

    QSqlQueryModel* getClientsTable();

private:
    QSqlDatabase db;
    QWidget* parent;

    /********** PRIVATE FUNCTIONS **********/

    void openDatabase();
    void solveFailedDatabase();
    bool createDatabase();

    bool executeQueryFile(const QString& file_name);
};

