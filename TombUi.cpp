#include "TombUi.h"

/********** CONSTRUCTOR **********/

TombUi::TombUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent)
    : db(db), css_folder(css_folder), icons_folder(icons_folder), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon(this->icons_folder + "funeraria.png"));

    // Load the stylesheet for the UI
    QString style = this->getStyle();

    if (!style.isEmpty()) {
        this->setStyleSheet(style);
    }

    this->progressive = 0;
    this->tomb = new Tomb(this->db);
    this->sculpture = new Sculpture(this->db);
    this->material = new Accessory(this->db, "materials");
    this->vase = new Accessory(this->db, "vases");
    this->lamp = new Accessory(this->db, "lamps");
    this->flame = new Accessory(this->db, "flames");
    this->tomb_type = new TombType(this->db);
    this->tomb_format = new TombFormat(this->db);
    this->pit_type = new PitType(this->db);
    this->pit_format = new PitFormat(this->db);

    // Connect the UI elements
    this->connect(this->ui.rbEngraveYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEngraveNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.leEpigraphAmount, &QLineEdit::textChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbMaterial, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbType, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbFormat, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbVase, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbLamp, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbFlame, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitFormatOne, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitTypeOne, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitFormatTwo, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitTypeTwo, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitFormatThree, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitTypeThree, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitFormatFour, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitTypeFour, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitFormatFive, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitTypeFive, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitFormatSix, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbPitTypeSix, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbMountYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbMountNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEpReliefYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEpReliefNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbInscriptionYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbInscriptionNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.leScHeight, &QLineEdit::textChanged, this, &TombUi::slotUpdateNotes);

    this->connect(this->ui.rbEngraveYes, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);
    this->connect(this->ui.rbEngraveNo, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);

    this->connect(this->ui.cbPitFormatOne, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFormatTwo, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFormatThree, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFormatFour, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFormatFive, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFormatSix, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);


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

    this->connect(this->ui.btnMissing, &QPushButton::clicked, this, &TombUi::slotNotInUseProgressives);
    this->connect(this->ui.btnNoEngraving, &QPushButton::clicked, this, &TombUi::slotSetNoEngraving);
    this->connect(this->ui.btnDelivered, &QPushButton::clicked, this, &TombUi::slotSetDelivered);
    this->connect(this->ui.btnDelete, &QPushButton::clicked, this, &TombUi::slotDelete);
}

/********** DESTRUCTOR **********/

TombUi::~TombUi()
{
    delete this->tomb;
    delete this->sculpture;
    delete this->material;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
    delete this->tomb_type;
    delete this->tomb_format;
    delete this->pit_type;
    delete this->pit_format;
    delete this->cross;
    delete this->sacred;
    delete this->currentDateMapper;
}

/********** PUBLIC FUNCTIONS **********/

