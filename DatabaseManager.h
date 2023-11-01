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

class DatabaseManager
{
public:
    QSqlDatabase db;
    bool connected;
    QString db_path = "./";
    QString db_name = "funeraria.db";

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the DatabaseManager object.
     *
     * @param	QWidget*        parent	- Pointer to the parent widget
     */
    DatabaseManager(QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the DatabaseManager object.
     */
    ~DatabaseManager();

private:
    QWidget* parent;
    QString backup_folder = "backups";
    // Interval in days to create a new backup
    int backup_interval = 3;
    // Number of backups to keep
    int backups_to_keep = 1;

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
     * @return boolean true if the connection has been established, false otherwise
     */
    bool solveDatabaseConnectionFailure();

    /*
     * Creates a new database
     *
     * @return boolen true if the operation succeeds, false otherwise
     */
    bool createDatabase();

    /*
     * Opens and executes the queries contained into the provided sql file
     * 
     * @param const QString& file_name - Path to the file to open
     *
     * @return boolen true if the operation succeeds, false otherwise
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
     * @return boolen true if a new backup is required, false otherwise
     */
    bool isBackupRequired();

    /*
     * Deletes the older backups
     * 
     * @return void
     */
    void deleteOldBackups();
};

