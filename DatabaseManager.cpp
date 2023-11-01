#include "DatabaseManager.h"

/********** CONSTRUCTOR **********/

DatabaseManager::DatabaseManager(const QString& path, QWidget* parent)
{
    this->parent = parent;
    this->path = path;
    if (this->openDatabase()) {
        if (this->isBackupRequired()) {
            this->backupDatabase();
        }

        this->deleteOldBackups();
    }
}

/********** DESTRUCTOR **********/

DatabaseManager::~DatabaseManager()
{
    this->db.close();
    QSqlDatabase::removeDatabase(this->db.connectionName());
}

/********** PUBLIC FUNCTIONS **********/

/********** PRIVATE FUNCTIONS **********/

bool DatabaseManager::openDatabase()
{
    if (!QFile::exists(this->path)) {
        this->connected = false;
        if (!this->solveDatabaseConnectionFailure()) {
            return false;
        }

        return true;
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
        this->db.setDatabaseName(this->path);

        if (!this->db.open()) {
            this->connected = false;
            if (!this->solveDatabaseConnectionFailure()) {
                return false;
            }

            return true;
        }
        else {
            this->connected = true;
            return true;
        }
    }
}

bool DatabaseManager::solveDatabaseConnectionFailure()
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
            if (!this->solveDatabaseConnectionFailure()) {
                return false;
            }

            return true;
        }

        return true;
    }
    else if (message.clickedButton() == (QAbstractButton*)openBtn) {
        this->path = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3)");

        if (!this->path.isEmpty()) {
            if (!this->openDatabase()) {
                return false;
            }

            return true;
        }
        else {
            // Ask again what to do
            if (!this->solveDatabaseConnectionFailure()) {
                return false;
            }

            return true;
        }
    }
    else {
        // The user has decided not to solve the problem
        this->connected = false;
        return false;
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

void DatabaseManager::backupDatabase()
{
    QString today = QDate::currentDate().toString("yyyyMMdd");

    // Copy of the .db file
    if (!QFile::copy(this->path, "./" + this->backup_folder + "/" + today + "-database.db")) {
        QMessageBox message;
        message.setWindowTitle("Funeraria Backup");
        message.setIcon(QMessageBox::Critical);
        message.setText("Impossibile eseguire la copia di backup del database.");
        message.exec();
        return;
    }

    // Create a file for the SQL statements
    QFile sqlFile("./" + this->backup_folder + "/" + today + "-database.sql");
    if (!sqlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria Backup");
        message.setIcon(QMessageBox::Critical);
        message.setText("Impossibile aprire il nuovo file .sql per il backup del database.");
        message.exec();
        return;
    }

    QTextStream out(&sqlFile);

    // Fetch and write SQL statements to the file
    QSqlQuery query(this->db);
    if (query.exec("PRAGMA foreign_keys=OFF") && query.exec("BEGIN")) {
        // Iterate through tables and write SQL to recreate them
        QSqlQuery tablesQuery("SELECT name FROM sqlite_master WHERE type='table'", this->db);
        while (tablesQuery.next()) {
            QString tableName = tablesQuery.value(0).toString();
            QSqlQuery tableDump("SELECT sql FROM sqlite_master WHERE name='" + tableName + "'", this->db);
            if (tableDump.next()) {
                QString createTableSQL = tableDump.value(0).toString();
                out << createTableSQL << ";\n";

                QSqlQuery dataDump("SELECT * FROM " + tableName, this->db);
                while (dataDump.next()) {
                    out << "INSERT INTO " << tableName << " VALUES (";

                    for (int i = 0; i < dataDump.record().count(); i++) {
                        if (i > 0) {
                            out << ", ";
                        }

                        QVariant value = dataDump.value(i);
                        if (value.type() == QVariant::String) {
                            out << "\"" << value.toString() << "\"";
                        }
                        else {
                            out << value.toString();
                        }
                    }

                    out << ");\n";
                }
            }
        }

        if (!query.exec("COMMIT")) {
            qDebug() << "Failed to commit SQL transaction: " << query.lastError().text();
            QMessageBox message;
            message.setWindowTitle("Funeraria Backup");
            message.setIcon(QMessageBox::Critical);
            message.setText("Impossibile generare correttamente il file di backup .sql. Transazione fallita.");
            message.exec();
            // Rollback not required, the database was only read
            sqlFile.close();
            return;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria Backup");
        message.setIcon(QMessageBox::Critical);
        message.setText("Impossibile iniziare la transazione per generare il file di backupp .sql");
        message.exec();
        return;
    }

    sqlFile.close();
}

bool DatabaseManager::isBackupRequired() {
    QDir directory("./" + this->backup_folder);
    QStringList files = directory.entryList(QStringList() << "*.db" << "*.sql", QDir::Files);

    // If the folder is empty, it means that no backup has been created
    if (files.isEmpty()) {
        return true;
    }

    QString max_date = "00000000";
    foreach(QString filename, files) {
        QStringList date = filename.split("-");
        
        if (date[0].compare(max_date) > 0) {
            max_date = date[0];
        }
    }

    QDate today = QDate::currentDate();
    QDate date = QDate::fromString(max_date, "yyyyMMdd");

    if (date.daysTo(today) >= this->backup_interval) {
        return true;
    }

    return false;
}

void DatabaseManager::deleteOldBackups()
{
    qDebug() << "Initiatin deletion";
    QString backup_path = "./" + this->backup_folder;
    QDir directory(backup_path);
    QStringList files = directory.entryList(QStringList() << "*.db" << "*.sql", QDir::Files);

    qDebug() << "Files found: " + QString::number(files.length());
    
    // * 2 is there because there are 2 files (1 .db and 1 .sql) for each backup
    int backups_to_delete = files.length() - (this->backups_to_keep * 2);

    qDebug() << "Backups to delete: " + QString::number(backups_to_delete);

    if (backups_to_delete > 0) {
        // Sort the the list of files found, having the format yyyyMMdd-backup they can be alphabetically sorted
        files.sort();

        for (int i = 0; i < backups_to_delete; i++) {
            QFile::remove(backup_path + "/" + files[i]);
        }
    }
}