void TombUi::setProgressive(const int& progressive)
{
    this->progressive = progressive;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

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

void TombUi::slotSetDelivered()
{
    this->ui.leDeliveredAt->setText("Consegnata");
}

void TombUi::slotSetNoEngraving()
{
    this->ui.leEngravedAt->setText(this->not_engraved);
}

void TombUi::slotNotInUseProgressives()
{
    QStringList not_in_use = this->tomb->getNotInUseProgressives();

    if (not_in_use.isEmpty()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non ci sono numeri progressivi utilizzabili.");
        message.exec();

        return;
    }

    bool ok;
    QString progressive = QInputDialog::getItem(
        this,                 // Parent widget
        "Numero progressivo", // Dialog title
        "Numeri",             // Label text
        not_in_use,           // Items for the combo box
        0,                    // Index of the default item
        false,                // Editable combo box (set to true if you want user to type custom text)
        &ok                   // To check if "OK" was clicked
    );

    if (ok && !progressive.isEmpty())
        this->ui.leProgressive->setText(progressive);
}

void TombUi::slotSave()
{
    if (!this->validateForm()) {
        return;
    }

    Client* client = new Client(this->db);
    // Default value, usually the tombs are engraved
    int engraved = 1;

    if (this->ui.rbEngraveNo->isChecked()) {
        engraved = 0;
    }
    else if (this->ui.rbEngraveBronze->isChecked()) {
        engraved = 2;
    }

    if (this->ui.btnSave->text() == this->btn_create_text) {
        if (this->tomb->store(
            this->ui.leProgressive->text().toInt(),
            client->getId(this->ui.cbClient->currentText()),
            this->ui.leName->text(),
            this->ui.leEngravedNames->text(),
            this->ui.leEpigraphAmount->text().toInt(),
            engraved,
            this->ui.lePrice->text().toDouble(),
            this->ui.chbPaid->isChecked(),
            this->material->getCode(this->ui.cbMaterial->currentText()),
            this->tomb_type->getCode(this->ui.cbType->currentText()),
            this->tomb_format->getCode(this->ui.cbFormat->currentText()),
            this->vase->getCode(this->ui.cbVase->currentText()),
            this->lamp->getCode(this->ui.cbLamp->currentText()),
            this->flame->getCode(this->ui.cbFlame->currentText()),
            this->cross->getCode(this->ui.cbCross->currentText()),
            this->sacred->getCode(this->ui.cbSacred->currentText()),
            this->sculpture->getCode(this->ui.cbSculpture->currentText()),
            this->ui.leScHeight->text().toInt(),
            this->ui.rbMountYes->isChecked(),
            this->ui.rbEpReliefYes->isChecked(),
            this->ui.rbInscriptionYes->isChecked(),
            this->pit_format->getCode(this->ui.cbPitFormatOne->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeOne->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatTwo->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeTwo->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatThree->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeThree->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatFour->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeFour->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatFive->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeFive->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatSix->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeSix->currentText()),
            this->ui.ptNote->toPlainText(),
            this->ui.chbAccessoriesMounted->isChecked(),
            this->ui.leOrderedAt->text(),
            this->ui.leProofedAt->text(),
            this->ui.leConfirmedAt->text(),
            this->ui.leEngravedAt->text(),
            this->ui.leDeliveredAt->text()
        )
            ) {
            delete client;
            this->close();
        }
    }
    else if (this->ui.btnSave->text() == this->btn_update_text) {
        if (tomb->update(
            this->progressive,
            this->ui.leProgressive->text().toInt(),
            client->getId(this->ui.cbClient->currentText()),
            this->ui.leName->text(),
            this->ui.leEngravedNames->text(),
            this->ui.leEpigraphAmount->text().toInt(),
            engraved,
            this->ui.lePrice->text().toDouble(),
            this->ui.chbPaid->isChecked(),
            this->material->getCode(this->ui.cbMaterial->currentText()),
            this->tomb_type->getCode(this->ui.cbType->currentText()),
            this->tomb_format->getCode(this->ui.cbFormat->currentText()),
            this->vase->getCode(this->ui.cbVase->currentText()),
            this->lamp->getCode(this->ui.cbLamp->currentText()),
            this->flame->getCode(this->ui.cbFlame->currentText()),
            this->cross->getCode(this->ui.cbCross->currentText()),
            this->sacred->getCode(this->ui.cbSacred->currentText()),
            this->sculpture->getCode(this->ui.cbSculpture->currentText()),
            this->ui.leScHeight->text().toInt(),
            this->ui.rbMountYes->isChecked(),
            this->ui.rbEpReliefYes->isChecked(),
            this->ui.rbInscriptionYes->isChecked(),
            this->pit_format->getCode(this->ui.cbPitFormatOne->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeOne->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatTwo->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeTwo->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatThree->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeThree->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatFour->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeFour->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatFive->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeFive->currentText()),
            this->pit_format->getCode(this->ui.cbPitFormatSix->currentText()),
            this->pit_type->getCode(this->ui.cbPitTypeSix->currentText()),
            this->ui.ptNote->toPlainText(),
            this->ui.chbAccessoriesMounted->isChecked(),
            this->ui.leOrderedAt->text(),
            this->ui.leProofedAt->text(),
            this->ui.leConfirmedAt->text(),
            this->ui.leEngravedAt->text(),
            this->ui.leDeliveredAt->text()
        )
            ) {
            delete client;
            this->close();
        }
    }
}

void TombUi::slotDelete()
{
    QMessageBox message;
    QPushButton* proceed_btn = message.addButton("Elimina", QMessageBox::ActionRole);
    QPushButton* abort_btn = message.addButton("Annulla", QMessageBox::ActionRole);
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Warning);
    message.setText("Vuoi eliminare questa lapide?");
    message.exec();

    if (message.clickedButton() == proceed_btn) {
        this->tomb->remove(this->ui.leProgressive->text().toInt());
    }

    this->close();
}

void TombUi::slotCloseDialog()
{
    this->close();
}

void TombUi::slotUpdateNotes()
{

}

void TombUi::slotUpdateEpNumState()
{
}

void TombUi::slotUpdatePitState()
{
}

void TombUi::slotUpdateScHightState()
{
}

/********** PRIVATE FUNCTIONS **********/

