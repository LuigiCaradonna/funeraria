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
    QSqlDatabase db;
    bool connected;
    QString path;

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the DatabaseManager object.
     *
     * @param	const QString&	path	- Path to the database file
     * @param	QWidget*        parent	- Pointer to the parent widget
     */
    DatabaseManager(const QString& path, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the DatabaseManager object.
     */
    ~DatabaseManager();

    /********** PUBLIC FUNCTIONS **********/


private:
    QWidget* parent;

    /********** PRIVATE FUNCTIONS **********/

    void openDatabase();
    void solveDatabaseConnectionFailure();
    bool createDatabase();

    bool executeQueryFile(const QString& file_name);
};

