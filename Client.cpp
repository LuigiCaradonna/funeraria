#include "Client.h"

Client::Client(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
	this->ui.setupUi(this);

    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &Client::slotUpdate);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &Client::slotCloseDialog);
}

Client::~Client()
{}

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

        return 0;
    }

    if (query.next()) {
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
        QMessageBox message;
        message.setIcon(QMessageBox::Warning);
        message.setText("Nessun record trovato");
        message.exec();

        return map;
    }

    return map;
}

void Client::setName(const QString& name)
{
    this->name = name;
    this->updateForm();
}

void Client::update(
    const QString& id, 
    const QString& position, 
    const QString& name, 
    const QString& email, 
    const QString& address, 
    const QString& phone, 
    const QString& active
)
{
    QString edited_at = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("UPDATE "  + this->table + ""
        " SET position = :position, name = :name, email = :emails, address = :address," 
        " phone = :phones, active = :active, edited_at = :edited_at "
        " WHERE id = :id;");
    query.bindValue(":position", position);
    query.bindValue(":name", name);
    query.bindValue(":emails", email);
    query.bindValue(":address", address);
    query.bindValue(":phones", phone);
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
}

void Client::slotUpdate()
{
    QString sep = ",";
    QString active;
    QString emails = "";
    QString phones = "";
    QStringList emails_list = this->ui.email->toPlainText().split(QRegularExpression("\n|\r\n|\r"));
    QStringList phones_list = this->ui.phone->toPlainText().split(QRegularExpression("\n|\r\n|\r"));

    qDebug() << this->ui.email->toPlainText();
    qDebug() << this->ui.phone->toPlainText();
    qDebug() << phones_list.length();

    if (this->ui.active->isChecked()) {
        active = "1";
    }
    else {
        active = "0";
    }

    for (int i = 0; i < emails_list.length(); i++) {
        sep = ",";
        if (i == emails_list.length() - 1) {
            sep = ""; // No separator after the last email address
        }
        emails += emails_list[i] + sep;
    }

    for (int i = 0; i < phones_list.length(); i++) {
        sep = ",";
        if (i == phones_list.length() - 1) {
            sep = ""; // No separator after the last phone number
        }
        phones += phones_list[i] + sep;
    }

    qDebug() << phones;

    this->update(
        this->ui.id->text(),
        this->ui.position->text().trimmed(),
        this->ui.name->text().trimmed(),
        emails,
        this->ui.address->text().trimmed(),
        phones,
        active
    );

    this->close();
}

void Client::slotCloseDialog()
{
    this->close();
}