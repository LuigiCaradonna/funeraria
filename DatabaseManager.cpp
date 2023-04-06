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
            
            // Create the tables
            if (this->createClientsTable());
            this->createFlamesTable();
            this->createMaterialsTable();
            this->createVasesTable();
            this->createLampsTable();
            this->createTombsTable();
            qDebug() << "###### New DB Created";
        }
    }
    
    return this->db_connected;
}

bool DatabaseManager::createClientsTable()
{
    QSqlQuery* queryList = new QSqlQuery(this->db);
    queryList->prepare(
        "CREATE TABLE 'clients' ("
            "'id'			INTEGER,"
            "'name'			TEXT NOT NULL,"
            "'email'		TEXT,"
            "'address'		TEXT,"
            "'phone'		TEXT,"
            "'active'		NUMERIC NOT NULL DEFAULT 1,"
            "'created_at'	TEXT,"
            "'edited_at'	TEXT,"
            "PRIMARY KEY('id' AUTOINCREMENT)"
        "); "
    );

    return queryList->exec();
}

bool DatabaseManager::createMaterialsTable()
{
    QSqlQuery* queryList = new QSqlQuery(this->db);
    queryList->prepare(
        "CREATE TABLE 'materials' ("
        "'id'			INTEGER,"
        "'name'			TEXT NOT NULL,"
        "'created_at'	TEXT,"
        "'edited_at'	TEXT,"
        "PRIMARY KEY('id' AUTOINCREMENT)"
        "); "
    );

    return queryList->exec();
}

bool DatabaseManager::createFlamesTable()
{
    QSqlQuery* queryList = new QSqlQuery(this->db);
    queryList->prepare(
        "CREATE TABLE 'flames' ("
            "'id'			INTEGER,"
            "'name'			TEXT NOT NULL,"
            "'created_at'	TEXT,"
            "'edited_at'	TEXT,"
            "PRIMARY KEY('id' AUTOINCREMENT)"
        "); "
    );

    return queryList->exec();
}

bool DatabaseManager::createVasesTable()
{
    QSqlQuery* queryList = new QSqlQuery(this->db);
    queryList->prepare(
        "CREATE TABLE 'vases' ("
            "'id'			INTEGER,"
            "'name'			TEXT NOT NULL,"
            "'created_at'	TEXT,"
            "'edited_at'	TEXT,"
            "PRIMARY KEY('id' AUTOINCREMENT)"
        "); "
    );

    return queryList->exec();
}

bool DatabaseManager::createLampsTable()
{
    QSqlQuery* queryList = new QSqlQuery(this->db);
    queryList->prepare(
        "CREATE TABLE 'lamps' ("
            "'id'			INTEGER,"
            "'name'			TEXT NOT NULL,"
            "'created_at'	TEXT,"
            "'edited_at'	TEXT,"
            "PRIMARY KEY('id' AUTOINCREMENT)"
        "); "
    );

    return queryList->exec();
}

bool DatabaseManager::createTombsTable()
{
    QSqlQuery* queryList = new QSqlQuery(this->db);
    queryList->prepare(
        "CREATE TABLE 'tombs' ("
            "'id'		    		INTEGER,"
            "'progressive'	    	INTEGER NOT NULL,"
            "'client_id'		    INTEGER NOT NULL,"
            "'name'		    		TEXT NOT NULL,"
            "'additional_names'	    TEXT,"
            "'price'	    		REAL,"
            "'material_id'	    	INTEGER,"
            "'vase_id'			    INTEGER,"
            "'lamp_id'	    		INTEGER,"
            "'flame_id'		    	INTEGER,"
            "'notes'			    TEXT,"
            "'accessories_mounted'	NUMERIC NOT NULL,"
            "'ordered_at'	    	TEXT NOT NULL,"
            "'proofed_at'		    TEXT,"
            "'confirmed_at'	    	TEXT,"
            "'done_at'			    TEXT,"
            "'delivered_at'		    TEXT,"
            "'created_at'	    	TEXT,"
            "'edited_at'		    TEXT,"
            "FOREIGN KEY('client_id') REFERENCES 'clients'('id') ON UPDATE cascade,"
            "FOREIGN KEY('material_id') REFERENCES 'materials'('id') ON UPDATE cascade,"
            "FOREIGN KEY('vase_id') REFERENCES 'vases'('id') ON UPDATE cascade,"
            "FOREIGN KEY('lamp_id') REFERENCES 'lamps'('id') ON UPDATE cascade,"
            "FOREIGN KEY('flame_id') REFERENCES 'flames'('id') ON UPDATE cascade,"
            "PRIMARY KEY('id' AUTOINCREMENT)"
        ");"
    );

    return queryList->exec();
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

void DatabaseManager::executeQueryFile(QFile& qf) {
    //Read query file content
    qf.open(QIODevice::ReadOnly);
    QString queryStr(qf.readAll());
    qf.close();

    QSqlQuery* query = new QSqlQuery(this->db);

    //Check if SQL Driver supports Transactions
    if (this->db.driver()->hasFeature(QSqlDriver::Transactions)) {
        //Replace comments and tabs and new lines with space
        queryStr = queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        //Remove waste spaces
        queryStr = queryStr.trimmed();

        //Extracting queries
        QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);

        //Initialize regular expression for detecting special queries (`begin transaction` and `commit`).
        //NOTE: I used new regular expression for Qt5 as recommended by Qt documentation.
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        //Check if query file is already wrapped with a transaction
        bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
        if (!isStartedWithTransaction)
            this->db.transaction();     //<=== not wrapped with a transaction, so we wrap it with a transaction.

        //Execute each individual queries
        foreach(const QString & s, qList) {
            if (re_transaction.match(s).hasMatch())    //<== detecting special query
                this->db.transaction();
            else if (re_commit.match(s).hasMatch())    //<== detecting special query
                this->db.commit();
            else {
                query->exec(s);                        //<== execute normal query
                if (query->lastError().type() != QSqlError::NoError) {
                    qDebug() << query->lastError().text();
                    this->db.rollback();                    //<== rollback the transaction if there is any problem
                }
            }
        }
        if (!isStartedWithTransaction)
            this->db.commit();          //<== ... completing of wrapping with transaction

        //Sql Driver doesn't supports transaction
    }
    else {
        //...so we need to remove special queries (`begin transaction` and `commit`)
        queryStr = queryStr.replace(QRegularExpression("(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        queryStr = queryStr.trimmed();

        //Execute each individual queries
        QStringList qList = queryStr.split(';', Qt::SkipEmptyParts);
        foreach(const QString & s, qList) {
            query->exec(s);
            if (query->lastError().type() != QSqlError::NoError) qDebug() << query->lastError().text();
        }
    }
}