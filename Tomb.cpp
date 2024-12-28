#include "Tomb.h"

/********** CONSTRUCTOR **********/

Tomb::Tomb(const QSqlDatabase& db)
    : db(db)
{
    this->settings = new Settings(db);
}

/********** DESTRUCTOR **********/

Tomb::~Tomb()
{
    delete this->settings;
}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Tomb::getList(
    const int client_id, 
    const int year,
    bool engraved,
    QString filter,
    QString sort_by,
    QString sort_direction
)
{
    QList<QMap<QString, QString>> list{};
    QSqlQuery query = QSqlQuery(this->db);

    QString query_string = "SELECT * FROM " + this->table + " WHERE 1 = 1";

    if (engraved == true) {
        query_string += " AND engraved = 1";
    }

    if (client_id > 0) {
        query_string += " AND client_id = " + QString::number(client_id) ;
    }

    if (filter.trimmed() != "") {
        query_string += " AND name LIKE \"%" + filter + "%\"";
    }

    if (year != 0) {
        query_string += " AND ordered_at LIKE '" + QString::number(year) + "%'";
    }

    query_string += " ORDER BY " + sort_by + " " + sort_direction;

    query.prepare(query_string);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return list;
    }

    while (query.next()) {
        QMap<QString, QString> tomb;

        QString accessories_mounted = "0";

        // Accessories should result to be mounted also if the field is set to 0, but there were no accessories to be mounted
        if (query.value("accessories_mounted").toString() == "1" || 
            (query.value("vase_code").toString() == "NO" &&
            query.value("lamp_code").toString() == "NO" &&
            query.value("flame_code").toString() == "NO"
            )) 
        {
            accessories_mounted = "1";
        }

        tomb["progressive"] = QString::number(query.value("progressive").toInt());
        tomb["client_id"] = query.value("client_id").toString();
        tomb["name"] = query.value("name").toString();
        tomb["engraved_names"] = query.value("engraved_names").toString();
        tomb["ep_amount"] = query.value("ep_amount").toString();
        tomb["engraved"] = query.value("engraved").toString();
        tomb["price"] = query.value("price").toString();
        tomb["paid"] = query.value("paid").toString();
        tomb["material_code"] = query.value("material_code").toString();
        tomb["type_code"] = query.value("type_code").toString();
        tomb["format_code"] = query.value("format_code").toString();
        tomb["vase_code"] = query.value("vase_code").toString();
        tomb["lamp_code"] = query.value("lamp_code").toString();
        tomb["flame_code"] = query.value("flame_code").toString();
        tomb["cross_code"] = query.value("cross_code").toString();
        tomb["sacred_code"] = query.value("sacred_code").toString();
        tomb["sculpture_code"] = query.value("sculpture_code").toString();
        tomb["sculpture_h"] = query.value("sculpture_h").toString();
        tomb["mounted"] = query.value("mounted").toString();
        tomb["ep_relief"] = query.value("ep_relief").toString();
        tomb["inscription"] = query.value("inscription").toString();
        tomb["pit_format_one"] = query.value("pit_format_one").toString();
        tomb["pit_type_one"] = query.value("pit_type_one").toString();
        tomb["pit_format_two"] = query.value("pit_format_two").toString();
        tomb["pit_type_two"] = query.value("pit_type_two").toString();
        tomb["pit_format_three"] = query.value("pit_format_three").toString();
        tomb["pit_type_three"] = query.value("pit_type_three").toString();
        tomb["pit_format_four"] = query.value("pit_format_four").toString();
        tomb["pit_type_four"] = query.value("pit_type_four").toString();
        tomb["pit_format_five"] = query.value("pit_format_five").toString();
        tomb["pit_type_five"] = query.value("pit_type_five").toString();
        tomb["pit_format_six"] = query.value("pit_format_six").toString();
        tomb["pit_type_six"] = query.value("pit_type_six").toString();
        tomb["notes"] = query.value("notes").toString();
        tomb["accessories_mounted"] = accessories_mounted;
        tomb["ordered_at"] = query.value("ordered_at").toString();
        tomb["proofed_at"] = query.value("proofed_at").toString();
        tomb["confirmed_at"] = query.value("confirmed_at").toString();
        tomb["engraved_at"] = query.value("engraved_at").toString();
        tomb["delivered_at"] = query.value("delivered_at").toString();
        tomb["created_at"] = query.value("created_at").toString();
        tomb["edited_at"] = query.value("edited_at").toString();

        list.append(tomb);
    }

    return list;
}

