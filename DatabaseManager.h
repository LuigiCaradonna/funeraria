#pragma once
#include <QSqlRecord>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include "Settings.h"
#include "Tomb.h"
#include "Client.h"
#include "Config.h"

class DatabaseManager
{
public:
    QSqlDatabase db;
    bool connected;

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the DatabaseManager object.
     *
     * @param	QWidget*    parent	- Pointer to the parent widget
     */
    DatabaseManager(QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the DatabaseManager object.
     */
    ~DatabaseManager();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Given the name of the QTableWidget column passed, retrieves the corresponding column's name
     * contained into the database if the column is sortable
     *
     * @param const QString& column - Name of the column to retrieve
     * 
     * @return the name of the column if it is sortable, empty string if the given colum can't be sorted
     */
    QString getSortableColumnName(const QString& column);

    /*
     * Saves the database in CSV format
     *
     * @return boolean true on success, false on failure
     */
    bool backupToCSV();

    /*
     * Reloads the database
     *
     * @return boolean true on success, false on failure
     */
    bool reloadDatabase();

private:
    QWidget* parent;
    Settings* settings;
    QString db_file = "./funeraria.db";
    QString backup_folder = "backups";
    // Interval in days to create a new backup
    int backup_interval = 1;
    // Number of backups to keep
    int backups_to_keep = 10;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Opens a database connection
     * 
     * @return boolean true if the database is opened, false on failure
     */
    bool openDatabase();

    /*
     * Gives options to solve eventual problems with the database opening
     *
     * @return boolean true if the connection has been established, false on failure
     */
    bool solveDatabaseConnectionFailure();

    /*
     * Creates a new database
     *
     * @return boolen true if the operation succeeds, false on failure
     */
    bool createDatabase();

    /*
     * Reads the database and load the settings
     *
     * @return void
     */
    void initSettings();

    /*
     * Opens and executes the queries contained into the provided sql file
     * 
     * @param const QString& file_name - Path to the file to open
     *
     * @return boolen true if the operation succeeds, false on failure
     */
    bool executeQueryFile(const QString& file_name);

    /*
     * Creates an sql file containing a backup of the database and a copy of the db file.
     * The filenames will have a timestamp to distinguish them.
     *
     * @return void
     */
    void backupDatabase();

    /*
     * Checks if a new backup is required
     *
     * @return boolen true if a new backup is required, false on failure
     */
    bool isBackupRequired();

    /*
     * Deletes the older backups
     * 
     * @return void
     */
    void deleteOldBackups();
};

