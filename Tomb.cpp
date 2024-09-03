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
    const int& client_id, 
    const int& year,
    bool engraved,
    QString filter,
    QString sort_by,
    QString sort_direction
)
{
    QList<QMap<QString, QString>> list;
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

        QString mounted = "0";

        // Accessories should result to be mounted also if the field is set to 0, but there were no accessories to be mounted
        if (query.value("accessories_mounted").toString() == "1" || 
            (query.value("vase_code").toString() == "NV" &&
            query.value("lamp_code").toString() == "NL" &&
            query.value("flame_code").toString() == "NF"
            )) {
            mounted = "1";
        }

        tomb["progressive"] = QString::number(query.value("progressive").toInt());
        tomb["client_id"] = query.value("client_id").toString();
        tomb["name"] = query.value("name").toString();
        tomb["material"] = query.value("material_code").toString();
        tomb["engraved_names"] = query.value("engraved_names").toString();
        tomb["engraved"] = query.value("engraved").toString();
        tomb["price"] = query.value("price").toString();
        tomb["paid"] = query.value("paid").toString();
        tomb["material_code"] = query.value("material_code").toString();
        tomb["vase_code"] = query.value("vase_code").toString();
        tomb["lamp_code"] = query.value("lamp_code").toString();
        tomb["flame_code"] = query.value("flame_code").toString();
        tomb["notes"] = query.value("notes").toString();
        tomb["accessories_mounted"] = mounted;
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

QMap<QString, QString> Tomb::getDetails(const int& progressive)
{
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT "
        "tombs.progressive, tombs.client_id, clients.name AS client_name, tombs.name, tombs.engraved_names,"
        "tombs.engraved, tombs.price, tombs.paid, tombs.material_code, materials.name AS material_name, tombs.vase_code, "
        "vases.name AS vase_name, tombs.lamp_code, lamps.name AS lamp_name, tombs.flame_code, "
        "flames.name AS flame_name, tombs.notes, tombs.accessories_mounted,"
        "tombs.ordered_at, tombs.proofed_at, tombs.confirmed_at, tombs.engraved_at,"
        "tombs.delivered_at, tombs.created_at, tombs.edited_at "
        "FROM " + this->table + " "
        "JOIN clients ON tombs.client_id = clients.id "
        "JOIN materials ON tombs.material_code = materials.code "
        "JOIN vases ON tombs.vase_code = vases.code "
        "JOIN lamps ON tombs.lamp_code = lamps.code "
        "JOIN flames ON tombs.flame_code = flames.code "
        "WHERE tombs.progressive = :progressive;");

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
        tomb["client"] = query.value("client_name").toString();
        tomb["name"] = query.value("name").toString();
        tomb["engraved_names"] = query.value("engraved_names").toString();
        tomb["engraved"] = query.value("engraved").toString();
        tomb["price"] = query.value("price").toString();
        tomb["paid"] = query.value("paid").toString();
        tomb["material_code"] = query.value("material_code").toString();
        tomb["material"] = query.value("material_name").toString();
        tomb["vase_code"] = query.value("vase_code").toString();
        tomb["vase"] = query.value("vase_name").toString();
        tomb["lamp_code"] = query.value("lamp_code").toString();
        tomb["lamp"] = query.value("lamp_name").toString();
        tomb["flame_code"] = query.value("flame_code").toString();
        tomb["flame"] = query.value("flame_name").toString();
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
    qDebug() << "TOMB: GET DETAILS";
    QList<QMap<QString, QString>> tombs;
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(this->db);
    query.prepare("SELECT tombs.progressive AS progressive, tombs.name AS name, "
        "tombs.confirmed_at AS confirmed_at, clients.name AS client_name, materials.name AS material "
        "FROM " + this->table + " "
        "JOIN clients ON tombs.client_id = clients.id "
        "JOIN materials ON tombs.material_code = materials.code "
        "WHERE (tombs.engraved = 1 AND tombs.confirmed_at != '' AND tombs.confirmed_at IS NOT NULL) AND "
        "(tombs.engraved_at == '' OR tombs.engraved_at IS NULL)"
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
    query.prepare("SELECT tombs.name AS name, clients.name AS client_name, vases.name AS vase_name, "
        "lamps.name AS lamp_name, flames.name AS flame_name, materials.name AS material_name "
        "FROM " + this->table + " "
        "JOIN materials ON tombs.material_code = materials.code "
        "JOIN vases ON tombs.vase_code = vases.code "
        "JOIN lamps ON tombs.lamp_code = lamps.code "
        "JOIN flames ON tombs.flame_code = flames.code "
        "JOIN clients ON tombs.client_id = clients.id "
        "WHERE tombs.accessories_mounted = 0 AND (tombs.confirmed_at != '' AND tombs.confirmed_at IS NOT NULL) "
        "AND (tombs.vase_code != 'NV' OR tombs.lamp_code != 'NL' OR tombs.flame_code != 'NF');"
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
    query.prepare("SELECT tombs.name AS name, tombs.price AS price, clients.name AS client_name "
        "FROM " + this->table + " "
        "JOIN clients ON tombs.client_id = clients.id "
        "WHERE tombs.delivered_at != '' AND tombs.delivered_at IS NOT NULL AND tombs.paid = 0 AND tombs.price != 0;"
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
            tomb["deceased"] = query.value("name").toString();
            tomb["price"] = query.value("price").toString();
            tomb["client"] = query.value("client_name").toString();
            tombs.append(tomb);
        }
    }

    return tombs;
}

/********** PRIVATE FUNCTIONS **********/

bool Tomb::checkDates(const QString& order, const QString& proof, const QString& confirmation, const QString& engraving, const QString& delivery)
{

    // About the dates when updating a tomb, check for the "-" character which is set when a NULL is found
    // in the database when retrieving the dates (NULLs are related to the data loss in the past)

    // The dates can be empty, they also can have not a valid format
    if (order.trimmed() == "" || !Helpers::isValidItaDate(order.trimmed())) {
        // But if the format is not correct, only the string "-" is acceptable 
        if (order.trimmed() != "-") {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data dell'ordine non è valida");
            message.exec();
            return false;
        }
    }

    // If the proof date is set
    if (proof.trimmed() != "" && proof.trimmed() != "-") {
        if (!Helpers::isValidItaDate(proof.trimmed())) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data del provino non è valida");
            message.exec();
            return false;
        }

        // The proof date must not preced the order
        if (Helpers::compareItaDates(proof.trimmed(), order.trimmed()) < 0) {
            QMessageBox message;
            // Create and add the buttons to the QMessageBox
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data del provino è antecedente a quella dell'ordine");
            message.exec();

            // The user has been notified about the inconsistency, 
            // but can still decide to accept it clicking on "confirmBtn" button
            if (message.clickedButton() == (QAbstractButton*)abortBtn) {
                return false;
            }
        }
    }

    // If the confirmation date is set
    if (confirmation.trimmed() != "" && confirmation.trimmed() != "-") {
        // The proof date must be set to set the confirmation date
        if (proof.trimmed() == "" || proof.trimmed() == "-") {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data del provino non è impostata");
            message.exec();
            return false;
        }

        if (!Helpers::isValidItaDate(confirmation.trimmed())) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della conferma non è valida");
            message.exec();
            return false;
        }

        // The confirmation date must not preced the proof
        if (Helpers::compareItaDates(confirmation.trimmed(), proof.trimmed()) < 0) {
            QMessageBox message;
            // Create and add the buttons to the QMessageBox
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della conferma è antecedente a quella del provino");
            message.exec();

            // The user has been notified about the inconsistency, 
            // but can still decide to accept it clicking on "confirmBtn" button
            if (message.clickedButton() == (QAbstractButton*)abortBtn) {
                return false;
            }
        }
    }

    // If the engraving date is set
    if (engraving.trimmed() != "" && engraving.trimmed() != "-") {
        // The confirmation date must be set to set the engraving date
        if (confirmation.trimmed() == "" || confirmation.trimmed() == "-") {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della conferma non è impostata");
            message.exec();
            return false;
        }

        // The dates must be checked only if the tomb had to be engraved
        if (engraving.trimmed() != this->not_engraved) {

            if (!Helpers::isValidItaDate(engraving.trimmed())) {
                QMessageBox message;
                message.setWindowTitle("Funeraria");
                message.setIcon(QMessageBox::Warning);
                message.setText("La data dell'incisione non è valida");
                message.exec();
                return false;
            }

            // The engraving date must not preced the confirmation
            if (Helpers::compareItaDates(engraving.trimmed(), confirmation.trimmed()) < 0) {
                QMessageBox message;
                // Create and add the buttons to the QMessageBox
                QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
                QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

                message.setWindowTitle("Funeraria");
                message.setIcon(QMessageBox::Warning);
                message.setText("La data dell'incisione è antecedente a quella della conferma");
                message.exec();

                // The user has been notified about the inconsistency, 
                // but can still decide to accept it clicking on "confirmBtn" button
                if (message.clickedButton() == (QAbstractButton*)abortBtn) {
                    return false;
                }
            }
        }
    }

    // If the delivery date is set and the tomb was engraved (it has an engraving date)
    if (delivery.trimmed() != "" && 
        delivery.trimmed() != "-" && 
        delivery.trimmed() != "Consegnata" &&
        engraving.trimmed() != this->not_engraved) {
        // The engraving date must be set to set delivery date
        if (engraving.trimmed() == "" || engraving.trimmed() == "-") {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data dell'incisione non è impostata");
            message.exec();
            return false;
        }

        if (!Helpers::isValidItaDate(delivery.trimmed())) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della consegna non è valida");
            message.exec();
            return false;
        }

        // The delivery date must not preced the engraving
        if (Helpers::compareItaDates(delivery.trimmed(), engraving.trimmed()) < 0) {
            QMessageBox message;
            // Create and add the buttons to the QMessageBox
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della consegna è antecedente a quella dell'incisione");
            message.exec();

            // The user has been notified about the inconsistency, 
            // but can still decide to accept it clicking on "confirmBtn" button
            if (message.clickedButton() == (QAbstractButton*)abortBtn) {
                return false;
            }
        }
    }

    return true;
}