QMap<QString, QString> Tomb::getByProgressive(const int progressive)
{
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);

    query.prepare("SELECT * FROM " + this->table + " WHERE progressive = :progressive");

    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return tomb;
    }

    while (query.next()) {
        QString accessories_mounted = "0";

        // Accessories should result to be mounted also if the field is set to 0, but there were no accessories to be mounted
        if (query.value("accessories_mounted").toString() == "1" ||
            (query.value("vase_code").toString() == "NO" &&
                query.value("lamp_code").toString() == "NO" &&
                query.value("flame_code").toString() == "NO"
                )) {
            accessories_mounted = "1";
        }

        tomb["progressive"] = QString::number(query.value("progressive").toInt());
        tomb["client_id"] = query.value("client_id").toString();
        tomb["name"] = query.value("name").toString();
        tomb["engraved_names"] = query.value("engraved_names").toString();
        tomb["ep_amount"] = query.value("ep_amount").toString();
        tomb["engraved"] = query.value("engraved").toString();
        tomb["price"] = query.value("price").toString();
        tomb["paid"] = query.value("paid").toString();
        tomb["material_code"] = query.value("material_code").toString();
        tomb["type_code"] = query.value("type_code").toString();
        tomb["format_code"] = query.value("format_code").toString();
        tomb["vase_code"] = query.value("vase_code").toString();
        tomb["lamp_code"] = query.value("lamp_code").toString();
        tomb["flame_code"] = query.value("flame_code").toString();
        tomb["cross_code"] = query.value("cross_code").toString();
        tomb["sacred_code"] = query.value("sacred_code").toString();
        tomb["sculpture_code"] = query.value("sculpture_code").toString();
        tomb["sculpture_h"] = query.value("sculpture_h").toString();
        tomb["mounted"] = query.value("mounted").toString();
        tomb["ep_relief"] = query.value("ep_relief").toString();
        tomb["inscription"] = query.value("inscription").toString();
        tomb["pit_format_one"] = query.value("pit_format_one").toString();
        tomb["pit_type_one"] = query.value("pit_type_one").toString();
        tomb["pit_format_two"] = query.value("pit_format_two").toString();
        tomb["pit_type_two"] = query.value("pit_type_two").toString();
        tomb["pit_format_three"] = query.value("pit_format_three").toString();
        tomb["pit_type_three"] = query.value("pit_type_three").toString();
        tomb["pit_format_four"] = query.value("pit_format_four").toString();
        tomb["pit_type_four"] = query.value("pit_type_four").toString();
        tomb["pit_format_five"] = query.value("pit_format_five").toString();
        tomb["pit_type_five"] = query.value("pit_type_five").toString();
        tomb["pit_format_six"] = query.value("pit_format_six").toString();
        tomb["pit_type_six"] = query.value("pit_type_six").toString();
        tomb["notes"] = query.value("notes").toString();
        tomb["accessories_mounted"] = accessories_mounted;
        tomb["ordered_at"] = query.value("ordered_at").toString();
        tomb["proofed_at"] = query.value("proofed_at").toString();
        tomb["confirmed_at"] = query.value("confirmed_at").toString();
        tomb["engraved_at"] = query.value("engraved_at").toString();
        tomb["delivered_at"] = query.value("delivered_at").toString();
        tomb["created_at"] = query.value("created_at").toString();
        tomb["edited_at"] = query.value("edited_at").toString();
    }

    return tomb;
}

