#include "DatabaseManager.h"

/********** CONSTRUCTOR **********/

DatabaseManager::DatabaseManager(QWidget* parent)
{
    this->parent = parent;

    // Check if the backup folder exist
    QDir dir("./" + this->backup_folder);

    if (!dir.exists()) {
        if (!dir.mkpath("./")) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("La cartella di backup non è stata trovata e non è possibile crearla.\n"
                            "Verificare che la cartella di installazione non sia protetta da scrittura.");
            message.exec();
        }
    }

    if (this->openDatabase()) {

        this->connected = true;

        this->initSettings();

        if (this->isBackupRequired()) {
            this->backupDatabase();
        }

        this->deleteOldBackups();
    }
    else {
        this->connected = false;
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
    QFile config_file("config.cfg");
    config_file.open(QIODevice::ReadOnly);
    this->db_path = config_file.readAll();
    config_file.close();

    if (!QFile::exists(this->db_path)) {
        return this->solveDatabaseConnectionFailure();
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
        this->db.setDatabaseName(this->db_path);

        if (!this->db.open()) {
            return this->solveDatabaseConnectionFailure();
        }

        return true;
    }
}

bool DatabaseManager::solveDatabaseConnectionFailure()
{
    QMessageBox message;
    QPushButton* newBtn = message.addButton("Nuovo", QMessageBox::ActionRole);
    QPushButton* backupBtn = message.addButton("Ripristina", QMessageBox::ActionRole);
    QPushButton* openBtn = message.addButton("Apri", QMessageBox::ActionRole);
    QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Warning);
    message.setText("Non e' stato possibile accedere al database.\n"
                    "Creane uno nuovo se è il primo utilizzo del sofware o se non hai un backup.\n"
                    "In alternativa ripristina un backup se disponibile.");
    message.exec();

    if (message.clickedButton() == (QAbstractButton*)newBtn) {
        // Create a new database
        // If the database creation fails
        if (!this->createDatabase()) {
            // Ask again what to do
            return this->solveDatabaseConnectionFailure();
        }
        else {
            return true;
        }
    }
    else if (message.clickedButton() == (QAbstractButton*)backupBtn) {
        // Copy the last .db file from the backups folder if available
        QDir directory("./" + this->backup_folder);
        QStringList files = directory.entryList(QStringList() << "*.db", QDir::Files);

        if (files.length() > 0) {
            // Find the last backup created
            QString max_date = "00000000";
            QString backup_file = "";
            foreach(QString filename, files) {
                QStringList date = filename.split("-");

                if (date[0].compare(max_date) > 0) {
                    max_date = date[0];
                    backup_file = filename;
                }
            }

            if (!QFile::copy("./" + this->backup_folder + "/" + backup_file, this->db_path)) {
                QMessageBox message;
                message.setWindowTitle("Funeraria");
                message.setIcon(QMessageBox::Critical);
                message.setText("Il ripristino del file di backup non è riuscito.");
                message.exec();

                return false;
            }

            // Try to open the just recovered backup
            if (!this->openDatabase()) {
                QMessageBox message;
                message.setWindowTitle("Funeraria");
                message.setIcon(QMessageBox::Warning);
                message.setText("File di backup ripristinato, ma potrebbe essere danneggiato. \n"
                                "Non è stato possibile aprire il database.");
                message.exec();

                return this->solveDatabaseConnectionFailure();
            }

            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("File di backup ripristinato.");
            message.exec();

            return true;
        }

        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non sono stati trovati file di backup da ripristinare.");
        message.exec();

        // Ask again to the user how to solve the problem
        return this->solveDatabaseConnectionFailure();
    }
    if (message.clickedButton() == (QAbstractButton*)openBtn) {
        // Prompt the user to select the db file
        this->db_path = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3)");

        if (!this->db_path.isEmpty()) {
            // Rewrite the content of the config file with the new db path
            QFile config_file("config.cfg");
            config_file.open(QIODeviceBase::ReadWrite | QIODeviceBase::Truncate);

            QTextStream outStream(&config_file);
            outStream << this->db_path;
            config_file.close();

            return this->openDatabase();
        }
        else {
            // Ask again what to do
            return this->solveDatabaseConnectionFailure();
        }
    }
    else {
        // The user has decided not to solve the problem
        return false;
    }
}

bool DatabaseManager::createDatabase()
{
    // Can't use the code inside openDatabase() method, because at this point no DB yet exists
    // thus a new one must be created here before to execute the queries from the sql file
    this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
    this->db.setDatabaseName(this->db_path);

    if (!this->db.open()) {
        return this->solveDatabaseConnectionFailure();
    }
    else {
        // Ask for the sql file to open to generate the new database
        QString sqlFile = QFileDialog::getOpenFileName(this->parent, "Selezione file sql per generare il database", "./", "Database (*.sql)");

        // If no file is selected or if the sql execution fails
        if (!QFile::exists(sqlFile) || sqlFile.isEmpty() || !this->executeQueryFile(sqlFile)) {
            this->db.close();
            return this->solveDatabaseConnectionFailure();
        }

        return true;
    }
}

void DatabaseManager::initSettings() {
    QSqlQuery query = QSqlQuery(this->db);

    query.prepare("SELECT name, value FROM settings");

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile recuperare i settaggi per la gestine dei backup.\n"
                        "Sono stati caricati i valori predefiniti");
        message.exec();
        return;
    }

    while (query.next()) {
        if (query.value("name") == "backup_interval") {
            this->backup_interval = query.value("value").toInt();
        }
        else if (query.value("name") == "backups_to_keep") {
            this->backups_to_keep = query.value("value").toInt();
        }
    }
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

    // Copy of the .db file, no need to check if it exists, 
    // the program arrives here only after having solved any issues with the database
    if (!QFile::copy(this->db_path, "./" + this->backup_folder + "/" + today + "-database.db")) {
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
        QSqlQuery tables_query("SELECT name FROM sqlite_master WHERE type='table'", this->db);
        while (tables_query.next()) {
            QString table_name = tables_query.value(0).toString();

            // Exclude reserved tables generatead automatically by sqlite
            if (table_name == "sqlite_sequence") continue;

            QSqlQuery tables_dump("SELECT sql FROM sqlite_master WHERE name='" + table_name + "'", this->db);
            if (tables_dump.next()) {
                QString create_table_sql = tables_dump.value(0).toString();
                out << create_table_sql << ";\n";

                QSqlQuery data_dump("SELECT * FROM " + table_name, this->db);
                while (data_dump.next()) {
                    out << "INSERT INTO " << table_name << " VALUES (";

                    for (int i = 0; i < data_dump.record().count(); i++) {
                        if (i > 0) {
                            out << ", ";
                        }

                        QVariant value = data_dump.value(i);
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
    QString backup_path = "./" + this->backup_folder;
    QDir directory(backup_path);
    QStringList files = directory.entryList(QStringList() << "*.db" << "*.sql", QDir::Files);

    // * 2 is there because there are 2 files (1 .db and 1 .sql) for each backup
    int backups_to_delete = files.length() - (this->backups_to_keep * 2);

    if (backups_to_delete > 0) {
        // Sort the the list of files found, having the format yyyyMMdd-backup they can be alphabetically sorted
        files.sort();

        for (int i = 0; i < backups_to_delete; i++) {
            QFile::remove(backup_path + "/" + files[i]);
        }
    }
}
