#include "TombUi.h"

/********** CONSTRUCTOR **********/

TombUi::TombUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent)
    : db(db), css_folder(css_folder), icons_folder(icons_folder), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon(this->icons_folder + "funeraria.png"));

    // Load the stylesheet for the UI
    QString style = Helpers::getStyle(this->css_folder);

    if (!style.isEmpty()) {
        this->setStyleSheet(style);
    }

    this->progressive = 0;
    this->tomb = new Tomb(this->db);
    this->sculpture = new Sculpture(this->db);
    this->material = new Accessory(this->db, "material");
    this->vase = new Accessory(this->db, "vase");
    this->lamp = new Accessory(this->db, "lamp");
    this->flame = new Accessory(this->db, "flame");
    this->tomb_type = new TombType(this->db);
    this->tomb_format = new TombFormat(this->db);
    this->frame = new Frame(this->db);
    this->pit = new Pit(this->db);
    this->cross = new Cross(this->db);
    this->sacred = new Sacred(this->db);

    // Connect the UI elements
    this->connect(this->ui.cbMaterial, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbType, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbFormat, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbVase, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbLamp, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbFlame, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbCross, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbSacred, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.cbSculpture, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEngraveYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.leScHeight, &QLineEdit::textChanged, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEngraveYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEngraveNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEngraveBronze, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbMountYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbMountNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEpReliefYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbEpReliefNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbInscriptionYes, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.rbInscriptionNo, &QRadioButton::clicked, this, &TombUi::slotUpdateNotes);
    this->connect(this->ui.leEpigraphAmount, &QLineEdit::textChanged, this, &TombUi::slotUpdateNotes);
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

    this->connect(this->ui.rbEngraveYes, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);
    this->connect(this->ui.rbEngraveNo, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);
    this->connect(this->ui.rbEngraveBronze, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);

    this->connect(this->ui.cbSculpture, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateScHightState);

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
    delete this->frame;
    delete this->pit;
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
        if (!this->validateForm("store")) {
            return;
        }

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
            this->pit->getCode(this->ui.cbPitFormatOne->currentText()),
            this->frame->getCode(this->ui.cbPitTypeOne->currentText()),
            this->pit->getCode(this->ui.cbPitFormatTwo->currentText()),
            this->frame->getCode(this->ui.cbPitTypeTwo->currentText()),
            this->pit->getCode(this->ui.cbPitFormatThree->currentText()),
            this->frame->getCode(this->ui.cbPitTypeThree->currentText()),
            this->pit->getCode(this->ui.cbPitFormatFour->currentText()),
            this->frame->getCode(this->ui.cbPitTypeFour->currentText()),
            this->pit->getCode(this->ui.cbPitFormatFive->currentText()),
            this->frame->getCode(this->ui.cbPitTypeFive->currentText()),
            this->pit->getCode(this->ui.cbPitFormatSix->currentText()),
            this->frame->getCode(this->ui.cbPitTypeSix->currentText()),
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
        if (!this->validateForm("update")) {
            return;
        }

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
            this->pit->getCode(this->ui.cbPitFormatOne->currentText()),
            this->frame->getCode(this->ui.cbPitTypeOne->currentText()),
            this->pit->getCode(this->ui.cbPitFormatTwo->currentText()),
            this->frame->getCode(this->ui.cbPitTypeTwo->currentText()),
            this->pit->getCode(this->ui.cbPitFormatThree->currentText()),
            this->frame->getCode(this->ui.cbPitTypeThree->currentText()),
            this->pit->getCode(this->ui.cbPitFormatFour->currentText()),
            this->frame->getCode(this->ui.cbPitTypeFour->currentText()),
            this->pit->getCode(this->ui.cbPitFormatFive->currentText()),
            this->frame->getCode(this->ui.cbPitTypeFive->currentText()),
            this->pit->getCode(this->ui.cbPitFormatSix->currentText()),
            this->frame->getCode(this->ui.cbPitTypeSix->currentText()),
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
    // Reset the text area content
    this->ui.ptNote->setPlainText("");
    QString notes = "";

    if (this->ui.rbMountYes->isChecked()) notes += "Montaggio, ";
    /*
    if (this->ui.cbType->currentIndex() != 0) notes += this->ui.cbType->currentText() + ", ";
    if (this->ui.cbFormat->currentIndex() != 0) notes += this->ui.cbFormat->currentText() + ", ";
    */
    if (this->ui.rbEngraveYes->isChecked()) {
        QString epigrafi = "epigrafe";
        if (this->ui.leEpigraphAmount->text().toInt() > 1) epigrafi = "epigrafi";
        if (this->ui.rbEpReliefYes->isChecked()) epigrafi += " a rilievo";
        notes += this->ui.leEpigraphAmount->text() + " " + epigrafi + ", ";
    }
    else if (this->ui.rbEngraveNo->isChecked()) notes += "Solo materiale, ";
    else if (this->ui.rbEngraveBronze->isChecked()) notes += "Bronzi applicati, ";

    if (this->ui.rbInscriptionYes->isChecked()) notes += " dedica, ";

    if (this->ui.cbPitFormatSix->currentIndex() != 0) notes += "6 scavi, ";
    else if (this->ui.cbPitFormatFive->currentIndex() != 0) notes += "5 scavi, ";
    else if (this->ui.cbPitFormatFour->currentIndex() != 0) notes += "4 scavi, ";
    else if (this->ui.cbPitFormatThree->currentIndex() != 0) notes += "3 scavi, ";
    else if (this->ui.cbPitFormatTwo->currentIndex() != 0) notes += "2 scavi, ";
    else if (this->ui.cbPitFormatOne->currentIndex() != 0) notes += "1 scavo, ";

    if (this->ui.cbCross->currentIndex() != 0) notes += "croce, ";

    if (this->ui.cbSacred->currentIndex() != 0) notes += "imm. serigrafata, ";

    if (this->ui.cbSculpture->currentIndex() != 0) {
        notes += "scultura ";
        if (this->ui.leScHeight->text().toInt() > 20) {
            notes += this->ui.leScHeight->text() + "cm, ";
        }
        else notes += ", ";
    }

    // Remove the last 2 characters ", "
    notes.chop(2);

    this->ui.ptNote->setPlainText(notes);
}