QMap<QString, QString> Tomb::getDetails(const int progressive)
{
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);

    query.prepare("SELECT "
        "tomb.progressive, tomb.client_id, client.name AS client_name, tomb.name, tomb.engraved_names,tomb.ep_amount, "
        "tomb.engraved, tomb.price, tomb.paid, tomb.material_code, material.name AS material_name, "
        "tomb.type_code, tomb_type.name AS type_name, tomb.format_code, tomb_format.name AS format_name, "
        "tomb.vase_code, vase.name AS vase_name, tomb.lamp_code, lamp.name AS lamp_name, tomb.flame_code, flame.name AS flame_name, "
        "tomb.cross_code, cross.name AS cross_name, tomb.sacred_code, sacred.name AS sacred_name, "
        "tomb.sculpture_code, sculpture.name AS sculpture_name, tomb.sculpture_h, tomb.mounted, tomb.ep_relief, tomb.inscription, "
        "tomb.pit_format_one, tomb.pit_type_one, "
        "tomb.pit_format_two, tomb.pit_type_two, "
        "tomb.pit_format_three, tomb.pit_type_three, "
        "tomb.pit_format_four, tomb.pit_type_four, "
        "tomb.pit_format_five, tomb.pit_type_five, "
        "tomb.pit_format_six, tomb.pit_type_six, "
        "tomb.notes, tomb.accessories_mounted, "
        "tomb.ordered_at, tomb.proofed_at, tomb.confirmed_at, tomb.engraved_at, "
        "tomb.delivered_at, tomb.created_at, tomb.edited_at "
        "FROM " + this->table + " "
        "JOIN client ON tomb.client_id = client.id "
        "JOIN material ON tomb.material_code = material.code "
        "JOIN vase ON tomb.vase_code = vase.code "
        "JOIN lamp ON tomb.lamp_code = lamp.code "
        "JOIN flame ON tomb.flame_code = flame.code "
        "JOIN cross ON tomb.cross_code = cross.code "
        "JOIN sacred ON tomb.sacred_code = sacred.code "
        "JOIN sculpture ON tomb.sculpture_code = sculpture.code "
        "JOIN tomb_type ON tomb.type_code = tomb_type.code "
        "JOIN tomb_format ON tomb.format_code = tomb_format.code "
        "WHERE tomb.progressive = :progressive;");

    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        tomb["progressive"] = QString::number(query.value("progressive").toInt());
        tomb["client_id"] = query.value("client_id").toString();
        tomb["client_name"] = query.value("client_name").toString();
        tomb["engraved_names"] = query.value("engraved_names").toString();
        tomb["ep_amount"] = query.value("ep_amount").toString();
        tomb["engraved"] = query.value("engraved").toString();
        tomb["price"] = query.value("price").toString();
        tomb["paid"] = query.value("paid").toString();
        tomb["material_code"] = query.value("material_code").toString();
        tomb["type_code"] = query.value("type_code").toString();
        tomb["format_code"] = query.value("format_code").toString();
        tomb["vase_code"] = query.value("vase_code").toString();
        tomb["lamp_code"] = query.value("lamp_code").toString();
        tomb["flame_code"] = query.value("flame_code").toString();
        tomb["cross_code"] = query.value("cross_code").toString();
        tomb["sacred_code"] = query.value("sacred_code").toString();
        tomb["sculpture_code"] = query.value("sculpture_code").toString();
        tomb["sculpture_h"] = query.value("sculpture_h").toString();
        tomb["mounted"] = query.value("mounted").toString();
        tomb["ep_relief"] = query.value("ep_relief").toString();
        tomb["inscription"] = query.value("inscription").toString();
        tomb["pit_format_one"] = query.value("pit_format_one").toString();
        tomb["pit_type_one"] = query.value("pit_type_one").toString();
        tomb["pit_format_two"] = query.value("pit_format_two").toString();
        tomb["pit_type_two"] = query.value("pit_type_two").toString();
        tomb["pit_format_three"] = query.value("pit_format_three").toString();
        tomb["pit_type_three"] = query.value("pit_type_three").toString();
        tomb["pit_format_four"] = query.value("pit_format_four").toString();
        tomb["pit_type_four"] = query.value("pit_type_four").toString();
        tomb["pit_format_five"] = query.value("pit_format_five").toString();
        tomb["pit_type_five"] = query.value("pit_type_five").toString();
        tomb["pit_format_six"] = query.value("pit_format_six").toString();
        tomb["pit_type_six"] = query.value("pit_type_six").toString();
        tomb["notes"] = query.value("notes").toString();
        tomb["accessories_mounted"] = query.value("accessories_mounted").toString();
        tomb["ordered_at"] = query.value("ordered_at").toString();
        tomb["proofed_at"] = query.value("proofed_at").toString();
        tomb["confirmed_at"] = query.value("confirmed_at").toString();
        tomb["engraved_at"] = query.value("engraved_at").toString();
        tomb["delivered_at"] = query.value("delivered_at").toString();
        tomb["created_at"] = query.value("created_at").toString();
        tomb["edited_at"] = query.value("edited_at").toString();
    }

    return tomb;
}

