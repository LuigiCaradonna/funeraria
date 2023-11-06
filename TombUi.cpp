#include "TombUi.h"

/********** CONSTRUCTOR **********/

TombUi::TombUi(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    this->progressive = 0;
    this->material = new Accessory(this->db, "materials");
    this->vase = new Accessory(this->db, "vases");
    this->lamp = new Accessory(this->db, "lamps");
    this->flame = new Accessory(this->db, "flames");

    this->connect(this->ui.chbAllowEdit, &QCheckBox::stateChanged, this, &TombUi::slotSwitchEnableState);
    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &TombUi::slotSave);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &TombUi::slotCloseDialog);

    // Map the signal coming from the clicked button to call the same function (slotSetCurrentDate) with the proper parameter
    this->currentDateMapper = new QSignalMapper(this);
    this->connect(this->ui.btnOrderedAt, SIGNAL(clicked()), currentDateMapper, SLOT(map()));
    this->connect(this->ui.btnProofedAt, SIGNAL(clicked()), currentDateMapper, SLOT(map()));
    this->connect(this->ui.btnConfirmedAt, SIGNAL(clicked()), currentDateMapper, SLOT(map()));
    this->connect(this->ui.btnEngravedAt, SIGNAL(clicked()), currentDateMapper, SLOT(map()));
    this->connect(this->ui.btnDeliveredAt, SIGNAL(clicked()), currentDateMapper, SLOT(map()));
    currentDateMapper->setMapping(this->ui.btnOrderedAt, "order");
    currentDateMapper->setMapping(this->ui.btnProofedAt, "proof");
    currentDateMapper->setMapping(this->ui.btnConfirmedAt, "confirm");
    currentDateMapper->setMapping(this->ui.btnEngravedAt, "engrave");
    currentDateMapper->setMapping(this->ui.btnDeliveredAt, "deliver");
    this->connect(currentDateMapper, &QSignalMapper::mappedString, this, &TombUi::slotSetCurrentDate);
}

/********** DESTRUCTOR **********/

TombUi::~TombUi()
{
    delete this->material;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
}

/********** PUBLIC FUNCTIONS **********/

void TombUi::setProgressive(const int& progressive)
{
    this->progressive = progressive;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

void TombUi::slotSwitchEnableState()
{
    this->ui.leProgressive->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbClient->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbMaterial->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbVase->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbLamp->setEnabled(this->ui.chbAllowEdit->isChecked());
    this->ui.cbFlame->setEnabled(this->ui.chbAllowEdit->isChecked());
}

void TombUi::slotSetCurrentDate(const QString& field)
{
    QString today = QDate::currentDate().toString("dd/MM/yyyy");

    if (field == "order") {
        this->ui.leOrderedAt->setText(today);
    }
    else if (field == "proof") {
        this->ui.leProofedAt->setText(today);
    }
    else if (field == "confirm") {
        this->ui.leConfirmedAt->setText(today);
    }
    else if (field == "engrave") {
        this->ui.leEngravedAt->setText(today);
    }
    else if (field == "deliver") {
        this->ui.leDeliveredAt->setText(today);
    }
}

void TombUi::slotSave()
{
    Tomb* tomb = new Tomb(this->db);
    Client* client = new Client(this->db);
    if (this->ui.btnSave->text() == this->btn_create_text) {
        if (tomb->store(
            this->ui.leProgressive->text().toInt(),
            client->getId(this->ui.cbClient->currentText()),
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
            delete tomb;
            this->close();
        }
    }
    else if (this->ui.btnSave->text() == this->btn_update_text) {
        if (tomb->update(
            this->progressive,
            this->ui.leProgressive->text().toInt(),
            client->getId(this->ui.cbClient->currentText()),
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
            delete tomb;
            delete client;
            this->close();
        }
    }
}

void TombUi::slotCloseDialog()
{
    this->close();
}

void TombUi::updateForm()
{
    Tomb* tomb = new Tomb(this->db);
    Client* client = new Client(this->db);
    // Get the selected tomb's data
    QMap<QString, QString> tomb_details = tomb->getDetails(this->progressive);

    QList<QMap<QString, QString>> clients = client->get();
    QList<QMap<QString, QString>> materials = this->material->get();
    QList<QMap<QString, QString>> vases = this->vase->get();
    QList<QMap<QString, QString>> lamps = this->lamp->get();
    QList<QMap<QString, QString>> flames = this->flame->get();

    int client_index = 0;
    int material_index = 0;
    int vase_index = 0;
    int lamp_index = 0;
    int flame_index = 0;

    QList<QString> client_names = client->getActiveNames();
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

    if (!tomb_details.isEmpty()) {
        this->setWindowTitle("Modifica lapide");

        /*********** Get the index to select for the comboboxes *************/
        for (int i = 0; i < clients.size(); i++) {
            if (client_names[i] == tomb_details["client"]) {
                client_index = i;
                break;
            }
        }

        for (int i = 0; i < materials.size(); i++) {
            if (materials[i]["code"] == tomb_details["material_code"]) {
                material_index = i;
                break;
            }
        }

        for (int i = 0; i < vases.size(); i++) {
            if (vases[i]["code"] == tomb_details["vase_code"]) {
                vase_index = i;
                break;
            }
        }

        for (int i = 0; i < lamps.size(); i++) {
            if (lamps[i]["code"] == tomb_details["lamp_code"]) {
                lamp_index = i;
                break;
            }
        }

        for (int i = 0; i < flames.size(); i++) {
            if (flames[i]["code"] == tomb_details["flame_code"]) {
                flame_index = i;
                break;
            }
        }
        /******************************************************/

        QStringList emails_list = tomb_details["email"].split(u',');
        QString emails = "";
        for (int i = 0; i < emails_list.length(); i++) {
            QString nl = "\n";
            if (i == emails_list.length() - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[i] + nl;
        }

        QStringList phones_list = tomb_details["phone"].split(u',');
        QString phones = "";
        for (int i = 0; i < phones_list.length(); i++) {
            QString nl = "\n";
            if (i == phones_list.length() - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[i] + nl;
        }

        // Fill the form fields with the selected tomb's data
        this->ui.leProgressive->setText(tomb_details["progressive"]);
        this->ui.cbClient->addItems(client_names);
        this->ui.leName->setText(tomb_details["name"]);
        this->ui.leAdditionalNames->setText(tomb_details["additional_names"]);
        this->ui.lePrice->setText(tomb_details["price"]);
        this->ui.chbPaid->setChecked(tomb_details["paid"] == "1");
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.chbAccessoriesMounted->setChecked(tomb_details["accessories_mounted"] == "1");
        this->ui.ptNote->setPlainText(tomb_details["notes"]);
        this->ui.leOrderedAt->setText(Helpers::dateSqlToIta(tomb_details["ordered_at"]));
        this->ui.leProofedAt->setText(Helpers::dateSqlToIta(tomb_details["proofed_at"]));
        this->ui.leConfirmedAt->setText(Helpers::dateSqlToIta(tomb_details["confirmed_at"]));
        this->ui.leEngravedAt->setText(Helpers::dateSqlToIta(tomb_details["engraved_at"]));
        this->ui.leDeliveredAt->setText(Helpers::dateSqlToIta(tomb_details["delivered_at"]));

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
        this->ui.btnSave->setText(this->btn_update_text);
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
        this->ui.leProgressive->setText(QString::number(tomb->getLastProgresive() + 1));
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
        this->ui.btnSave->setText(this->btn_create_text);
    }

    delete tomb;
    delete client;
}