void TombUi::slotUpdateEpNumState()
{
    if (this->ui.rbEngraveYes->isChecked()) {
        this->ui.leEpigraphAmount->setEnabled(true);
        this->ui.leEpigraphAmount->setText("1");
    }
    else {
        this->ui.leEpigraphAmount->setEnabled(false);
        this->ui.leEpigraphAmount->setText("0");
    }
}

void TombUi::slotUpdatePitState()
{
    if (this->ui.cbPitFormatOne->currentIndex() != 0) {
        this->ui.cbPitFormatTwo->setEnabled(true);
        this->ui.cbPitTypeTwo->setEnabled(true);
    }
    else {
        this->ui.cbPitFormatTwo->setEnabled(false);
        this->ui.cbPitFormatTwo->setCurrentIndex(0);
        this->ui.cbPitTypeTwo->setEnabled(false);
        this->ui.cbPitTypeTwo->setCurrentIndex(0);
    }

    if (this->ui.cbPitFormatTwo->currentIndex() != 0) {
        this->ui.cbPitFormatThree->setEnabled(true);
        this->ui.cbPitTypeThree->setEnabled(true);
    }
    else {
        this->ui.cbPitTypeTwo->setCurrentIndex(0);
        this->ui.cbPitFormatThree->setEnabled(false);
        this->ui.cbPitFormatThree->setCurrentIndex(0);
        this->ui.cbPitTypeThree->setEnabled(false);
        this->ui.cbPitTypeThree->setCurrentIndex(0);
    }

    if (this->ui.cbPitFormatThree->currentIndex() != 0) {
        this->ui.cbPitFormatFour->setEnabled(true);
        this->ui.cbPitTypeFour->setEnabled(true);
    }
    else {
        this->ui.cbPitTypeThree->setCurrentIndex(0);
        this->ui.cbPitFormatFour->setEnabled(false);
        this->ui.cbPitFormatFour->setCurrentIndex(0);
        this->ui.cbPitTypeFour->setEnabled(false);
        this->ui.cbPitTypeFour->setCurrentIndex(0);
    }

    if (this->ui.cbPitFormatFour->currentIndex() != 0) {
        this->ui.cbPitFormatFive->setEnabled(true);
        this->ui.cbPitTypeFive->setEnabled(true);
    }
    else {
        this->ui.cbPitTypeFour->setCurrentIndex(0);
        this->ui.cbPitFormatFive->setEnabled(false);
        this->ui.cbPitFormatFive->setCurrentIndex(0);
        this->ui.cbPitTypeFive->setEnabled(false);
        this->ui.cbPitTypeFive->setCurrentIndex(0);
    }

    if (this->ui.cbPitFormatFive->currentIndex() != 0) {
        this->ui.cbPitFormatSix->setEnabled(true);
        this->ui.cbPitTypeSix->setEnabled(true);
    }
    else {
        this->ui.cbPitTypeFive->setCurrentIndex(0);
        this->ui.cbPitFormatSix->setEnabled(false);
        this->ui.cbPitFormatSix->setCurrentIndex(0);
        this->ui.cbPitTypeSix->setEnabled(false);
        this->ui.cbPitTypeSix->setCurrentIndex(0);
    }
}