QList<QMap<QString, QString>> Tomb::tombsToEngrave()
{
    QList<QMap<QString, QString>> tombs;
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT tomb.progressive AS progressive, tomb.name AS name, "
        "tomb.confirmed_at AS confirmed_at, client.name AS client_name, material.name AS material "
        "FROM " + this->table + " "
        "JOIN client ON tomb.client_id = client.id "
        "JOIN material ON tomb.material_code = material.code "
        "WHERE (tomb.engraved = 1 AND tomb.confirmed_at != '' AND tomb.confirmed_at IS NOT NULL) AND "
        "(tomb.engraved_at == '' OR tomb.engraved_at IS NULL)"
        "ORDER BY confirmed_at ASC;"
    );

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else {
        while (query.next()) {
            tomb["progressive"] = query.value("progressive").toString();
            tomb["deceased"] = query.value("name").toString();
            tomb["confirmed_at"] = query.value("confirmed_at").toString();
            tomb["material"] = query.value("material").toString();
            tomb["client"] = query.value("client_name").toString();
            tombs.append(tomb);
        }
    }

    return tombs;
}

QList<QMap<QString, QString>> Tomb::accessoriesToMount()
{
    QList<QMap<QString, QString>> accessories;
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT tomb.progressive AS progressive, tomb.name AS name, "
        "client.name AS client_name, vase.name AS vase_name, "
        "lamp.name AS lamp_name, flame.name AS flame_name, material.name AS material_name "
        "FROM " + this->table + " "
        "JOIN material ON tomb.material_code = material.code "
        "JOIN vase ON tomb.vase_code = vase.code "
        "JOIN lamp ON tomb.lamp_code = lamp.code "
        "JOIN flame ON tomb.flame_code = flame.code "
        "JOIN client ON tomb.client_id = client.id "
        "WHERE tomb.accessories_accessories_mounted = 0 AND (tomb.confirmed_at != '' AND tomb.confirmed_at IS NOT NULL) "
        "AND (tomb.vase_code != 'NV' OR tomb.lamp_code != 'NL' OR tomb.flame_code != 'NF');"
    );

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else {
        while (query.next()) {
            tomb["progressive"] = query.value("progressive").toString();
            tomb["deceased"] = query.value("name").toString();
            tomb["material"] = query.value("material_name").toString();
            tomb["vase"] = query.value("vase_name").toString();
            tomb["lamp"] = query.value("lamp_name").toString();
            tomb["flame"] = query.value("flame_name").toString();
            tomb["client"] = query.value("client_name").toString();
            accessories.append(tomb);
        }
    }

    return accessories;
}

QList<QMap<QString, QString>> Tomb::tombsToPay()
{
    QList<QMap<QString, QString>> tombs;
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT tomb.progressive AS progressive, tomb.name AS name, tomb.price AS price, "
        "client.name AS client_name "
        "FROM " + this->table + " "
        "JOIN client ON tomb.client_id = client.id "
        "WHERE tomb.delivered_at != '' AND tomb.delivered_at IS NOT NULL AND tomb.paid = 0 AND tomb.price != 0;"
    );

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else {
        while (query.next()) {
            tomb["progressive"] = query.value("progressive").toString();
            tomb["deceased"] = query.value("name").toString();
            tomb["price"] = query.value("price").toString();
            tomb["client"] = query.value("client_name").toString();
            tombs.append(tomb);
        }
    }

    return tombs;
}

/********** PRIVATE FUNCTIONS **********/

