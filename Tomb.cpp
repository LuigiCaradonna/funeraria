#include "Tomb.h"

/********** CONSTRUCTOR **********/

Tomb::Tomb(QSqlDatabase* db, QWidget* parent)
    : db(db), parent(parent)
{
}

/********** DESTRUCTOR **********/

Tomb::~Tomb()
{
}

/********** PUBLIC FUNCTIONS **********/

QList<QStringList> Tomb::get(int client_id, int year, const QString& filter)
{
    QList<QStringList> tombs;
    QSqlQuery query = QSqlQuery(*this->db);

    QString query_string = "SELECT "
        "progressive, name, price, paid, notes, accessories_mounted,"
        "ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at "
        "FROM " + this->table + " WHERE client_id = :client_id";

    if (filter.trimmed() != "") {
        query_string += " AND name LIKE '" + filter + "%'";
    }

    if (year != 0) {
        query_string += " AND ordered_at LIKE '" + QString::number(year) + "%'";
    }

    query_string += " ORDER BY ordered_at DESC";

    query.prepare(query_string);

    query.bindValue(":client_id", client_id);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }

    while (query.next()) {
        QStringList tomb{
            query.value("progressive").toString(),
            query.value("name").toString(),
            query.value("price").toString(),
            query.value("paid").toString(),
            query.value("notes").toString(),
            query.value("accessories_mounted").toString(),
            query.value("ordered_at").toString(),
            query.value("proofed_at").toString(),
            query.value("confirmed_at").toString(),
            query.value("engraved_at").toString(),
            query.value("delivered_at").toString()
        };

        tombs.append(tomb);
    }

    return tombs;
}

QMap<QString, QString> Tomb::getDetails(int progressive)
{
    QMap<QString, QString> map;
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT * FROM " + this->table + " WHERE progressive = :progressive;");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        map["progressive"] = query.value("progressive").toString();
        map["client_id"] = query.value("client_id").toString();
        map["name"] = query.value("name").toString();
        map["additional_names"] = query.value("additional_names").toString();
        map["price"] = query.value("price").toString();
        map["paid"] = query.value("paid").toString();
        map["material_id"] = query.value("material_id").toString();
        map["vase_id"] = query.value("vase_id").toString();
        map["lamp_id"] = query.value("lamp_id").toString();
        map["flame_id"] = query.value("flame_id").toString();
        map["notes"] = query.value("notes").toString();
        map["accessories_mounted"] = query.value("accessories_mounted").toString();
        map["ordered_at"] = query.value("ordered_at").toString();
        map["proofed_at"] = query.value("proofed_at").toString();
        map["confirmed_at"] = query.value("confirmed_at").toString();
        map["engraved_at"] = query.value("engraved_at").toString();
        map["delivered_at"] = query.value("delivered_at").toString();
        map["created_at"] = query.value("created_at").toString();
        map["edited_at"] = query.value("edited_at").toString();
    }

    return map;
}

bool Tomb::update(QString id, QString value)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("UPDATE " + this->table + " SET name = :name WHERE id = :id; ");
    query.bindValue(":name", value);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();

        return false;
    }

    return true;
}

void Tomb::setProgressive(int progressive)
{
    this->progressive = progressive;
    this->updateForm();
}

void Tomb::remove(const int& progressive)
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE progressive = :progressive;");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
}

/********** PRIVATE FUNCTIONS **********/

void Tomb::updateForm()
{
    // Get the selected tomb's data
    QMap<QString, QString> tomb = this->getDetails(this->progressive);

    if (!tomb.isEmpty()) {
        QStringList emails_list = tomb["email"].split(u',');
        QString emails = "";
        for (int j = 0; j < emails_list.length(); j++) {
            QString nl = "\n";
            if (j == emails_list.length() - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[j] + nl;
        }

        QStringList phones_list = tomb["phone"].split(u',');
        QString phones = "";
        for (int j = 0; j < phones_list.length(); j++) {
            QString nl = "\n";
            if (j == phones_list.length() - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[j] + nl;
        }

        // Fill the form fields with the selected tomb's data
        // this->ui.id->setText(tomb["id"]);
        // this->ui.position->setText(tomb["position"]);
        // this->ui.name->setText(tomb["name"]);
        // this->ui.email->setText(emails);
        // this->ui.address->setText(tomb["address"]);
        // this->ui.phone->setText(phones);
        // this->ui.active->setChecked(tomb["active"] == "1");

        // Set the save button text
        this->ui.btnSave->setText("Aggiorna");
    }
    else {
        // Tomb not found means we are asking to insert a new one

        // Reset the form fields
        // this->ui.id->setText("0");
        // this->ui.position->setText(QString::number(this->getLastPosition() + 1));
        // this->ui.name->setText("");
        // this->ui.email->setText("");
        // this->ui.address->setText("");
        // this->ui.phone->setText("");
        // this->ui.active->setChecked(true);

        // Set the save button text
        this->ui.btnSave->setText("Crea");
    }
}
