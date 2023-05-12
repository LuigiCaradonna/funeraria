#include "DatabaseManager.h"

/********** CONSTRUCTOR **********/

DatabaseManager::DatabaseManager(const QString& path, QWidget* parent)
{
    this->parent = parent;
    this->path = path;
    this->openDatabase();
}

/********** DESTRUCTOR **********/

DatabaseManager::~DatabaseManager()
{
    this->db.close();
    QSqlDatabase::removeDatabase(this->db.connectionName());
}

/********** PUBLIC FUNCTIONS **********/

/********** PRIVATE FUNCTIONS **********/

void DatabaseManager::openDatabase()
{
    if (!QFile::exists(this->path)) {
        this->connected = false;
        this->solveDatabaseConnectionFailure();
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
        this->db.setDatabaseName(this->path);

        if (!this->db.open()) {
            this->connected = false;
            this->solveDatabaseConnectionFailure();
        }
        else {
            this->connected = true;
        }
    }
}

void DatabaseManager::solveDatabaseConnectionFailure()
{
    QMessageBox message;
    QPushButton* newBtn = message.addButton("Nuovo", QMessageBox::ActionRole);
    QPushButton* openBtn = message.addButton("Apri", QMessageBox::ActionRole);
    QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Warning);
    message.setText("Non e' stato possibile accedere al database.\n"
        "Vuoi crearne uno nuovo?\n"
        "In alternativa puoi aprire un file differente.");
    message.exec();

    if (message.clickedButton() == (QAbstractButton*)newBtn) {
        // Create a new database
        // If the database creation fails
        if (!this->createDatabase()) {
            // Ask again what to do
            this->solveDatabaseConnectionFailure();
        }
    }
    else if (message.clickedButton() == (QAbstractButton*)openBtn) {
        this->path = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3)");

        if (!this->path.isEmpty()) {
            this->openDatabase();
        }
        else {
            // Ask again what to do
            this->solveDatabaseConnectionFailure();
        }
    }
    else {
        // The user has decided not to solve the problem
        this->connected = false;
    }
}

bool DatabaseManager::createDatabase()
{
    this->path = QFileDialog::getSaveFileName(this->parent, "Salva", "./", "Database (*.db *.sqlite *.sqlite3)");

    // If the user does not select any file
    if (this->path.isEmpty()) {
        this->connected = false;
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName(this->path);

        if (!this->db.open()) {
            this->connected = false;
            this->solveDatabaseConnectionFailure();
        }
        else {
            this->connected = true;
            
            // Ask for the sql file to open
            QString sqlFile = QFileDialog::getOpenFileName(this->parent, "Selezione file sql", "./", "Database (*.sql)");

            // If no file is selected or if the sql execution fails
            if (!QFile::exists(sqlFile) || sqlFile.isEmpty() || !this->executeQueryFile(sqlFile)) {
                this->connected = false;
                this->db.close();
                this->solveDatabaseConnectionFailure();
            }
        }
    }
    
    return this->connected;
}

bool DatabaseManager::executeQueryFile(const QString& file_name) {

    if (!QFile::exists(file_name)) {
        return false;
    }

    QFile file(file_name);

    // Read the file content
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QString query_string(file.readAll());
    file.close();

    QSqlQuery query = QSqlQuery(this->db);

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
        if (!isStartedWithTransaction) {
            this->db.transaction();
        }

        // Execute the queries
        foreach(const QString & s, qList) {
            if (re_transaction.match(s).hasMatch())    // Special query detected
                this->db.transaction();
            else if (re_commit.match(s).hasMatch())    // Special query detected
                this->db.commit();
            else {
                query.exec(s);                        // Execute normal query
                if (query.lastError().type() != QSqlError::NoError) {
                    this->db.rollback();

                    QMessageBox message;
                    message.setWindowTitle("Funeraria");
                    message.setIcon(QMessageBox::Critical);
                    message.setText(query.lastError().text());
                    message.exec();

                    return false;
                }
            }
        }

        // If the SQL file did not had a transaction set
        if (!isStartedWithTransaction) {
            this->db.commit();
        }

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
            query.exec(s);
            if (query.lastError().type() != QSqlError::NoError) {
                return false;
            };
        }

        return true;
    }
}