bool Tomb::store(
    const int progressive,
    const int client_id,
    const QString& name,
    const QString& engraved_names,
    const int ep_amount,
    const int engraved,
    const double& price,
    const bool paid,
    const QString& material_code,
    const QString& tomb_type,
    const QString& tomb_format,
    const QString& vase_code,
    const QString& lamp_code,
    const QString& flame_code,
    const QString& cross_code,
    const QString& sacred_code,
    const QString& sculpture_code,
    const float& sculpture_height,
    const bool mounted,
    const bool ep_relief,
    const bool inscription,
    const QString& pit_format_one,
    const QString& pit_type_one,
    const QString& pit_format_two,
    const QString& pit_type_two,
    const QString& pit_format_three,
    const QString& pit_type_three,
    const QString& pit_format_four,
    const QString& pit_type_four,
    const QString& pit_format_five,
    const QString& pit_type_five,
    const QString& pit_format_six,
    const QString& pit_type_six,
    const QString& notes,
    const bool accessories_mounted,
    const QString& ordered_at,
    const QString& proofed_at,
    const QString& confirmed_at,
    const QString& engraved_at,
    const QString& delivered_at
)
{
    // Create the folder into the archive

    Config* config = new Config();

    QString path_to_archive = config->getArchivePath();

    delete config;

    if (path_to_archive == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile recuperare la cartella dell'archivio.\n"
                "La procedura di inserimento nel database continuerà regolarmente.");
        message.exec();
    }
    else {
        // e.g.: archive/2400 - 2499/2465 - lastname firstname
        QString full_path = path_to_archive + "/" + this->getGroupingFolder(progressive) + "/" + QString::number(progressive) + " - " + name.trimmed();

        QDir dir(full_path);

        if (!dir.exists()) {
            if (!dir.mkpath(full_path)) {
                QMessageBox message;
                message.setWindowTitle("Funeraria");
                message.setIcon(QMessageBox::Warning);
                message.setText("Non è stato possibile creare la cartella relativa a questa lapide.\n"
                    "La procedura di inserimento nel database continuerà regolarmente.");
                message.exec();
            }
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("La cartella relativa a questa lapide è già presente.\n"
                "La procedura di inserimento nel database continuerà regolarmente.");
            message.exec();
        }
    }

    // Store the tomb into the database

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "INSERT INTO " + this->table + " "
        "(progressive, client_id, name, engraved_names, ep_amount, engraved, price, paid, material_code, type_code, format_code, "
        "vase_code, lamp_code, flame_code, cross_code, sacred_code, sculpture_code, sculpture_h, mounted, ep_relief, inscription, "
        "pit_format_one, pit_type_one, pit_format_two, pit_type_two, pit_format_three, pit_type_three, "
        "pit_format_four, pit_type_four, pit_format_five, pit_type_five, pit_format_six, pit_type_six, "
        "notes, accessories_mounted, ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at, "
        "created_at, edited_at) "
        "VALUES (:progressive, :client_id, :name, :engraved_names, :ep_amount, :engraved, :price, :paid, :material_code, :type_code, :format_code, "
        ":vase_code, :lamp_code, :flame_code, cross_code, :sacred_code, :sculpture_code, :sculpture_h, :mounted, :ep_relief, :inscription, "
        ":pit_format_one, :pit_type_one, :pit_format_two, :pit_type_two, :pit_format_three, :pit_type_three, "
        ":pit_format_four, :pit_type_four, :pit_format_five, :pit_type_five, :pit_format_six, :pit_type_six, "
        ":notes, : accessories_mounted, : ordered_at, : proofed_at, : confirmed_at, : engraved_at, : delivered_at, "
        ":created_at, :edited_at)"
    );
    query.bindValue(":progressive", progressive);
    query.bindValue(":client_id", client_id);
    query.bindValue(":name", name.trimmed());
    query.bindValue(":engraved_names", (engraved_names.trimmed() == "") ? name.trimmed() : engraved_names.trimmed());
    query.bindValue(":ep_amount", ep_amount);
    query.bindValue(":engraved", engraved);
    query.bindValue(":price", price);
    query.bindValue(":paid", paid);
    query.bindValue(":material_code", material_code);
    query.bindValue(":type_code", tomb_type);
    query.bindValue(":format_code", tomb_format);
    query.bindValue(":vase_code", vase_code);
    query.bindValue(":lamp_code", lamp_code);
    query.bindValue(":flame_code", flame_code);
    query.bindValue(":cross_code", cross_code);
    query.bindValue(":sacred_code", sacred_code);
    query.bindValue(":sculpture_code", sculpture_code);
    query.bindValue(":sculpture_h", sculpture_height);
    query.bindValue(":mounted", mounted);
    query.bindValue(":ep_relief", ep_relief);
    query.bindValue(":inscription", inscription);
    query.bindValue(":pit_format_one", pit_format_one);
    query.bindValue(":pit_type_one", pit_type_one);
    query.bindValue(":pit_format_two", pit_format_two);
    query.bindValue(":pit_type_two", pit_type_two);
    query.bindValue(":pit_format_three", pit_format_three);
    query.bindValue(":pit_type_three", pit_type_three);
    query.bindValue(":pit_format_four", pit_format_four);
    query.bindValue(":pit_type_four", pit_type_four);
    query.bindValue(":pit_format_five", pit_format_five);
    query.bindValue(":pit_type_five", pit_type_five);
    query.bindValue(":pit_format_six", pit_format_six);
    query.bindValue(":pit_type_six", pit_type_six);
    query.bindValue(":notes", notes.trimmed());
    query.bindValue(":accessories_mounted", accessories_mounted);
    query.bindValue(":ordered_at", QDate::fromString(ordered_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    query.bindValue(":proofed_at", QDate::fromString(proofed_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    query.bindValue(":confirmed_at", QDate::fromString(confirmed_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));

    if (engraved_at.trimmed() == this->not_engraved) {
        query.bindValue(":engraved_at", this->not_engraved);
    }
    else {
        query.bindValue(":engraved_at", QDate::fromString(engraved_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    }

    if (delivered_at.trimmed() == "Consegnata") {
        query.bindValue(":delivered_at", "Consegnata");
    }
    else {
        query.bindValue(":delivered_at", QDate::fromString(delivered_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    }

    query.bindValue(":created_at", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":edited_at", QDate::currentDate().toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return false;
    }

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Lapide creata");
    message.exec();

    return true;
}

bool Tomb::update(
    const int old_progressive,
    const int progressive,
    const int client_id,
    const QString& name,
    const QString& engraved_names,
    const int ep_amount,
    const int engraved,
    const double& price,
    const bool paid,
    const QString& material_code,
    const QString& tomb_type,
    const QString& tomb_format,
    const QString& vase_code,
    const QString& lamp_code,
    const QString& flame_code,
    const QString& cross_code,
    const QString& sacred_code,
    const QString& sculpture_code,
    const float& sculpture_height,
    const bool mounted,
    const bool ep_relief,
    const bool inscription,
    const QString& pit_format_one,
    const QString& pit_type_one,
    const QString& pit_format_two,
    const QString& pit_type_two,
    const QString& pit_format_three,
    const QString& pit_type_three,
    const QString& pit_format_four,
    const QString& pit_type_four,
    const QString& pit_format_five,
    const QString& pit_type_five,
    const QString& pit_format_six,
    const QString& pit_type_six,
    const QString& notes,
    const bool accessories_mounted,
    const QString& ordered_at,
    const QString& proofed_at,
    const QString& confirmed_at,
    const QString& engraved_at,
    const QString& delivered_at
)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET progressive = :progressive, client_id = :client_id, name = :name, engraved_names = :engraved_names, "
        "ep_amount = :ep_amount, engraved = :engraved, price = :price, paid = :paid, material_code = :material_code, "
        "type_code = :type_code, format_code = :format_code, vase_code = :vase_code, lamp_code = :lamp_code,  flame_code = :flame_code, "
        "cross_code = :cross_code, sacred_code = :sacred_code,  sculpture_code = :sculpture_code, sculpture_h = :sculpture_h, "
        "accessories_mounted = :mounted, ep_relief = :ep_relief, inscription = :inscription, "
        "pit_format_one = :pit_format_one, pit_type_one = :pit_type_one, pit_format_two = :pit_format_two, pit_type_two = :pit_type_two, "
        "pit_format_three = :pit_format_three, pit_type_three = :pit_type_three, pit_format_four = :pit_format_four, pit_type_four = :pit_type_four, "
        "pit_format_five = :pit_format_five, pit_type_five = :pit_type_five, pit_format_six = :pit_format_six, pit_type_six = :pit_type_six, "
        "notes = :notes, accessories_accessories_mounted = :accessories_mounted, "
        "ordered_at = :ordered_at, proofed_at = :proofed_at, confirmed_at = :confirmed_at, engraved_at = :engraved_at, "
        "delivered_at = :delivered_at, edited_at = :edited_at "
        "WHERE progressive = :old_progressive;"
    );
    query.bindValue(":progressive", progressive);
    query.bindValue(":client_id", client_id);
    query.bindValue(":name", name.trimmed());
    query.bindValue(":engraved_names", engraved_names.trimmed());
    query.bindValue(":ep_amount", ep_amount);
    query.bindValue(":engraved", engraved);
    query.bindValue(":price", price);
    query.bindValue(":paid", paid);
    query.bindValue(":material_code", material_code);
    query.bindValue(":type_code", tomb_type);
    query.bindValue(":format_code", tomb_format);
    query.bindValue(":vase_code", vase_code);
    query.bindValue(":lamp_code", lamp_code);
    query.bindValue(":flame_code", flame_code);
    query.bindValue(":cross_code", cross_code);
    query.bindValue(":sacred_code", sacred_code);
    query.bindValue(":sculpture_code", sculpture_code);
    query.bindValue(":sculpture_h", sculpture_height);
    query.bindValue(":mounted", mounted);
    query.bindValue(":ep_relief", ep_relief);
    query.bindValue(":inscription", inscription);
    query.bindValue(":pit_format_one", pit_format_one);
    query.bindValue(":pit_type_one", pit_type_one);
    query.bindValue(":pit_format_two", pit_format_two);
    query.bindValue(":pit_type_two", pit_type_two);
    query.bindValue(":pit_format_three", pit_format_three);
    query.bindValue(":pit_type_three", pit_type_three);
    query.bindValue(":pit_format_four", pit_format_four);
    query.bindValue(":pit_type_four", pit_type_four);
    query.bindValue(":pit_format_five", pit_format_five);
    query.bindValue(":pit_type_five", pit_type_five);
    query.bindValue(":pit_format_six", pit_format_six);
    query.bindValue(":pit_type_six", pit_type_six);
    query.bindValue(":notes", notes.trimmed());
    query.bindValue(":accessories_mounted", accessories_mounted);
    query.bindValue(":ordered_at", QDate::fromString(ordered_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    query.bindValue(":proofed_at", QDate::fromString(proofed_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    query.bindValue(":confirmed_at", QDate::fromString(confirmed_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));

    if (engraved_at.trimmed() == this->not_engraved) {
        query.bindValue(":engraved_at", this->not_engraved);
    }
    else {
        query.bindValue(":engraved_at", QDate::fromString(engraved_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    }

    if (delivered_at.trimmed() == "Consegnata") {
        query.bindValue(":delivered_at", "Consegnata");
    }
    else {
        query.bindValue(":delivered_at", QDate::fromString(delivered_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    }
    
    query.bindValue(":edited_at", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":old_progressive", old_progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return false;
    }

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Dati aggiornati");
    message.exec();
    return true;
}

void Tomb::remove(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("DELETE FROM " + this->table + " WHERE progressive = :progressive;");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Lapide eliminata");
        message.exec();
    }
}

bool Tomb::setConfirmed(const int progressive)
{
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET confirmed_at = '" + today + "' "
        "WHERE progressive = :progressive;"
    );

    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Tomb::setEngraved(const int progressive)
{
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET engraved_at = '" + today + "' "
        "WHERE progressive = :progressive;"
    );
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Tomb::setDelivered(const int progressive)
{
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET delivered_at = '" + today + "' "
        "WHERE progressive = :progressive;"
    );
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Tomb::setPaid(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET paid = 1 "
        "WHERE progressive = :progressive;"
    );
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool Tomb::setAccessoriesMounted(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET accessories_accessories_mounted = 1 "
        "WHERE progressive = :progressive;"
    );
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        return false;
    }

    return true;
}

int Tomb::getLastProgresive() {
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT MAX(progressive) as progressive FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else if (query.next()) {
        return query.value("progressive").toInt();
    }

    return 0;
}

QStringList Tomb::getNotInUseProgressives()
{
    QList<int> progressives;
    QStringList missing;
    int last_progressive = this->getLastProgresive();

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT progressive FROM " + this->table);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
    }
    else {
        while (query.next()) {
            progressives.append(query.value("progressive").toInt());
        }

        int i = 0;
        int length = progressives.length();

        for (int expected = 1; expected <= length; expected++) {
            // All array entries checked
            if (i >= length) break;

            if (progressives[i] == expected) {
                // Matched i'th, move on to next
                i += 1;
            }
            else {
                // Missing progressive found

                // If it is not too old compared to the current last progressive
                if (expected > last_progressive - 300) {
                    // Add the missing progressive to the list
                    missing.append(QString::number(expected));
                }
            }
        }
    }

    return missing;
}

bool Tomb::isProgressiveInUse(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT progressive FROM " + this->table + " WHERE progressive = :progressive");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return true;
    }
    else if (query.next()) {
        return true;
    }

    return false;
}

bool Tomb::isConfirmed(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT confirmed_at FROM " + this->table + " WHERE progressive = :progressive");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return true;
    }
    else if (query.next()) {
        return query.value("confirmed_at").toString().trimmed() != "";
    }

    return false;
}

bool Tomb::isEngraved(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT engraved_at FROM " + this->table + " WHERE progressive = :progressive");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return true;
    }
    else if (query.next()) {
        return query.value("engraved_at").toString().trimmed() != "";
    }

    return false;
}

bool Tomb::isDelivered(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT delivered_at FROM " + this->table + " WHERE progressive = :progressive");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return true;
    }
    else if (query.next()) {
        return query.value("delivered_at").toString().trimmed() != "";
    }

    return false;
}

bool Tomb::isPaied(const int progressive)
{
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT price, paid FROM " + this->table + " WHERE progressive = :progressive");
    query.bindValue(":progressive", progressive);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return true;
    }
    else if (query.next()) {
        if (query.value("price").toString().trimmed() == "0" || query.value("paid").toString().trimmed() == "1") {
            return true;
        }

        return false;
    }

    return false;
}

QString Tomb::getGroupingFolder(const int progressive)
{
    // last 2 digit of the progressive number
    int last = QString::number(progressive).last(2).toInt();

    // e.g.: prog_int = 2446, last = 46, start = 2446 - 46 = 2400
    int start = progressive - last;
    // e.g.: end = 2400 + 99 = 2499
    int end = start + 99;

    // e.g.: "2400-2499"
    return QString::number(start) + " - " + QString::number(end);
}

QString Tomb::getFolderPath(const int progressive, const QString& name)
{
    Config* config = new Config();

    QString path_to_archive = config->getArchivePath();

    delete config;

    return "file:///" + path_to_archive + "/" + 
        this->getGroupingFolder(progressive) + "/" + 
        QString::number(progressive) + " - " + name.trimmed();
}

QList<QMap<QString, QString>> Tomb::getReport(
    const int client_id,
    const int year,
    bool engraved,
    QString group,
    bool by_client
)
{
    QList<QMap<QString, QString>> report{};
    QSqlQuery query = QSqlQuery(this->db);

    QString query_string = "SELECT client_id, count(name) AS amount ";

    if (group == "year") {
        query_string += ", strftime('%Y', ordered_at) AS year ";
    }
    else if (group == "month") {
        query_string += ", strftime('%m', ordered_at) AS month ";
    }

    query_string += " FROM " + this->table + " WHERE 1 = 1";

    if (client_id > 0) {
        query_string += " AND client_id = " + QString::number(client_id);
    }

    if (engraved == true) {
        query_string += " AND engraved = 1";
    }

    if (year != 0) {
        query_string += " AND ordered_at LIKE '" + QString::number(year) + "%'";
    }

    // Only engraved tomb to have only those actually made
    query_string += " AND engraved_at != ''";

    if (by_client && group == "year") {
        query_string += " GROUP BY client_id, year";
    }
    else if (by_client && group == "month") {
        query_string += " GROUP BY client_id, month";
    }
    else if (by_client) {
        query_string += " GROUP BY client_id";
    }
    else if (group == "year") {
        query_string += " GROUP BY year";
    }
    else if (group == "month") {
        query_string += " GROUP BY month";
    }

    query.prepare(query_string);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return report;
    }

    while (query.next()) {
        QMap<QString, QString> orders;
        orders["client_id"] = query.value("client_id").toString();
        orders["amount"] = QString::number(query.value("amount").toInt());

        if (group == "year") {
            orders["year"] = QString::number(query.value("year").toInt());
        }
        else if (group == "month") {
            // Convert the month number to its name before to add it to the array
            orders["month"] = Helpers::monthNumberToName(query.value("month").toInt());
        }

        report.append(orders);
    }

    return report;
}

QList<QMap<QString, QString>> Tomb::getGeneralTrend()
{
    QList<QMap<QString, QString>> trend{};
    QSqlQuery query = QSqlQuery(this->db);

    QString query_string = "SELECT strftime('%Y', ordered_at) AS year, count(name) AS amount ";

    query_string += " FROM " + this->table + " WHERE 1 = 1";

    // Only delivered tomb to heve only those actually made
    query_string += " AND delivered_at != ''";

    query_string += " GROUP BY year";

    query.prepare(query_string);

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return trend;
    }

    while (query.next()) {
        QMap<QString, QString> years;
        years["year"] = query.value("year").toString();
        years["amount"] = QString::number(query.value("amount").toInt());

        trend.append(years);
    }

    return trend;
}