void TombUi::slotUpdateScHightState()
{
    if (this->ui.cbSculpture->currentIndex() != 0) {
        this->ui.leScHeight->setEnabled(true);
        this->ui.leScHeight->setText("20");
    }
    else {
        this->ui.leScHeight->setEnabled(false);
        this->ui.leScHeight->setText("");
    }
}

/********** PRIVATE FUNCTIONS **********/

bool TombUi::validateForm(const QString& op)
{
    int current_last = this->tomb->getLastProgresive();

    if (op == "store") {
        if (this->tomb->isProgressiveInUse(this->ui.leProgressive->text().toInt())) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("Il numero assegnato alla lapide è già in uso");
            message.exec();
            return false;
        }

    }
    else {
        // If the progressive has changed and the new number is already in use
        if (this->progressive != this->ui.leProgressive->text().toInt() && 
            this->tomb->isProgressiveInUse(this->ui.leProgressive->text().toInt())) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("Il numero assegnato alla lapide è già in uso");
            message.exec();
            return false;
        }
    }

    if (this->ui.leProgressive->text().toInt() > current_last + 1) {
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

    if (this->sculpture->getCode(this->ui.cbSculpture->currentText()) != "NO" &&
        (this->ui.leScHeight->text().trimmed() == "" || this->ui.leScHeight->text().toInt() <= 0)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'altezza della scultura non è corretta");
        message.exec();
        return false;
    }

    if (this->ui.rbEngraveYes->isChecked() &&
        (this->ui.leEpigraphAmount->text().trimmed() == "" || this->ui.leEpigraphAmount->text().toInt() <= 0)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero di epigrafi non è corretto");
        message.exec();
        return false;
    }

    if (
        (
            this->pit->getCode(this->ui.cbPitFormatTwo->currentText()) != "NO" && 
			this->pit->getCode(this->ui.cbPitFormatOne->currentText()) == "NO"
        )
        ||
        (this->pit->getCode(this->ui.cbPitFormatThree->currentText()) != "NO" &&
            (
                this->pit->getCode(this->ui.cbPitFormatTwo->currentText()) == "NO" || 
                this->pit->getCode(this->ui.cbPitFormatOne->currentText()) == "NO"
            )
        ) 
        ||
        (this->pit->getCode(this->ui.cbPitFormatFour->currentText()) != "NO" &&
            (
                this->pit->getCode(this->ui.cbPitFormatThree->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatTwo->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatOne->currentText()) == "NO"
            )
        ) 
        ||
        (this->pit->getCode(this->ui.cbPitFormatFive->currentText()) != "NO" &&
            (
                this->pit->getCode(this->ui.cbPitFormatFour->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatThree->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatTwo->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatOne->currentText()) == "NO"
            )
        ) 
        ||
        (this->pit->getCode(this->ui.cbPitFormatSix->currentText()) != "NO" &&
            (
                this->pit->getCode(this->ui.cbPitFormatFive->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatFour->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatThree->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatTwo->currentText()) == "NO" ||
                this->pit->getCode(this->ui.cbPitFormatOne->currentText()) == "NO"
            )
        )
    ) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Gli scavi devono essere selezionati in sequenza senza saltare posizioni");
        message.exec();
        return false;
    }

    if (!this->checkDates(
        this->ui.leOrderedAt->text(), 
        this->ui.leProofedAt->text(), 
        this->ui.leConfirmedAt->text(),
        this->ui.leEngravedAt->text(),
        this->ui.leDeliveredAt->text())
    ) {
        return false;
    }

    return true;
}

