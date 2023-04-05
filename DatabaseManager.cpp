#include "DatabaseManager.h"

/********** CONSTRUCTOR **********/

DatabaseManager::DatabaseManager(const QString& db_path, QWidget* parent)
{
    this->parent = parent;
    this->db_path = db_path;
    qDebug() << "###### " << this->db_path;
    this->openDatabase();
}

/********** DESTRUCTOR **********/

DatabaseManager::~DatabaseManager()
{
    QSqlDatabase::removeDatabase(this->db.connectionName());
    this->db.close();
}

/********** PUBLIC FUNCTIONS **********/

QSqlQueryModel* DatabaseManager::getClients()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery* queryList = new QSqlQuery(db);
    queryList->prepare("select name from clients;");
    queryList->exec();
    model->setQuery(*queryList);

    return model;
}

QSqlQueryModel* DatabaseManager::getClientsTable()
{
    QSqlQueryModel* modelTable = new QSqlQueryModel();
    QSqlQuery* queryTable = new QSqlQuery(db);
    queryTable->prepare("select * from clients;");
    queryTable->exec();
    modelTable->setQuery(*queryTable);

    return modelTable;
}

/********** PRIVATE FUNCTIONS **********/

void DatabaseManager::openDatabase()
{
    qDebug() << "###### Opening DB";

    if (!Helpers::fileExists(this->db_path.toStdString())) {
        qDebug() << "###### DB File Opening failed";
        this->db_connected = false;
        this->solveFailedDatabase();
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
        this->db.setDatabaseName(this->db_path);

        if (!this->db.open()) {
            this->db_connected = false;
            this->solveFailedDatabase();
        }
        else {
            this->db_connected = true;
            qDebug() << "###### DB SUCCESS";
        }
    }
}

void DatabaseManager::solveFailedDatabase()
{
    qDebug() << "###### Solving DB Failure";
    QMessageBox::StandardButton reply;

    reply = QMessageBox::critical(this->parent, "Funeraria",
        "Non e' stato possibile accedere al database.\n"
        "Vuoi crearne uno nuovo?\n"
        "In alternativa puoi aprire un file differente.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Open);

    if (reply == QMessageBox::Yes) {
        // Create a new database
        // If the database creation failed
        if (!this->createDatabase()) {
            // Ask again what to do
            this->solveFailedDatabase();
        }
    }
    else if (reply == QMessageBox::Open) {
        qDebug() << "###### Asking for a new DB file";
        this->db_path = this->browseFile();
        qDebug() << "###### New DB path: " << this->db_path;

        if (!this->db_path.isEmpty()) {
            this->openDatabase();
        }
        else {
            // Ask again what to do
            this->solveFailedDatabase();
        }
    }
    else {
        // Close the program
        this->parent->close();
    }
}

bool DatabaseManager::createDatabase()
{
    qDebug() << "###### Creating a new DB";
    this->db_path = this->saveDestination();

    // If the user does not select any file
    if (this->db_path.isEmpty()) {
        this->db_connected = false;
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName(this->db_path);

        if (!this->db.open()) {
            this->db_connected = false;
            this->solveFailedDatabase();
        }
        else {
            this->db_connected = true;
            // TODO: Create one function for each table and call them
            qDebug() << "###### New DB Created";
        }
    }
    
    return this->db_connected;
}

QString DatabaseManager::browseFile()
{
    qDebug() << "###### New DB file selection";
    // Filter to show only PGR files
    QString filter = "Database (*.db *.sqlite *.sqlite3)";

    // Files selection
    QString file = QFileDialog::getOpenFileName(this->parent, "Apri", "./", filter);

    qDebug() << "###### Selected file: " << file;
    return file;
}

QString DatabaseManager::saveDestination()
{
    // Filter to show only PGR files
    QString filter = "Database (*.db *.sqlite *.sqlite3)";

    // Get the folder and the file name to use to save the new file
    QString file = QFileDialog::getSaveFileName(this->parent, "Salva", "./", filter);

    return file;
}