bool Tomb::store(
    const int& progressive,
    const int& client_id,
    const QString& name,
    const QString& engraved_names,
    const bool& engraved,
    const double& price,
    const bool& paid,
    const QString& material_code,
    const QString& vase_code,
    const QString& lamp_code,
    const QString& flame_code,
    const QString& notes,
    const bool& accessories_mounted,
    const QString& ordered_at,
    const QString& proofed_at,
    const QString& confirmed_at,
    const QString& engraved_at,
    const QString& delivered_at
)
{
    int current_last = this->getLastProgresive();

    /************************ Validate the form fields *************************/

    if (this->isProgressiveInUse(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero assegnato alla lapide è già in uso");
        message.exec();
        return false;
    }

    if (progressive > current_last + 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero assegnato alla lapide non può essere maggiore di " + QString::number(current_last + 1));
        message.exec();
        return false;
    }

    if (client_id == 0) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il cliente selezionato non è valido");
        message.exec();
        return false;
    }

    if (name.trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome del defunto è obbligatorio");
        message.exec();
        return false;
    }

    if (!this->checkDates(ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at)) {
        return false;
    }

    /*************************************************/

    // Validation passed

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
        "(progressive, client_id, name, engraved_names, engraved, price, paid, material_code, vase_code, lamp_code, "
        "flame_code, notes, accessories_mounted, ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at, "
        "created_at, edited_at) "
        "VALUES (:progressive, :client_id, :name, :engraved_names, :engraved, :price, :paid, :material_code, :vase_code, :lamp_code, "
        ":flame_code, :notes, :accessories_mounted, :ordered_at, :proofed_at, :confirmed_at, :engraved_at, :delivered_at, "
        ":created_at, :edited_at)"
    );
    query.bindValue(":progressive", progressive);
    query.bindValue(":client_id", client_id);
    query.bindValue(":name", name.trimmed());
    query.bindValue(":engraved_names", (engraved_names.trimmed() == "") ? name.trimmed() : engraved_names.trimmed());
    query.bindValue(":engraved", engraved);
    query.bindValue(":price", price);
    query.bindValue(":paid", paid);
    query.bindValue(":material_code", material_code);
    query.bindValue(":vase_code", vase_code);
    query.bindValue(":lamp_code", lamp_code);
    query.bindValue(":flame_code", flame_code);
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
    const int& old_progressive,
    const int& progressive,
    const int& client_id,
    const QString& name,
    const QString& engraved_names,
    const bool& engraved,
    const double& price,
    const bool& paid,
    const QString& material_code,
    const QString& vase_code,
    const QString& lamp_code,
    const QString& flame_code,
    const QString& notes,
    const bool& accessories_mounted,
    const QString& ordered_at,
    const QString& proofed_at,
    const QString& confirmed_at,
    const QString& engraved_at,
    const QString& delivered_at
)
{
    int current_last = this->getLastProgresive();

    /************************ Validate the form fields *************************/

    // If the progressive has changed and the new number is already in use
    if (old_progressive != progressive && this->isProgressiveInUse(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero assegnato alla lapide è già in uso");
        message.exec();
        return false;
    }

    if (progressive > current_last + 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero assegnato alla lapide non può essere maggiore di " + QString::number(current_last + 1));
        message.exec();
        return false;
    }

    if (client_id == 0) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il cliente selezionato non è valido");
        message.exec();
        return false;
    }

    if (name.trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome del defunto è obbligatorio");
        message.exec();
        return false;
    }

    // About the dates when updating a tomb, check for the "-" character which is set when a NULL is found
    // in the database when retrieving the dates (NULLs related to the data loss in the past)

    if (!this->checkDates(ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at)) {
        return false;
    }

    /*************************************************/

    // Validation passed

    QSqlQuery query = QSqlQuery(this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET progressive = :progressive, client_id = :client_id, name = :name, engraved_names = :engraved_names, "
        "engraved = :engraved, price = :price, paid = :paid, material_code = :material_code, vase_code = :vase_code, "
        "lamp_code = :lamp_code, flame_code = :flame_code, notes = :notes, accessories_mounted = :accessories_mounted, "
        "ordered_at = :ordered_at, proofed_at = :proofed_at, confirmed_at = :confirmed_at, engraved_at = :engraved_at, "
        "delivered_at = :delivered_at, edited_at = :edited_at "
        "WHERE progressive = :old_progressive;"
    );
    query.bindValue(":progressive", progressive);
    query.bindValue(":client_id", client_id);
    query.bindValue(":name", name.trimmed());
    query.bindValue(":engraved_names", engraved_names.trimmed());
    query.bindValue(":engraved", engraved);
    query.bindValue(":price", price);
    query.bindValue(":paid", paid);
    query.bindValue(":material_code", material_code);
    query.bindValue(":vase_code", vase_code);
    query.bindValue(":lamp_code", lamp_code);
    query.bindValue(":flame_code", flame_code);
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

void Tomb::remove(const int& progressive)
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
}

bool Tomb::setPaid(const int& progressive)
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

bool Tomb::isProgressiveInUse(const int& progressive)
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

QString Tomb::getGroupingFolder(const int& progressive)
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

QString Tomb::getFolderPath(const int& progressive, const QString& name)
{
    Config* config = new Config();

    QString path_to_archive = config->getArchivePath();

    delete config;

    return "file:///" + path_to_archive + "/" + this->getGroupingFolder(progressive) + "/" + QString::number(progressive) + " - " + name.trimmed();
}


