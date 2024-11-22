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

        this->settings = new Settings(this->db);

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
    delete this->settings;
}

/********** PUBLIC FUNCTIONS **********/

QString DatabaseManager::getSortableColumnName(const QString& column) {
    if (column == "Numero") {
        return "progressive";
    }
    else if (column == "Defunto") {
        return "name";
    }
    else if (column == "Prezzo") {
        return "price";
    }
    else if (column == "Ordine") {
        return "ordered_At";
    }
    else if (column == "Provino") {
        return "proofed_at";
    }
    else if (column == "Conferma") {
        return "confirmed_at";
    }
    else if (column == "Incisione") {
        return "engraved_at";
    }
    else if (column == "Consegna") {
        return "delivered_at";
    }
    else {
        return "";
    }
}
bool DatabaseManager::backupToCSV()
{
    Client* client = new Client(this->db);
    Tomb* tomb = new Tomb(this->db);

    QStringList clients = client->getNames();

    QString year = QDate::currentDate().toString("yyyy");
    QString filename;

    // Check if the backup folder exist
    QDir dir("./" + this->backup_folder + "/" + year);

    // Check if the folder for the current year already exists
    if (!dir.exists()) {
        // Try to create the folder starting from the current folder position
        if (!dir.mkpath("./")) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("La cartella per il backup in CSV del database non è stata trovata e non è possibile crearla.\n"
                "Verificare che la cartella di backup non sia protetta da scrittura.");
            message.exec();

            return false;
        }
    }

    // For each name
    for (QString client_name : clients) {
        int client_id = client->getId(client_name);
        QList<QMap<QString, QString>> tombs = tomb->getList(client_id, year.toInt());

        // If the current client has tombs ordered in the current year
        if (tombs.size() > 0) {
            filename = client_name + ".csv";
            QFile sql_file("./" + this->backup_folder + "/" + year + "/" + filename);

            if (!sql_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return false;
            }

            QTextStream out(&sql_file);
            // Columns name row
            out << "Numero;Nome;Prezzo;Pagata;Ordine;Provino;Conferma;Incisione;Consegna;Note;" << "\n";

            // Tombs' data, one per row
            for (int i = 0; i < tombs.size(); i++) {
                out << tombs[i]["progressive"] << ";";
                out << tombs[i]["name"] << ";";
                out << tombs[i]["price"] << ";";
                out << tombs[i]["paid"] << ";";
                out << tombs[i]["ordered_at"] << ";";
                out << tombs[i]["proofed_at"] << ";";
                out << tombs[i]["confirmed_at"] << ";";
                out << tombs[i]["engraved_at"] << ";";
                out << tombs[i]["delivered_at"] << ";";
                out << tombs[i]["notes"] << ";\n";
            }

            sql_file.close();
        }
    }

    delete client;
    delete tomb;
    return true;
}
bool DatabaseManager::reloadDatabase()
{
    // Close the currently open DB
    this->db.close();

    return this->openDatabase();
}
/********** PRIVATE FUNCTIONS **********/

bool DatabaseManager::openDatabase()
{
    Config* config = new Config();

    this->db_file = config->getDbFile();

    delete config;

    if (!QFile::exists(this->db_file)) {
        return this->solveDatabaseConnectionFailure();
    }
    else {
        this->db = QSqlDatabase::addDatabase("QSQLITE", "funerariadb");
        this->db.setDatabaseName(this->db_file);

        if (!this->db.open()) {
            return this->solveDatabaseConnectionFailure();
        }

        return true;
    }
}

bool DatabaseManager::solveDatabaseConnectionFailure()
{
    QMessageBox message;
    QPushButton* new_btn = message.addButton("Nuovo", QMessageBox::ActionRole);
    QPushButton* backup_btn = message.addButton("Ripristina", QMessageBox::ActionRole);
    QPushButton* openBtn = message.addButton("Apri", QMessageBox::ActionRole);
    QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Warning);
    message.setText("Non e' stato possibile accedere al database.\n"
                    "Creane uno nuovo se è il primo utilizzo del sofware o se non hai un backup.\n"
                    "In alternativa ripristina un backup se disponibile.");
    message.exec();

    if (message.clickedButton() == (QAbstractButton*)new_btn) {
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
    else if (message.clickedButton() == (QAbstractButton*)backup_btn) {
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

            if (!QFile::copy("./" + this->backup_folder + "/" + backup_file, this->db_file)) {
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
        this->db_file = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3)");

        if (!this->db_file.isEmpty()) {
            // Rewrite the content of the config file with the new db path
            Config* config = new Config();

            config->setDbFile(this->db_file);

            delete config;

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
    this->db.setDatabaseName(this->db_file);

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
    bool errors = false;
    int interval = this->settings->getBackupInterval();
    int keep = this->settings->getBackupsToKeep();

    if (interval != -1) {
        this->backup_interval = interval;
    }
    else {
        errors = true;
    }

    if (interval != -1) {
        this->backups_to_keep = keep;
    }
    else {
        errors = true;
    }

    if (errors) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile recuperare uno o più settaggi.\n"
                        "Sono stati caricati i valori di default.");
        message.exec();
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
        bool is_started_with_transaction = re_transaction.match(qList.at(0)).hasMatch();

        // If the SQL file did not had a transaction set
        if (!is_started_with_transaction) {
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
        if (!is_started_with_transaction) {
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
    if (!QFile::copy(this->db_file, "./" + this->backup_folder + "/" + today + "-database.db")) {
        QMessageBox message;
        message.setWindowTitle("Funeraria Backup");
        message.setIcon(QMessageBox::Critical);
        message.setText("Impossibile eseguire la copia di backup del database.");
        message.exec();
        return;
    }

    // Create a file for the SQL statements
    QFile sql_file("./" + this->backup_folder + "/" + today + "-database.sql");
    if (!sql_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria Backup");
        message.setIcon(QMessageBox::Critical);
        message.setText("Impossibile aprire il nuovo file .sql per il backup del database.");
        message.exec();
        return;
    }

    QTextStream out(&sql_file);

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
            QMessageBox message;
            message.setWindowTitle("Funeraria Backup");
            message.setIcon(QMessageBox::Critical);
            message.setText("Impossibile generare correttamente il file di backup .sql. \nTransazione fallita.");
            message.exec();
            // Rollback not required, the database was only read
            sql_file.close();
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

    sql_file.close();
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
