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

    /*
     * Opens the file browser.
     *
     * @return void
     */
    QString browseFile();

    /*
     * Opens the file browser to select the folder where to save the new file.
     *
     * @return void
     */
    QString saveDestination();

    void openDatabase();
    void solveFailedDatabase();
    bool createDatabase();

};