bool TombUi::validateForm()
{
    int current_last = this->tomb->getLastProgresive();

    if (this->tomb->isProgressiveInUse(progressive)) {
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

    Client* client = new Client(this->db);
    int client_id = client->getId(this->ui.cbClient->currentText());
    delete client;
    if (client_id == 0) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il cliente selezionato non è valido");
        message.exec();
        return false;
    }

    if (this->ui.leName->text().trimmed() == "") {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il nome del defunto è obbligatorio");
        message.exec();
        return false;
    }

    if (!this->tomb->checkDates(
        this->ui.leOrderedAt->text(), 
        this->ui.leProofedAt->text(), 
        this->ui.leConfirmedAt->text(),
        this->ui.leEngravedAt->text(),
        this->ui.leDeliveredAt->text())
    ) {
        return false;
    }

}

void TombUi::updateForm()
{
    Client* client = new Client(this->db);
    // Get the selected tomb's data
    QMap<QString, QString> tomb_details = this->tomb->getDetails(this->progressive);

    QList<QMap<QString, QString>> clients = client->get();
    QList<QMap<QString, QString>> materials = this->material->get();
    QList<QMap<QString, QString>> tomb_types = this->tomb_type->get();
    QList<QMap<QString, QString>> vases = this->vase->get();
    QList<QMap<QString, QString>> lamps = this->lamp->get();
    QList<QMap<QString, QString>> flames = this->flame->get();
    QList<QMap<QString, QString>> tomb_formats = this->tomb_format->get();
    QList<QMap<QString, QString>> pit_types = this->pit_type->get();
    QList<QMap<QString, QString>> pit_formats = this->pit_format->get();

    int client_index = 0;
    int material_index = 0;
    int type_index = 0;
    int vase_index = 0;
    int lamp_index = 0;
    int flame_index = 0;

    QList<QString> client_names = client->getActiveNames();
    QList<QString> material_names = this->material->getNames();
    QList<QString> tomb_type_names = this->tomb_type->getNames();
    QList<QString> tomb_format_names = this->tomb_format->getNames();
    QList<QString> vase_names = this->vase->getNames();
    QList<QString> lamp_names = this->lamp->getNames();
    QList<QString> flame_names = this->flame->getNames();
    QList<QString> pit_format_names = this->pit_format->getNames();
    QList<QString> pit_type_names = this->pit_type->getNames();

    // Clear the combo boxes
    this->ui.cbClient->clear();
    this->ui.cbMaterial->clear();
    this->ui.cbType->clear();
    this->ui.cbFormat->clear();
    this->ui.cbVase->clear();
    this->ui.cbLamp->clear();
    this->ui.cbFlame->clear();
    this->ui.cbPitFormatOne->clear();
    this->ui.cbPitTypeOne->clear();
    this->ui.cbPitFormatTwo->clear();
    this->ui.cbPitTypeTwo->clear();
    this->ui.cbPitFormatThree->clear();
    this->ui.cbPitTypeThree->clear();
    this->ui.cbPitFormatFour->clear();
    this->ui.cbPitTypeFour->clear();
    this->ui.cbPitFormatFive->clear();
    this->ui.cbPitTypeFive->clear();
    this->ui.cbPitFormatSix->clear();
    this->ui.cbPitTypeSix->clear();

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

        for (int i = 0; i < tomb_types.size(); i++) {
            if (tomb_types[i]["code"] == tomb_details["type_code"]) {
                type_index = i;
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

        QString engraved_at;

        if (tomb_details["engraved_at"] != this->not_engraved) {
            engraved_at = Helpers::dateSqlToIta(tomb_details["engraved_at"]);
        }
        else {
            engraved_at = this->not_engraved;
        }

        // Fill the form fields with the selected tomb's data
        this->ui.leProgressive->setText(tomb_details["progressive"]);
        this->ui.cbClient->addItems(client_names);
        this->ui.leName->setText(tomb_details["name"]);
        this->ui.leEngravedNames->setText(tomb_details["engraved_names"]);
        this->ui.leEpigraphAmount->setText(tomb_details["ep_amount"]);
        this->ui.rbEngraveYes->setChecked(tomb_details["engraved"] == "1");
        this->ui.rbEngraveNo->setChecked(tomb_details["engraved"] == "0");
        this->ui.lePrice->setText(tomb_details["price"]);
        this->ui.chbPaid->setChecked(tomb_details["paid"] == "1");
        this->ui.chbAccessoriesMounted->setChecked(tomb_details["accessories_mounted"] == "1");
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbType->addItems(tomb_type_names);
        this->ui.cbFormat->addItems(tomb_format_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.cbPitFormatOne->addItems(pit_format_names);
        this->ui.cbPitTypeOne->addItems(pit_type_names);
        this->ui.cbPitFormatTwo->addItems(pit_format_names);
        this->ui.cbPitTypeTwo->addItems(pit_type_names);
        this->ui.cbPitFormatThree->addItems(pit_format_names);
        this->ui.cbPitTypeThree->addItems(pit_type_names);
        this->ui.cbPitFormatFour->addItems(pit_format_names);
        this->ui.cbPitTypeFour->addItems(pit_type_names);
        this->ui.cbPitFormatFive->addItems(pit_format_names);
        this->ui.cbPitTypeFive->addItems(pit_type_names);
        this->ui.cbPitFormatSix->addItems(pit_format_names);
        this->ui.cbPitTypeSix->addItems(pit_type_names);
        this->ui.ptNote->setPlainText(tomb_details["notes"]);
        this->ui.leOrderedAt->setText(Helpers::dateSqlToIta(tomb_details["ordered_at"]));
        this->ui.leProofedAt->setText(Helpers::dateSqlToIta(tomb_details["proofed_at"]));
        this->ui.leConfirmedAt->setText(Helpers::dateSqlToIta(tomb_details["confirmed_at"]));
        this->ui.leEngravedAt->setText(engraved_at);
        
        QString delivery_date;
        if (tomb_details["delivered_at"] == "Consegnata") {
            this->ui.leDeliveredAt->setText("Consegnata");
        }
        else {
            this->ui.leDeliveredAt->setText(Helpers::dateSqlToIta(tomb_details["delivered_at"]));
        }

        // Set the item to show inside the combo boxes
        this->ui.cbClient->setCurrentIndex(client_index);
        this->ui.cbMaterial->setCurrentIndex(material_index);
        this->ui.cbType->setCurrentIndex(type_index);
        this->ui.cbVase->setCurrentIndex(vase_index);
        this->ui.cbLamp->setCurrentIndex(lamp_index);
        this->ui.cbFlame->setCurrentIndex(flame_index);

        // Set the save button text
        this->ui.btnSave->setText(this->btn_update_text);
    }
    else {
        // Tomb not found means we are asking to insert a new one
        this->setWindowTitle("Crea nuova");

        // Reset the form fields
        this->ui.leProgressive->setText(QString::number(this->tomb->getLastProgresive() + 1));
        this->ui.cbClient->addItems(client_names);
        this->ui.leName->setText("");
        this->ui.leEngravedNames->setText("");
        this->ui.leEpigraphAmount->setText("1");
        this->ui.rbEngraveYes->setChecked(true);
        this->ui.rbEngraveNo->setChecked(false);
        this->ui.lePrice->setText("0");
        this->ui.chbPaid->setChecked(false);
        this->ui.chbAccessoriesMounted->setChecked(false);
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbType->addItems(tomb_type_names);
        this->ui.cbFormat->addItems(tomb_format_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.cbPitFormatOne->addItems(pit_format_names);
        this->ui.cbPitTypeOne->addItems(pit_type_names);
        this->ui.cbPitFormatTwo->addItems(pit_format_names);
        this->ui.cbPitTypeTwo->addItems(pit_type_names);
        this->ui.cbPitFormatThree->addItems(pit_format_names);
        this->ui.cbPitTypeThree->addItems(pit_type_names);
        this->ui.cbPitFormatFour->addItems(pit_format_names);
        this->ui.cbPitTypeFour->addItems(pit_type_names);
        this->ui.cbPitFormatFive->addItems(pit_format_names);
        this->ui.cbPitTypeFive->addItems(pit_type_names);
        this->ui.cbPitFormatSix->addItems(pit_format_names);
        this->ui.cbPitTypeSix->addItems(pit_type_names);
        this->ui.ptNote->setPlainText("");
        this->ui.leOrderedAt->setText(QDate::currentDate().toString("dd/MM/yyyy"));
        this->ui.leProofedAt->setText(QDate::currentDate().toString("dd/MM/yyyy"));
        this->ui.leConfirmedAt->setText("");
        this->ui.leEngravedAt->setText("");
        this->ui.leDeliveredAt->setText("");

        // Set the save button text
        this->ui.btnSave->setText(this->btn_create_text);
    }

    delete client;
}

QString TombUi::getStyle()
{
    QFile file(this->css_folder + "tombui.css");
    QString css = "";

    if (file.exists()) {
        QString line;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                line = stream.readLine();
                css += line + "\n";
            }
        }
    }
    file.close();

    return css;
}
