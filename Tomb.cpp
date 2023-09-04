#include "Tomb.h"

/********** CONSTRUCTOR **********/

Tomb::Tomb(QSqlDatabase* db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    this->progressive = 0;
    this->client = new Client(this->db, this);
    this->material = new Accessory(this->db, "materials", this);
    this->vase = new Accessory(this->db, "vases", this);
    this->lamp = new Accessory(this->db, "lamps", this);
    this->flame = new Accessory(this->db, "flames", this);

    this->connect(this->ui.chbAllowEdit, &QCheckBox::stateChanged, this, &Tomb::slotSwitchEnableState);
    this->connect(this->ui.btnAvailableProgressives, &QPushButton::clicked, this, &Tomb::slotAvailableProgressives);
    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &Tomb::slotSave);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &Tomb::slotCloseDialog);
}

/********** DESTRUCTOR **********/

Tomb::~Tomb()
{
    delete this->client;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
    delete this->material;
}

/********** PUBLIC FUNCTIONS **********/

QList<QMap<QString, QString>> Tomb::getList(const int& client_id, const int& year, const QString& filter)
{
    QString order = "DESC";
    QList<QMap<QString, QString>> list;
    QSqlQuery query = QSqlQuery(*this->db);

    QString query_string = "SELECT * FROM " + this->table + " WHERE 1 = 1";

    if (client_id > 0) {
        query_string += " AND client_id = " + QString::number(client_id) ;
    }

    if (filter.trimmed() != "") {
        query_string += " AND name LIKE \"" + filter + "%\"";
    }

    if (year != 0) {
        query_string += " AND ordered_at LIKE '" + QString::number(year) + "%'";
        // When looking for a spedific year, show from the older to the newer, there if a few to scroll
        // while looking for all the orders of the specific client, shows the list from the newer which is
        // generally where we want to look in that situation
        order = "ASC";
    }

    query_string += " ORDER BY progressive " + order;

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
        tomb["progressive"] = QString::number(query.value("progressive").toInt());
        tomb["client_id"] = query.value("client_id").toString();
        tomb["name"] = query.value("name").toString();
        tomb["material"] = query.value("material_code").toString();
        tomb["additional_names"] = query.value("additional_names").toString();
        tomb["price"] = query.value("price").toString();
        tomb["paid"] = query.value("paid").toString();
        tomb["material_code"] = query.value("material_code").toString();
        tomb["vase_code"] = query.value("vase_code").toString();
        tomb["lamp_code"] = query.value("lamp_code").toString();
        tomb["flame_code"] = query.value("flame_code").toString();
        tomb["notes"] = query.value("notes").toString();
        tomb["accessories_mounted"] = query.value("accessories_mounted").toString();
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
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT "
        "tombs.progressive, tombs.client_id, clients.name AS client_name, tombs.name, tombs.additional_names,"
        "tombs.price, tombs.paid, tombs.material_code, materials.name AS material_name, tombs.vase_code, "
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
        tomb["additional_names"] = query.value("additional_names").toString();
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

void Tomb::setProgressive(const int& progressive)
{
    this->progressive = progressive;
    this->updateForm();
}

QList<QMap<QString, QString>> Tomb::tombsToEngrave()
{
    QList<QMap<QString, QString>> tombs;
    QMap<QString, QString> tomb;
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT tombs.progressive AS progressive, tombs.name AS name, clients.name AS client_name, materials.name AS material "
        "FROM " + this->table + " "
        "JOIN clients ON tombs.client_id = clients.id "
        "JOIN materials ON tombs.material_code = materials.code "
        "WHERE (tombs.confirmed_at != '' AND tombs.confirmed_at IS NOT NULL) AND (tombs.engraved_at == '' OR tombs.engraved_at IS NULL);"
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
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT tombs.name AS name, clients.name AS client_name, vases.name AS vase_name, "
        "lamps.name AS lamp_name, flames.name AS flame_name, materials.name AS material_name "
        "FROM " + this->table + " "
        "JOIN materials ON tombs.material_code = materials.code "
        "JOIN vases ON tombs.vase_code = vases.code "
        "JOIN lamps ON tombs.lamp_code = lamps.code "
        "JOIN flames ON tombs.flame_code = flames.code "
        "JOIN clients ON tombs.client_id = clients.id "
        "WHERE tombs.accessories_mounted = 0 AND (tombs.engraved_at != '' "
        "AND tombs.engraved_at IS NOT NULL) "
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
    QSqlQuery query = QSqlQuery(*this->db);
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

/********** PROTECTED SLOTS **********/

void Tomb::slotSwitchEnableState()
{
    this->ui.leProgressive->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbClient->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbMaterial->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbVase->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbLamp->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbFlame->setEnabled(this->ui.chbAllowEdit->isChecked());
}

void Tomb::slotAvailableProgressives()
{
    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare("SELECT progressive FROM " + this->table + " ORDER BY progressive ASC");

    if (!query.exec()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText(query.lastError().text());
        message.exec();
        return;
    }
    
    QList<int> progressive;

    while (query.next()) {
        progressive.append(query.value("progressive").toInt());
    }

    int min = progressive[0];
    int max = progressive[progressive.size() - 1];

    if (static_cast<long long>(max) - min + 1 == progressive.size()) {
        // No holes in the progressives' list, only the one after the last is available
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Numeri disponibili: " + QString::number(this->getLastProgresive() + 1));
        message.exec();
        return;
    }

    QString holes = "";

    // There are holes in the progressives' list, need to spot them
    for (int i = 1; i < progressive.size(); i++) {
        int num_expected = min + i;

        if (!progressive.contains(num_expected)) {
            holes += "\n" + QString::number(num_expected);
        }
    }

    holes += "\n" + QString::number(this->getLastProgresive() + 1);

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Numeri disponibili: " + holes);
    message.exec();
}

void Tomb::slotSave()
{
    if (this->ui.btnSave->text() == this->btnCreateText) {
        if (this->store(
                this->ui.leProgressive->text().toInt(),
                this->client->getId(this->ui.cbClient->currentText()),
                this->ui.leName->text(),
                this->ui.leAdditionalNames->text(),
                this->ui.lePrice->text().toDouble(),
                this->ui.chbPaid->isChecked(),
                this->material->getCode(this->ui.cbMaterial->currentText()),
                this->vase->getCode(this->ui.cbVase->currentText()),
                this->lamp->getCode(this->ui.cbLamp->currentText()),
                this->flame->getCode(this->ui.cbFlame->currentText()),
                this->ui.ptNote->toPlainText(),
                this->ui.chbAccessoriesMounted->isChecked(),
                this->ui.leOrderedAt->text(),
                this->ui.leProofedAt->text(),
                this->ui.leConfirmedAt->text(),
                this->ui.leEngravedAt->text(),
                this->ui.leDeliveredAt->text()
            )
        ) {
            this->close();
        }
    }
    else if (this->ui.btnSave->text() == this->btnUpdateText) {
        if (this->update(
                this->progressive,
                this->ui.leProgressive->text().toInt(),
                this->client->getId(this->ui.cbClient->currentText()),
                this->ui.leName->text(),
                this->ui.leAdditionalNames->text(),
                this->ui.lePrice->text().toDouble(),
                this->ui.chbPaid->isChecked(),
                this->material->getCode(this->ui.cbMaterial->currentText()),
                this->vase->getCode(this->ui.cbVase->currentText()),
                this->lamp->getCode(this->ui.cbLamp->currentText()),
                this->flame->getCode(this->ui.cbFlame->currentText()),
                this->ui.ptNote->toPlainText(),
                this->ui.chbAccessoriesMounted->isChecked(),
                this->ui.leOrderedAt->text(),
                this->ui.leProofedAt->text(),
                this->ui.leConfirmedAt->text(),
                this->ui.leEngravedAt->text(),
                this->ui.leDeliveredAt->text()
            )
        ) {
            this->close();
        }
    }
}

void Tomb::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

bool Tomb::checkDates(const QString& order, const QString& proof, const QString& confirmation, const QString& engraving, const QString& delivery)
{
    if (order.trimmed() == "" || !Helpers::isValidItaDate(order.trimmed())) {
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
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data del provino è antecedente a quella dell'ordine");
            message.exec();

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
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della conferma è antecedente a quella del provino");
            message.exec();

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
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data dell'incisione è antecedente a quella della conferma");
            message.exec();

            if (message.clickedButton() == (QAbstractButton*)abortBtn) {
                return false;
            }
        }
    }

    // If the delivery date is set
    if (delivery.trimmed() != "" && delivery.trimmed() != "-") {
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
            QPushButton* confirmBtn = message.addButton("Continua", QMessageBox::ActionRole);
            QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);

            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("La data della consegna è antecedente a quella dell'incisione");
            message.exec();

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
    const QString& additional_names,
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

    // About the dates when updating a tomb, check for the "-" character which is set when a NULL is found
    // in the database when retrieving the dates (NULLs related to the data loss in the past)

    if (!this->checkDates(ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at)) {
        return false;
    }

    /*************************************************/

    // Validation passed

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare(
        "INSERT INTO " + this->table + " "
        "(progressive, client_id, name, additional_names, price, paid, material_code, vase_code, lamp_code, "
        "flame_code, notes, accessories_mounted, ordered_at, proofed_at, confirmed_at, engraved_at, delivered_at, "
        "created_at, edited_at) "
        "VALUES (:progressive, :client_id, :name, :additional_names, :price, :paid, :material_code, :vase_code, :lamp_code, "
        ":flame_code, :notes, :accessories_mounted, :ordered_at, :proofed_at, :confirmed_at, :engraved_at, :delivered_at, "
        ":created_at, :edited_at)"
    );
    query.bindValue(":progressive", progressive);
    query.bindValue(":client_id", client_id);
    query.bindValue(":name", name.trimmed());
    query.bindValue(":additional_names", additional_names.trimmed());
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
    query.bindValue(":engraved_at", QDate::fromString(engraved_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    query.bindValue(":delivered_at", QDate::fromString(delivered_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
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
    const QString& additional_names,
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

    QSqlQuery query = QSqlQuery(*this->db);
    query.prepare(
        "UPDATE " + this->table + " "
        "SET progressive = :progressive, client_id = :client_id, name = :name, additional_names = :additional_names, "
        "price = :price, paid = :paid, material_code = :material_code, vase_code = :vase_code, lamp_code = :lamp_code, "
        "flame_code = :flame_code, notes = :notes, accessories_mounted = :accessories_mounted, ordered_at = :ordered_at, "
        "proofed_at = :proofed_at, confirmed_at = :confirmed_at, engraved_at = :engraved_at, delivered_at = :delivered_at, "
        "edited_at = :edited_at "
        "WHERE progressive = :old_progressive;"
    );
    query.bindValue(":progressive", progressive);
    query.bindValue(":client_id", client_id);
    query.bindValue(":name", name.trimmed());
    query.bindValue(":additional_names", additional_names.trimmed());
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
    query.bindValue(":engraved_at", QDate::fromString(engraved_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
    query.bindValue(":delivered_at", QDate::fromString(delivered_at.trimmed(), "dd/MM/yyyy").toString("yyyy-MM-dd"));
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

int Tomb::getLastProgresive() {
    QSqlQuery query = QSqlQuery(*this->db);
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

void Tomb::updateForm()
{
    // Get the selected tomb's data
    QMap<QString, QString> tomb = this->getDetails(this->progressive);

    QList<QMap<QString, QString>> clients = this->client->get();
    QList<QMap<QString, QString>> materials = this->material->get();
    QList<QMap<QString, QString>> vases = this->vase->get();
    QList<QMap<QString, QString>> lamps = this->lamp->get();
    QList<QMap<QString, QString>> flames = this->flame->get();

    int client_index = 0;
    int material_index = 0;
    int vase_index = 0;
    int lamp_index = 0;
    int flame_index = 0;

    QList<QString> client_names = this->client->getNames();
    QList<QString> material_names = this->material->getNames();
    QList<QString> vase_names = this->vase->getNames();
    QList<QString> lamp_names = this->lamp->getNames();
    QList<QString> flame_names = this->flame->getNames();

    // Clear the combo boxes
    this->ui.cbClient->clear();
    this->ui.cbMaterial->clear();
    this->ui.cbVase->clear();
    this->ui.cbLamp->clear();
    this->ui.cbFlame->clear();

    if (!tomb.isEmpty()) {
        this->setWindowTitle("Modifica lapide");

        /*********** Get the index to select for the comboboxes *************/
        for (int i = 0; i < clients.size(); i++) {
            if (client_names[i] == tomb["client"]) {
                client_index = i;
                break;
            }
        }

        for (int i = 0; i < materials.size(); i++) {
            if (materials[i]["code"] == tomb["material_code"]) {
                material_index = i;
                break;
            }
        }

        for (int i = 0; i < vases.size(); i++) {
            if (vases[i]["code"] == tomb["vase_code"]) {
                vase_index = i;
                break;
            }
        }

        for (int i = 0; i < lamps.size(); i++) {
            if (lamps[i]["code"] == tomb["lamp_code"]) {
                lamp_index = i;
                break;
            }
        }

        for (int i = 0; i < flames.size(); i++) {
            if (flames[i]["code"] == tomb["flame_code"]) {
                flame_index = i;
                break;
            }
        }
        /******************************************************/

        QStringList emails_list = tomb["email"].split(u',');
        QString emails = "";
        for (int i = 0; i < emails_list.length(); i++) {
            QString nl = "\n";
            if (i == emails_list.length() - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[i] + nl;
        }

        QStringList phones_list = tomb["phone"].split(u',');
        QString phones = "";
        for (int i = 0; i < phones_list.length(); i++) {
            QString nl = "\n";
            if (i == phones_list.length() - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[i] + nl;
        }

        // Fill the form fields with the selected tomb's data
        this->ui.leProgressive->setText(tomb["progressive"]);
        this->ui.cbClient->addItems(client_names);
        this->ui.leName->setText(tomb["name"]);
        this->ui.leAdditionalNames->setText(tomb["additional_names"]);
        this->ui.lePrice->setText(tomb["price"]);
        this->ui.chbPaid->setChecked(tomb["paid"] == "1");
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.chbAccessoriesMounted->setChecked(tomb["accessories_mounted"] == "1");
        this->ui.ptNote->setPlainText(tomb["notes"]);
        this->ui.leOrderedAt->setText(Helpers::dateSqlToIta(tomb["ordered_at"]));
        this->ui.leProofedAt->setText(Helpers::dateSqlToIta(tomb["proofed_at"]));
        this->ui.leConfirmedAt->setText(Helpers::dateSqlToIta(tomb["confirmed_at"]));
        this->ui.leEngravedAt->setText(Helpers::dateSqlToIta(tomb["engraved_at"]));
        this->ui.leDeliveredAt->setText(Helpers::dateSqlToIta(tomb["delivered_at"]));

        // Disable the fields which should not generally be edited
        this->ui.leProgressive->setEnabled(false);
        this->ui.cbClient->setEnabled(false);
        this->ui.cbMaterial->setEnabled(false);
        this->ui.cbVase->setEnabled(false);
        this->ui.cbLamp->setEnabled(false);
        this->ui.cbFlame->setEnabled(false);

        // Set the item to show inside the combo boxes
        this->ui.cbClient->setCurrentIndex(client_index);
        this->ui.cbMaterial->setCurrentIndex(material_index);
        this->ui.cbVase->setCurrentIndex(vase_index);
        this->ui.cbLamp->setCurrentIndex(lamp_index);
        this->ui.cbFlame->setCurrentIndex(flame_index);

        // Set the "Allow edit" checkbox unchecked
        this->ui.chbAllowEdit->setChecked(false);
        // Set the "Allow edit" checkbox enabled
        this->ui.chbAllowEdit->setEnabled(true);

        // Set the save button text
        this->ui.btnSave->setText(this->btnUpdateText);
    }
    else {
        // Tomb not found means we are asking to insert a new one
        this->setWindowTitle("Crea nuova");

        this->ui.leProgressive->setEnabled(true);
        this->ui.cbClient->setEnabled(true);
        this->ui.cbMaterial->setEnabled(true);
        this->ui.cbVase->setEnabled(true);
        this->ui.cbLamp->setEnabled(true);
        this->ui.cbFlame->setEnabled(true);


        // Reset the form fields
        this->ui.leProgressive->setText(QString::number(this->getLastProgresive() + 1));
        this->ui.cbClient->addItems(client_names);
        this->ui.leName->setText("");
        this->ui.leAdditionalNames->setText("");
        this->ui.lePrice->setText("0");
        this->ui.chbPaid->setChecked(false);
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.chbAccessoriesMounted->setChecked(false);
        this->ui.ptNote->setPlainText("");
        this->ui.leOrderedAt->setText(QDate::currentDate().toString("dd/MM/yyyy"));
        this->ui.leProofedAt->setText(QDate::currentDate().toString("dd/MM/yyyy"));
        this->ui.leConfirmedAt->setText("");
        this->ui.leEngravedAt->setText("");
        this->ui.leDeliveredAt->setText("");

        this->ui.chbAllowEdit->setEnabled(false);

        // Set the save button text
        this->ui.btnSave->setText(this->btnCreateText);
    }
}

bool Tomb::isProgressiveInUse(const int& progressive)
{
    QSqlQuery query = QSqlQuery(*this->db);
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
