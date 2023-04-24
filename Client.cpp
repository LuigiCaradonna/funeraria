#include "Client.h"

/********** CONSTRUCTOR **********/

Client::Client(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
	this->ui.setupUi(this);

    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &Client::slotSave);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &Client::slotCloseDialog);
}

/********** DESTRUCTOR **********/

Client::~Client()
{}

/********** PUBLIC FUNCTIONS **********/

int Client::getId(const QString& name)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT id FROM " + this->table + " WHERE name = :name;");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        return query.value("id").toInt();
    }

    return 0;
}

QStringList Client::getNames()
{
    QStringList names{};
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT name FROM " + this->table + " ORDER BY position ASC;");

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        names.append(query.value("name").toString());
    }

    return names;
}

QList<QMap<QString, QString>> Client::get()
{
    QList<QMap<QString, QString>> list{};

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QMap<QString, QString> row;

        row["id"] = query.value("id").toString();
        row["position"] = query.value("position").toString();
        row["name"] = query.value("name").toString();
        row["email"] = query.value("email").toString();
        row["address"] = query.value("address").toString();
        row["phone"] = query.value("phone").toString();
        row["active"] = query.value("active").toString();
        row["created_at"] = query.value("created_at").toString();
        row["edited_at"] = query.value("edited_at").toString();

        list.append(row);
    }

    return list;
}

QMap<QString, QString> Client::getDetails(const QString& name)
{
    QMap<QString, QString> map;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT * FROM " + this->table + " WHERE name = :name;");
    query.bindValue(":name", name);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return map;
    }

    if (query.next()) {
        map["id"] = query.value("id").toString();
        map["position"] = query.value("position").toString();
        map["name"] = query.value("name").toString();
        map["email"] = query.value("email").toString();
        map["address"] = query.value("address").toString();
        map["phone"] = query.value("phone").toString();
        map["active"] = query.value("active").toString();
        map["created_at"] = query.value("created_at").toString();
        map["edited_at"] = query.value("edited_at").toString();
    }
    else {
        return map;
    }

    return map;
}

void Client::setName(const QString& name)
{
    this->name = name;
    this->updateForm();
}

void Client::remove(const int& id)
{
    // TODO:: check for positions rearranging

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE id = :id;");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

/********** PROTECTED SLOTS **********/

void Client::slotSave()
{
    // An id set to 0 means that we are creating a new client
    if (this->ui.id->text() == "0") {
        this->create(
            this->ui.position->text().trimmed(),
            this->ui.name->text().trimmed(),
            this->ui.email->toPlainText(),
            this->ui.address->text().trimmed(),
            this->ui.phone->toPlainText(),
            "1"
        );
    }
    else {
        QString active;
        if (this->ui.active->isChecked()) {
            active = "1";
        }
        else {
            active = "0";
        }

        this->update(
            this->ui.id->text(),
            this->ui.position->text().trimmed(),
            this->ui.name->text().trimmed(),
            this->ui.email->toPlainText(),
            this->ui.address->text().trimmed(),
            this->ui.phone->toPlainText(),
            active
        );
    }

    this->close();
}

void Client::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

void Client::create(
    const QString& position, 
    const QString& name, 
    const QString& emails, 
    const QString& address, 
    const QString& phones, 
    const QString& active
)
{
    // TODO:: check for positions rearranging

    QString created_at = QDate::currentDate().toString("yyyy-MM-dd");

    QString sep = ",";
    QString formatted_emails = "";
    QString formatted_phones = "";
    QStringList emails_list = emails.split(QRegularExpression("\n|\r\n|\r"));
    QStringList phones_list = phones.split(QRegularExpression("\n|\r\n|\r"));

    for (int i = 0; i < emails_list.length(); i++) {
        sep = ",";
        if (i == emails_list.length() - 1) {
            sep = ""; // No separator after the last email address
        }
        formatted_emails += emails_list[i] + sep;
    }

    for (int i = 0; i < phones_list.length(); i++) {
        sep = ",";
        if (i == phones_list.length() - 1) {
            sep = ""; // No separator after the last phone number
        }
        formatted_phones += phones_list[i] + sep;
    }

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("INSERT INTO " + this->table + ""
        " (position, name, email, address, phone, active, created_at, edited_at)"
        " VALUES (:position, :name, :emails, :address, :phones, :active, :created_at, :edited_at)");
    query.bindValue(":position", position);
    query.bindValue(":name", name);
    query.bindValue(":emails", formatted_emails);
    query.bindValue(":address", address);
    query.bindValue(":phones", formatted_phones);
    query.bindValue(":active", active);
    query.bindValue(":created_at", created_at);
    query.bindValue(":edited_at", created_at);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

void Client::update(
    const QString& id, 
    const QString& position, 
    const QString& name, 
    const QString& emails,
    const QString& address, 
    const QString& phones,
    const QString& active
)
{
    // TODO:: check for positions rearranging

    QString edited_at = QDate::currentDate().toString("yyyy-MM-dd");

    QString sep = ",";
    QString formatted_emails = "";
    QString formatted_phones = "";
    QStringList emails_list = emails.split(QRegularExpression("\n|\r\n|\r"));
    QStringList phones_list = phones.split(QRegularExpression("\n|\r\n|\r"));

    for (int i = 0; i < emails_list.length(); i++) {
        sep = ",";
        if (i == emails_list.length() - 1) {
            sep = ""; // No separator after the last email address
        }
        formatted_emails += emails_list[i] + sep;
    }

    for (int i = 0; i < phones_list.length(); i++) {
        sep = ",";
        if (i == phones_list.length() - 1) {
            sep = ""; // No separator after the last phone number
        }
        formatted_phones += phones_list[i] + sep;
    }

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("UPDATE "  + this->table + ""
        " SET position = :position, name = :name, email = :emails, address = :address," 
        " phone = :phones, active = :active, edited_at = :edited_at "
        " WHERE id = :id;");
    query.bindValue(":position", position);
    query.bindValue(":name", name);
    query.bindValue(":emails", formatted_emails);
    query.bindValue(":address", address);
    query.bindValue(":phones", formatted_phones);
    query.bindValue(":active", active);
    query.bindValue(":edited_at", edited_at);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

void Client::updateForm()
{
    QMap<QString, QString> client = this->getDetails(this->name);

    if (!client.isEmpty()) {
        QStringList emails_list = client["email"].split(u',');
        QString emails = "";
        for (int j = 0; j < emails_list.length(); j++) {
            QString nl = "\n";
            if (j == emails_list.length() - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[j] + nl;
        }

        QStringList phones_list = client["phone"].split(u',');
        QString phones = "";
        for (int j = 0; j < phones_list.length(); j++) {
            QString nl = "\n";
            if (j == phones_list.length() - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[j] + nl;
        }

        this->ui.id->setText(client["id"]);
        this->ui.position->setText(client["position"]);
        this->ui.name->setText(client["name"]);
        this->ui.email->setText(emails);
        this->ui.address->setText(client["address"]);
        this->ui.phone->setText(phones);

        this->ui.active->setChecked(client["active"] == "1");
    }
    else {
        // Client not found means we are asking to insert a new one
        
        // Reset the form fields
        this->ui.id->setText("0");
        this->ui.position->setText(QString::number(this->getLastPosition() + 1));
        this->ui.name->setText("");
        this->ui.email->setText("");
        this->ui.address->setText("");
        this->ui.phone->setText("");
        this->ui.active->setChecked(true);
    }
}

int Client::getLastPosition()
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT MAX(position) as position FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        return query.value("position").toInt();
    }

    return 0;
}
