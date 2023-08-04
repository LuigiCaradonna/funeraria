#include "Accessory.h"

/********** CONSTRUCTOR **********/

Accessory::Accessory(QSqlDatabase* db, const QString& table, QWidget* parent)
    : db(db), table(table), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    if (table == "material") {
        this->setWindowTitle("Inserimento materiale");
    }
    else if (table == "vase") {
        this->setWindowTitle("Inserimento vaso");
    }
    else if (table == "lamp") {
        this->setWindowTitle("Inserimento lume");
    }
    else if (table == "flame") {
        this->setWindowTitle("Inserimento fiamma");
    }

    this->connect(this->ui.btnOk, &QPushButton::clicked, this, &Accessory::slotAddAccessory);
    this->connect(this->ui.btnCancel, &QPushButton::clicked, this, &Accessory::slotCloseDialog);
}

/********** DESTRUCTOR **********/

Accessory::~Accessory()
{
}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Accessory::get()
{
    QList<QMap<QString, QString>> accessories;
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT code, name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["code"] = query.value("code").toString();
        row["name"] = query.value("name").toString();

        accessories.append(row);
    }

    return accessories;
}

QString Accessory::getCode(const QString& name)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT code FROM " + this->table + " WHERE name = :name");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    if (query.next()) {
        return query.value("code").toString();
    }

    return "";
}

QStringList Accessory::getNames()
{
    QStringList accessories;
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT name FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        accessories.append(query.value("name").toString());
    }

    return accessories;
}

void Accessory::remove(const QString& code)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE code = :code; ");
    query.bindValue(":code", code);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Eliminazione eseguita.");
    message.exec();
}

void Accessory::update(const QString& code, const QString& name)
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("UPDATE " + this->table + " SET code = :code, name = :name, edited_at = :edited_at WHERE code = :codew; ");
    query.bindValue(":code", code.trimmed());
    query.bindValue(":name", name.trimmed());
    query.bindValue(":edited_at", date);
    query.bindValue(":codew", code.trimmed());

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Modifica eseguita.");
    message.exec();
}

/********** PRIVATE SLOTS **********/

void Accessory::slotAddAccessory()
{
    if (this->ui.leCode->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il codice è obbligatorio");
        message.exec();

        return;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome è obbligatorio");
        message.exec();

        return;
    }

    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("INSERT INTO " + this->table + " (code, name, created_at, edited_at) VALUES (:code, :name, :created_at, :edited_at);");
    query.bindValue(":code", this->ui.leCode->text().trimmed());
    query.bindValue(":name", this->ui.leName->text().trimmed());
    query.bindValue(":created_at", date);
    query.bindValue(":edited_at", date);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return;
    }

    // Insert successful

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Inserimento eseguito.");
    message.exec();

    // Reset the input fields
    this->ui.leCode->setText("");
    this->ui.leName->setText("");
    // Close the dialog
    this->close();
}

void Accessory::slotCloseDialog()
{
    this->close();
}
