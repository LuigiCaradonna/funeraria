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

private:
    QWidget* parent;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Opens a database connection
     * 
     * @return void
     */
    void openDatabase();

    /*
     * Gives options to solve eventual problems with the database opening
     *
     * @return void
     */
    void solveDatabaseConnectionFailure();

    /*
     * Creates a new database
     *
     * @return boolena true if the operation succeeds, false otherwise
     */
    bool createDatabase();

    /*
     * Opens and executes the queries contained into the provided sql file
     * 
     * @param const QString& file_name - Path to the file to open
     *
     * @return boolena true if the operation succeeds, false otherwise
     */
    bool executeQueryFile(const QString& file_name);
};