bool TombUi::checkDates(const QString& order, const QString& proof, const QString& confirmation, const QString& engraving, const QString& delivery)
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

void TombUi::updateForm()
{
    Client* client = new Client(this->db);
    // Get the selected tomb's data
    QMap<QString, QString> tomb_details = this->tomb->getDetails(this->progressive);

    QList<QMap<QString, QString>> clients = client->get();
    QList<QMap<QString, QString>> materials = this->material->get();
    QList<QMap<QString, QString>> tomb_types = this->tomb_type->get();
    QList<QMap<QString, QString>> tomb_formats = this->tomb_format->get();
    QList<QMap<QString, QString>> vases = this->vase->get();
    QList<QMap<QString, QString>> lamps = this->lamp->get();
    QList<QMap<QString, QString>> flames = this->flame->get();
    QList<QMap<QString, QString>> crosses = this->cross->getListByCode();
    QList<QMap<QString, QString>> sacreds = this->sacred->get();
    QList<QMap<QString, QString>> sculptures = this->sculpture->getListByCode();
    QList<QMap<QString, QString>> frames = this->frame->get();
    QList<QMap<QString, QString>> pits = this->pit->get();

    int client_index = 0;
    int material_index = 0;
    int type_index = 0;
    int format_index = 0;
    int vase_index = 0;
    int lamp_index = 0;
    int flame_index = 0;
    int cross_index = 0;
    int sacred_index = 0;
    int sculpture_index = 0;
    int pf1_index = 0;
    int pt1_index = 0;
    int pf2_index = 0;
    int pt2_index = 0;
    int pf3_index = 0;
    int pt3_index = 0;
    int pf4_index = 0;
    int pt4_index = 0;
    int pf5_index = 0;
    int pt5_index = 0;
    int pf6_index = 0;
    int pt6_index = 0;

    QList<QString> client_names = client->getActiveNames();
    QList<QString> material_names = this->material->getNames();
    QList<QString> tomb_type_names = this->tomb_type->getNames();
    QList<QString> tomb_format_names = this->tomb_format->getNames();
    QList<QString> vase_names = this->vase->getNames();
    QList<QString> lamp_names = this->lamp->getNames();
    QList<QString> flame_names = this->flame->getNames();
    QList<QString> cross_names = this->cross->getNames();
    QList<QString> sacred_names = this->sacred->getNames();
    QList<QString> sculpture_names = this->sculpture->getNames();
    QList<QString> pit_names = this->pit->getNames();
    QList<QString> frame_names = this->frame->getNames();

    // Clear the combo boxes
    this->ui.cbClient->clear();
    this->ui.cbMaterial->clear();
    this->ui.cbType->clear();
    this->ui.cbFormat->clear();
    this->ui.cbVase->clear();
    this->ui.cbLamp->clear();
    this->ui.cbFlame->clear();
    this->ui.cbCross->clear();
    this->ui.cbSacred->clear();
    this->ui.cbSculpture->clear();
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
        // Tomb found means we are asking to update an existing one
        this->setWindowTitle("Modifica lapide");

        /*********** Get the index to select for the comboboxes *************/
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i]["id"] == tomb_details["client_id"]) {
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

        for (int i = 0; i < tomb_formats.size(); i++) {
            if (tomb_formats[i]["code"] == tomb_details["format_code"]) {
                format_index = i;
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

        for (int i = 0; i < crosses.size(); i++) {
            if (crosses[i]["code"] == tomb_details["cross_code"]) {
                cross_index = i;
                break;
            }
        }

        for (int i = 0; i < sacreds.size(); i++) {
            if (sacreds[i]["code"] == tomb_details["sacred_code"]) {
                sacred_index = i;
                break;
            }
        }

        for (int i = 0; i < sculptures.size(); i++) {
            if (sculptures[i]["code"] == tomb_details["sculpture_code"]) {
                sculpture_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_one"]) {
                pf1_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_one"]) {
                pt1_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_two"]) {
                pf2_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_two"]) {
                pt2_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_three"]) {
                pf3_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_three"]) {
                pt3_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_four"]) {
                pf4_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_four"]) {
                pt4_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_five"]) {
                pf5_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_five"]) {
                pt5_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_six"]) {
                pf6_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_six"]) {
                pt6_index = i;
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
        this->ui.cbCross->addItems(cross_names);
        this->ui.cbSacred->addItems(sacred_names);
        this->ui.cbSculpture->addItems(sculpture_names);
        this->ui.cbPitFormatOne->addItems(pit_names);
        this->ui.cbPitTypeOne->addItems(frame_names);
        this->ui.cbPitFormatTwo->addItems(pit_names);
        this->ui.cbPitTypeTwo->addItems(frame_names);
        this->ui.cbPitFormatThree->addItems(pit_names);
        this->ui.cbPitTypeThree->addItems(frame_names);
        this->ui.cbPitFormatFour->addItems(pit_names);
        this->ui.cbPitTypeFour->addItems(frame_names);
        this->ui.cbPitFormatFive->addItems(pit_names);
        this->ui.cbPitTypeFive->addItems(frame_names);
        this->ui.cbPitFormatSix->addItems(pit_names);
        this->ui.cbPitTypeSix->addItems(frame_names);
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
        this->ui.cbFormat->setCurrentIndex(format_index);
        this->ui.cbVase->setCurrentIndex(vase_index);
        this->ui.cbLamp->setCurrentIndex(lamp_index);
        this->ui.cbFlame->setCurrentIndex(flame_index);
        this->ui.cbCross->setCurrentIndex(cross_index);
        this->ui.cbSacred->setCurrentIndex(sacred_index);
        this->ui.cbSculpture->setCurrentIndex(sculpture_index);
        this->ui.cbPitFormatOne->setCurrentIndex(pf1_index);
        this->ui.cbPitTypeOne->setCurrentIndex(pt1_index);
        this->ui.cbPitFormatTwo->setCurrentIndex(pf2_index);
        this->ui.cbPitTypeTwo->setCurrentIndex(pt2_index);
        this->ui.cbPitFormatThree->setCurrentIndex(pf3_index);
        this->ui.cbPitTypeThree->setCurrentIndex(pt3_index);
        this->ui.cbPitFormatFour->setCurrentIndex(pf4_index);
        this->ui.cbPitTypeFour->setCurrentIndex(pt4_index);
        this->ui.cbPitFormatFive->setCurrentIndex(pf5_index);
        this->ui.cbPitTypeFive->setCurrentIndex(pt5_index);
        this->ui.cbPitFormatSix->setCurrentIndex(pf6_index);
        this->ui.cbPitTypeSix->setCurrentIndex(pt6_index);

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
        this->ui.cbCross->addItems(cross_names);
        this->ui.cbSacred->addItems(sacred_names);
        this->ui.cbSculpture->addItems(sculpture_names);
        this->ui.cbPitFormatOne->addItems(pit_names);
        this->ui.cbPitTypeOne->addItems(frame_names);
        this->ui.cbPitFormatTwo->addItems(pit_names);
        this->ui.cbPitTypeTwo->addItems(frame_names);
        this->ui.cbPitFormatThree->addItems(pit_names);
        this->ui.cbPitTypeThree->addItems(frame_names);
        this->ui.cbPitFormatFour->addItems(pit_names);
        this->ui.cbPitTypeFour->addItems(frame_names);
        this->ui.cbPitFormatFive->addItems(pit_names);
        this->ui.cbPitTypeFive->addItems(frame_names);
        this->ui.cbPitFormatSix->addItems(pit_names);
        this->ui.cbPitTypeSix->addItems(frame_names);
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
