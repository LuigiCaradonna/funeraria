#include "DatabaseManager.h"

/********** CONSTRUCTOR **********/

DatabaseManager::DatabaseManager(const QString& db_path, QWidget* parent)
{
    this->parent = parent;
    this->db_path = db_path;
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
    queryList->prepare("select name from clients order by position asc;");
    queryList->exec();
    model->setQuery(*queryList);
    delete(queryList);

    return model;
}

QSqlQueryModel* DatabaseManager::getClientsTable()
{
    QSqlQueryModel* modelTable = new QSqlQueryModel();
    QSqlQuery* queryTable = new QSqlQuery(db);
    queryTable->prepare("select * from clients;");
    queryTable->exec();
    modelTable->setQuery(*queryTable);
    delete(queryTable);

    return modelTable;
}

/********** PRIVATE FUNCTIONS **********/

void DatabaseManager::openDatabase()
{
    if (!Helpers::fileExists(this->db_path.toStdString())) {
        this->db_connected = false;
        this->solveDatabaseConnectionFailure();
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
        this->db.setDatabaseName(this->db_path);

        if (!this->db.open()) {
            this->db_connected = false;
            this->solveDatabaseConnectionFailure();
        }
        else {
            this->db_connected = true;
        }
    }
}

void DatabaseManager::solveDatabaseConnectionFailure()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::critical(this->parent, "Funeraria",
        "Non e' stato possibile accedere al database.\n"
        "Vuoi crearne uno nuovo?\n"
        "In alternativa puoi aprire un file differente.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Open);

    if (reply == QMessageBox::Yes) {
        // Create a new database
        // If the database creation fails
        if (!this->createDatabase()) {
            // Ask again what to do
            this->solveDatabaseConnectionFailure();
        }
    }
    else if (reply == QMessageBox::Open) {
        this->db_path = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3");

        if (!this->db_path.isEmpty()) {
            this->openDatabase();
        }
        else {
            // Ask again what to do
            this->solveDatabaseConnectionFailure();
        }
    }
    else {
        // The user has decided not to solve the problem
        this->db_connected = false;
    }
}

bool DatabaseManager::createDatabase()
{
    this->db_path = QFileDialog::getSaveFileName(this->parent, "Salva", "./", "Database (*.db *.sqlite *.sqlite3)");

    // If the user does not select any file
    if (this->db_path.isEmpty()) {
        this->db_connected = false;
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName(this->db_path);

        if (!this->db.open()) {
            this->db_connected = false;
            this->solveDatabaseConnectionFailure();
        }
        else {
            this->db_connected = true;
            
            // Ask for the sql file to open
            QString sqlFile = QFileDialog::getOpenFileName(this->parent, "Selezione file sql", "./", "Database (*.sql)");

            // If no file is selected or if the sql execution fails
            if (sqlFile.isEmpty() || !this->executeQueryFile(sqlFile)) {
                this->db_connected = false;
                this->db.close();
            }
        }
    }
    
    return this->db_connected;
}

bool DatabaseManager::executeQueryFile(const QString& file_name) {
    QFile file(file_name);

    // Read the file content
    file.open(QIODevice::ReadOnly);
    QString query_string(file.readAll());
    file.close();

    QSqlQuery* query = new QSqlQuery(this->db);

    // Check if SQL Driver supports Transactions
    if (this->db.driver()->hasFeature(QSqlDriver::Transactions)) {
        // Replace comments and tabs and new lines with space
        query_string = query_string.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        // Remove not necessary spaces
        query_string = query_string.trimmed();

        // Extracting queries
        QStringList qList = query_string.split(';', Qt::SkipEmptyParts);

        // Detecting special queries (`begin transaction` and `commit`).
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        // Check if the SQL file has a transaction set
        bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();

        // If the SQL file did not had a transaction set
        if (!isStartedWithTransaction)
            this->db.transaction();

        // Execute the queries
        foreach(const QString & s, qList) {
            if (re_transaction.match(s).hasMatch())    // Special query detected
                this->db.transaction();
            else if (re_commit.match(s).hasMatch())    // Special query detected
                this->db.commit();
            else {
                query->exec(s);                        // Execute normal query
                if (query->lastError().type() != QSqlError::NoError) {
                    this->db.rollback();

                    return false;
                }
            }
        }

        // If the SQL file did not had a transaction set
        if (!isStartedWithTransaction)
            this->db.commit();

        return true;
    }
    else {
        // Sql Driver doesn't supports transaction
        // Remove special queries (`begin transaction` and `commit`)
        query_string = query_string.replace(QRegularExpression("(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        query_string = query_string.trimmed();

        // Execute each individual queries
        QStringList qList = query_string.split(';', Qt::SkipEmptyParts);

        foreach(const QString & s, qList) {
            query->exec(s);
            if (query->lastError().type() != QSqlError::NoError) {
                return false;
            };
        }

        return true;
    }
}