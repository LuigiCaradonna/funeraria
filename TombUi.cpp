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
    this->frame = new Accessory(this->db, "frame");
    this->pit = new Pit(this->db);
    this->cross = new Cross(this->db);
    this->drawing = new Drawing(this->db);

    // Connect the UI elements
    this->connect(this->ui.rbEngraveYes, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);
    this->connect(this->ui.rbEngraveNo, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);
    this->connect(this->ui.rbEngraveBronze, &QRadioButton::clicked, this, &TombUi::slotUpdateEpNumState);

    this->connect(this->ui.cbSculpture, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdateScHightState);

    this->connect(this->ui.cbPitOne, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitTwo, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitThree, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFour, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitFive, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);
    this->connect(this->ui.cbPitSix, &QComboBox::currentIndexChanged, this, &TombUi::slotUpdatePitState);

    this->connect(this->ui.btnCompNotes, &QPushButton::clicked, this, &TombUi::slotUpdateNotes);
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
    delete this->drawing;
    delete this->currentDateMapper;
}

/********** PUBLIC FUNCTIONS **********/

void TombUi::setProgressive(const int& progressive)
{
    this->progressive = progressive;
    this->updateForm();
}

bool TombUi::doReload()
{
    return this->ui.cbReload->isChecked();
}

/********** PROTECTED SLOTS **********/

void TombUi::slotCloseDialog()
{
    this->close();
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
            this->ui.leDeposit->text().toDouble(),
            this->ui.chbPaid->isChecked(),
            this->material->getCode(this->ui.cbMaterial->currentText()),
            this->tomb_type->getCode(this->ui.cbType->currentText()),
            this->tomb_format->getCode(this->ui.cbFormat->currentText()),
            this->vase->getCode(this->ui.cbVase->currentText()),
            this->lamp->getCode(this->ui.cbLamp->currentText()),
            this->flame->getCode(this->ui.cbFlame->currentText()),
            this->cross->getCode(this->ui.cbCross->currentText()),
            this->drawing->getCode(this->ui.cbDrawing->currentText()),
            this->sculpture->getCode(this->ui.cbSculpture->currentText()),
            this->ui.leScHeight->text().toFloat(),
            this->ui.rbMountYes->isChecked(),
            this->ui.rbMProvYes->isChecked(),
            this->ui.rbEpReliefYes->isChecked(),
            this->ui.rbInscriptionYes->isChecked(),
            this->pit->getCode(this->ui.cbPitOne->currentText()),
            this->frame->getCode(this->ui.cbFrameOne->currentText()),
            this->pit->getCode(this->ui.cbPitTwo->currentText()),
            this->frame->getCode(this->ui.cbFrameTwo->currentText()),
            this->pit->getCode(this->ui.cbPitThree->currentText()),
            this->frame->getCode(this->ui.cbFrameThree->currentText()),
            this->pit->getCode(this->ui.cbPitFour->currentText()),
            this->frame->getCode(this->ui.cbFrameFour->currentText()),
            this->pit->getCode(this->ui.cbPitFive->currentText()),
            this->frame->getCode(this->ui.cbFrameFive->currentText()),
            this->pit->getCode(this->ui.cbPitSix->currentText()),
            this->frame->getCode(this->ui.cbFrameSix->currentText()),
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
            this->ui.leDeposit->text().toDouble(),
            this->ui.chbPaid->isChecked(),
            this->material->getCode(this->ui.cbMaterial->currentText()),
            this->tomb_type->getCode(this->ui.cbType->currentText()),
            this->tomb_format->getCode(this->ui.cbFormat->currentText()),
            this->vase->getCode(this->ui.cbVase->currentText()),
            this->lamp->getCode(this->ui.cbLamp->currentText()),
            this->flame->getCode(this->ui.cbFlame->currentText()),
            this->cross->getCode(this->ui.cbCross->currentText()),
            this->drawing->getCode(this->ui.cbDrawing->currentText()),
            this->sculpture->getCode(this->ui.cbSculpture->currentText()),
            this->ui.leScHeight->text().toFloat(),
            this->ui.rbMountYes->isChecked(),
            this->ui.rbMProvYes->isChecked(),
            this->ui.rbEpReliefYes->isChecked(),
            this->ui.rbInscriptionYes->isChecked(),
            this->pit->getCode(this->ui.cbPitOne->currentText()),
            this->frame->getCode(this->ui.cbFrameOne->currentText()),
            this->pit->getCode(this->ui.cbPitTwo->currentText()),
            this->frame->getCode(this->ui.cbFrameTwo->currentText()),
            this->pit->getCode(this->ui.cbPitThree->currentText()),
            this->frame->getCode(this->ui.cbFrameThree->currentText()),
            this->pit->getCode(this->ui.cbPitFour->currentText()),
            this->frame->getCode(this->ui.cbFrameFour->currentText()),
            this->pit->getCode(this->ui.cbPitFive->currentText()),
            this->frame->getCode(this->ui.cbFrameFive->currentText()),
            this->pit->getCode(this->ui.cbPitSix->currentText()),
            this->frame->getCode(this->ui.cbFrameSix->currentText()),
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

void TombUi::slotUpdateNotes()
{
    // Reset the text area content
    this->ui.ptNote->setPlainText("");
    QString notes = "";

    if (this->ui.rbMProvYes->isChecked()) notes += "Materiale fornito - ";

    if (this->ui.rbMountYes->isChecked()) notes += "Assemblaggio - ";
    
    if (this->ui.rbEngraveYes->isChecked()) {
        // A tomb can be engraved without an epigraph
        if (this->ui.leEpigraphAmount->text().toInt() > 0) {
            QString epigrafi = "Epigrafe";
            if (this->ui.leEpigraphAmount->text().toInt() > 1) epigrafi = this->ui.leEpigraphAmount->text() + " " + "epigrafi";
            if (this->ui.rbEpReliefYes->isChecked()) epigrafi += " a rilievo";
            notes += epigrafi + " - ";
        }
    }
    else if (this->ui.rbEngraveNo->isChecked()) notes += "Solo materiale - ";
    else if (this->ui.rbEngraveBronze->isChecked()) notes += "Bronzi applicati - ";

    if (this->ui.rbInscriptionYes->isChecked()) notes += "Dedica - ";

    if (this->ui.cbPitSix->currentIndex() != 0) {
        if (this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameTwo->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameThree->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameFour->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameFive->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameSix->currentIndex()) {

            if (this->frame->getCode(this->ui.cbFrameOne->currentText()) == "NO") {
                notes += "6 scavi senza cornice - ";
            }
            else {
                notes += "6 scavi cornice " + this->ui.cbFrameOne->currentText().toLower() + " - ";
            }
        }
        else {
            notes += "6 scavi cornici varie - ";
        }
    }
    else if (this->ui.cbPitFive->currentIndex() != 0) {
        if (this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameTwo->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameThree->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameFour->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameFive->currentIndex()) {

            if (this->frame->getCode(this->ui.cbFrameOne->currentText()) == "NO") {
                notes += "5 scavi senza cornice - ";
            }
            else {
                notes += "5 scavi cornice " + this->ui.cbFrameOne->currentText().toLower() + " - ";
            }
        }
        else {
            notes += "5 scavi cornici varie - ";
        }
    }
    else if (this->ui.cbPitFour->currentIndex() != 0) {
        if (this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameTwo->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameThree->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameFour->currentIndex()) {

            if (this->frame->getCode(this->ui.cbFrameOne->currentText()) == "NO") {
                notes += "4 scavi senza cornice - ";
            }
            else {
                notes += "4 scavi cornice " + this->ui.cbFrameOne->currentText().toLower() + " - ";
            }
        }
        else {
            notes += "4 scavi cornici varie - ";
        }
    }
    else if (this->ui.cbPitThree->currentIndex() != 0) {
        if (this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameTwo->currentIndex() &&
            this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameThree->currentIndex()) {

            if (this->frame->getCode(this->ui.cbFrameOne->currentText()) == "NO") {
                notes += "3 scavi senza cornice - ";
            }
            else {
                notes += "3 scavi cornice " + this->ui.cbFrameOne->currentText().toLower() + " - ";
            }
        }
        else {
            notes += "3 scavi cornici varie - ";
        }
    }
    else if (this->ui.cbPitTwo->currentIndex() != 0) {
        if (this->ui.cbFrameOne->currentIndex() == this->ui.cbFrameTwo->currentIndex()) {

            if (this->frame->getCode(this->ui.cbFrameOne->currentText()) == "NO") {
                notes += "2 scavi senza cornice - ";
            }
            else {
                notes += "2 scavi cornice " + this->ui.cbFrameOne->currentText().toLower() + " - ";
            }
        }
        else {
            notes += "2 scavi cornici varie - ";
        }
    }
    else if (this->ui.cbFrameOne->currentIndex() != 0 && this->ui.cbPitOne->currentIndex() != 0) {

        if (this->frame->getCode(this->ui.cbFrameOne->currentText()) == "NO") {
            notes += "Scavo senza cornice - ";
        }
        else {
            notes += "Scavo cornice " + this->ui.cbFrameOne->currentText().toLower() + " - ";
        }
    }

    if (this->ui.cbCross->currentIndex() != 0) notes += "Croce - ";

    if (this->ui.cbDrawing->currentIndex() != 0) notes += "Imm. serigrafata - ";

    if (this->ui.cbSculpture->currentIndex() != 0) {
        notes += "Scultura";
        if (this->ui.leScHeight->text().toFloat() > 20) {
            notes += " " + this->ui.leScHeight->text() + "cm - ";
        }
        else notes += " - ";
    }

    // Remove the last 3 characters " - "
    notes.chop(3);

    this->ui.ptNote->setPlainText(notes);
}

void TombUi::slotUpdatePitState()
{
    if (this->ui.cbPitOne->currentIndex() != 0) {
        this->ui.cbPitTwo->setEnabled(true);
        this->ui.cbFrameTwo->setEnabled(true);
    }
    else {
        this->ui.cbPitTwo->setEnabled(false);
        this->ui.cbPitTwo->setCurrentIndex(0);
        this->ui.cbFrameTwo->setEnabled(false);
        this->ui.cbFrameTwo->setCurrentIndex(0);
    }

    if (this->ui.cbPitTwo->currentIndex() != 0) {
        this->ui.cbPitThree->setEnabled(true);
        this->ui.cbFrameThree->setEnabled(true);
    }
    else {
        this->ui.cbFrameTwo->setCurrentIndex(0);
        this->ui.cbPitThree->setEnabled(false);
        this->ui.cbPitThree->setCurrentIndex(0);
        this->ui.cbFrameThree->setEnabled(false);
        this->ui.cbFrameThree->setCurrentIndex(0);
    }

    if (this->ui.cbPitThree->currentIndex() != 0) {
        this->ui.cbPitFour->setEnabled(true);
        this->ui.cbFrameFour->setEnabled(true);
    }
    else {
        this->ui.cbFrameThree->setCurrentIndex(0);
        this->ui.cbPitFour->setEnabled(false);
        this->ui.cbPitFour->setCurrentIndex(0);
        this->ui.cbFrameFour->setEnabled(false);
        this->ui.cbFrameFour->setCurrentIndex(0);
    }

    if (this->ui.cbPitFour->currentIndex() != 0) {
        this->ui.cbPitFive->setEnabled(true);
        this->ui.cbFrameFive->setEnabled(true);
    }
    else {
        this->ui.cbFrameFour->setCurrentIndex(0);
        this->ui.cbPitFive->setEnabled(false);
        this->ui.cbPitFive->setCurrentIndex(0);
        this->ui.cbFrameFive->setEnabled(false);
        this->ui.cbFrameFive->setCurrentIndex(0);
    }

    if (this->ui.cbPitFive->currentIndex() != 0) {
        this->ui.cbPitSix->setEnabled(true);
        this->ui.cbFrameSix->setEnabled(true);
    }
    else {
        this->ui.cbFrameFive->setCurrentIndex(0);
        this->ui.cbPitSix->setEnabled(false);
        this->ui.cbPitSix->setCurrentIndex(0);
        this->ui.cbFrameSix->setEnabled(false);
        this->ui.cbFrameSix->setCurrentIndex(0);
    }
}

void TombUi::slotUpdateScHightState()
{
    if (this->ui.cbSculpture->currentIndex() != 0) {
        this->ui.leScHeight->setEnabled(true);
    }
    else {
        this->ui.leScHeight->setEnabled(false);
        this->ui.leScHeight->setText("");
    }
}

/********** PRIVATE FUNCTIONS **********/

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

void TombUi::resetForm()
{
    this->ui.cbClient->setCurrentIndex(0);
    this->ui.leName->setText("");
    this->ui.leEngravedNames->setText("");
    this->ui.lePrice->setText("");
    this->ui.chbPaid->setChecked(false);
    this->ui.chbAccessoriesMounted->setChecked(false);
    this->ui.cbMaterial->setCurrentIndex(0);
    this->ui.cbType->setCurrentIndex(0);
    this->ui.cbFormat->setCurrentIndex(0);
    this->ui.cbVase->setCurrentIndex(0);
    this->ui.cbLamp->setCurrentIndex(0);
    this->ui.cbFlame->setCurrentIndex(0);
    this->ui.cbCross->setCurrentIndex(0);
    this->ui.cbDrawing->setCurrentIndex(0);
    this->ui.cbSculpture->setCurrentIndex(0);
    this->ui.leScHeight->setText("");
    this->ui.leScHeight->setEnabled(false);
    this->ui.rbEngraveYes->setChecked(true);
    this->ui.rbMountYes->setChecked(true);
    this->ui.rbMProvNo->setChecked(true);
    this->ui.rbEpReliefNo->setChecked(true);
    this->ui.rbInscriptionNo->setChecked(true);
    this->ui.leEpigraphAmount->setText("1");
    this->ui.leEpigraphAmount->setEnabled(true);
    this->ui.cbPitOne->setCurrentIndex(0);
    this->ui.cbFrameOne->setCurrentIndex(0);
    this->ui.cbPitTwo->setCurrentIndex(0);
    this->ui.cbFrameTwo->setCurrentIndex(0);
    this->ui.cbPitThree->setCurrentIndex(0);
    this->ui.cbFrameThree->setCurrentIndex(0);
    this->ui.cbPitFour->setCurrentIndex(0);
    this->ui.cbFrameFour->setCurrentIndex(0);
    this->ui.cbPitFive->setCurrentIndex(0);
    this->ui.cbFrameFive->setCurrentIndex(0);
    this->ui.cbPitSix->setCurrentIndex(0);
    this->ui.cbFrameSix->setCurrentIndex(0);
    this->ui.ptNote->setPlainText("");
    this->ui.leOrderedAt->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    this->ui.leProofedAt->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    this->ui.leConfirmedAt->setText("");
    this->ui.leEngravedAt->setText("");
    this->ui.leDeliveredAt->setText("");
    this->ui.cbReload->setChecked(true);
}

void TombUi::updateForm()
{
    Client* client = new Client(this->db);
    // Get the selected tomb's data
    QMap<QString, QString> tomb_details = this->tomb->getDetails(this->progressive);

    QList<QMap<QString, QString>> clients = client->getActive();
    QList<QMap<QString, QString>> materials = this->material->get();
    QList<QMap<QString, QString>> tomb_types = this->tomb_type->get();
    QList<QMap<QString, QString>> tomb_formats = this->tomb_format->get();
    QList<QMap<QString, QString>> vases = this->vase->get();
    QList<QMap<QString, QString>> lamps = this->lamp->get();
    QList<QMap<QString, QString>> flames = this->flame->get();
    QList<QMap<QString, QString>> crosses = this->cross->getListByCode();
    QList<QMap<QString, QString>> drawings = this->drawing->getListByCode();
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
    int drawing_index = 0;
    int sculpture_index = 0;
    int pit1_index = 0;
    int frame1_index = 0;
    int pit2_index = 0;
    int frame2_index = 0;
    int pit3_index = 0;
    int frame3_index = 0;
    int pit4_index = 0;
    int frame4_index = 0;
    int pit5_index = 0;
    int frame5_index = 0;
    int pit6_index = 0;
    int frame6_index = 0;

    QList<QString> client_names = client->getActiveNames();
    QList<QString> material_names = this->material->getNames();
    QList<QString> tomb_type_names = this->tomb_type->getNames();
    QList<QString> tomb_format_names = this->tomb_format->getNames();
    QList<QString> vase_names = this->vase->getNames();
    QList<QString> lamp_names = this->lamp->getNames();
    QList<QString> flame_names = this->flame->getNames();
    QList<QString> cross_names = this->cross->getNames();
    QList<QString> drawing_names = this->drawing->getNames();
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
    this->ui.cbDrawing->clear();
    this->ui.cbSculpture->clear();
    this->ui.cbPitOne->clear();
    this->ui.cbFrameOne->clear();
    this->ui.cbPitTwo->clear();
    this->ui.cbFrameTwo->clear();
    this->ui.cbPitThree->clear();
    this->ui.cbFrameThree->clear();
    this->ui.cbPitFour->clear();
    this->ui.cbFrameFour->clear();
    this->ui.cbPitFive->clear();
    this->ui.cbFrameFive->clear();
    this->ui.cbPitSix->clear();
    this->ui.cbFrameSix->clear();

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

        for (int i = 0; i < drawings.size(); i++) {
            if (drawings[i]["code"] == tomb_details["drawing_code"]) {
                drawing_index = i;
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
                pit1_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_one"]) {
                frame1_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_two"]) {
                pit2_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_two"]) {
                frame2_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_three"]) {
                pit3_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_three"]) {
                frame3_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_four"]) {
                pit4_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_four"]) {
                frame4_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_five"]) {
                pit5_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_five"]) {
                frame5_index = i;
                break;
            }
        }

        for (int i = 0; i < pits.size(); i++) {
            if (pits[i]["code"] == tomb_details["pit_six"]) {
                pit6_index = i;
                break;
            }
        }

        for (int i = 0; i < frames.size(); i++) {
            if (frames[i]["code"] == tomb_details["frame_six"]) {
                frame6_index = i;
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

        // Fill the comboboxes
        this->ui.cbClient->addItems(client_names);
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbType->addItems(tomb_type_names);
        this->ui.cbFormat->addItems(tomb_format_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.cbCross->addItems(cross_names);
        this->ui.cbDrawing->addItems(drawing_names);
        this->ui.cbSculpture->addItems(sculpture_names);
        this->ui.cbPitOne->addItems(pit_names);
        this->ui.cbFrameOne->addItems(frame_names);
        this->ui.cbPitTwo->addItems(pit_names);
        this->ui.cbFrameTwo->addItems(frame_names);
        this->ui.cbPitThree->addItems(pit_names);
        this->ui.cbFrameThree->addItems(frame_names);
        this->ui.cbPitFour->addItems(pit_names);
        this->ui.cbFrameFour->addItems(frame_names);
        this->ui.cbPitFive->addItems(pit_names);
        this->ui.cbFrameFive->addItems(frame_names);
        this->ui.cbPitSix->addItems(pit_names);
        this->ui.cbFrameSix->addItems(frame_names);

        // Fill the form fields with the selected tomb's data
        this->ui.leProgressive->setText(tomb_details["progressive"]);
        this->ui.leName->setText(tomb_details["name"]);
        this->ui.leEngravedNames->setText(tomb_details["engraved_names"]);
        this->ui.leEpigraphAmount->setText(tomb_details["ep_amount"]);
        this->ui.leScHeight->setText(tomb_details["sculpture_h"]);
        this->ui.lePrice->setText(tomb_details["price"]);
        this->ui.leDeposit->setText(tomb_details["deposit"]);
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

        // Set fields' enabled state
        if (tomb_details["sculpture_h"].toFloat() != 0) {
            this->ui.leScHeight->setEnabled(true);
        }
        else {
            this->ui.leScHeight->setEnabled(false);
        }

        if (tomb_details["engraved"] == "1") {
            this->ui.leEpigraphAmount->setEnabled(true);
        }
        else {
            this->ui.leEpigraphAmount->setEnabled(false);
        }

        // Set radio buttons selection
        this->ui.rbEngraveYes->setChecked(tomb_details["engraved"] == "1");
        this->ui.rbEngraveNo->setChecked(tomb_details["engraved"] == "0");
        this->ui.rbEngraveBronze->setChecked(tomb_details["engraved"] == "2");
        this->ui.rbMountYes->setChecked(tomb_details["mounted"] == "1");
        this->ui.rbMountNo->setChecked(tomb_details["mounted"] == "0");
        this->ui.rbMProvYes->setChecked(tomb_details["mat_provided"] == "1");
        this->ui.rbMProvNo->setChecked(tomb_details["mat_provided"] == "0");
        this->ui.rbEpReliefYes->setChecked(tomb_details["ep_relief"] == "1");
        this->ui.rbEpReliefNo->setChecked(tomb_details["ep_relief"] == "0");
        this->ui.rbInscriptionYes->setChecked(tomb_details["inscription"] == "1");
        this->ui.rbInscriptionNo->setChecked(tomb_details["inscription"] == "0");

        // Set checkboxes selection
        this->ui.chbPaid->setChecked(tomb_details["paid"] == "1");
        this->ui.chbAccessoriesMounted->setChecked(tomb_details["accessories_mounted"] == "1");
        
        // Set the item to show inside the comboboxes
        this->ui.cbClient->setCurrentIndex(client_index);
        this->ui.cbMaterial->setCurrentIndex(material_index);
        this->ui.cbType->setCurrentIndex(type_index);
        this->ui.cbFormat->setCurrentIndex(format_index);
        this->ui.cbVase->setCurrentIndex(vase_index);
        this->ui.cbLamp->setCurrentIndex(lamp_index);
        this->ui.cbFlame->setCurrentIndex(flame_index);
        this->ui.cbCross->setCurrentIndex(cross_index);
        this->ui.cbDrawing->setCurrentIndex(drawing_index);
        this->ui.cbSculpture->setCurrentIndex(sculpture_index);
        this->ui.cbPitOne->setCurrentIndex(pit1_index);
        this->ui.cbFrameOne->setCurrentIndex(frame1_index);
        this->ui.cbPitTwo->setCurrentIndex(pit2_index);
        this->ui.cbFrameTwo->setCurrentIndex(frame2_index);
        this->ui.cbPitThree->setCurrentIndex(pit3_index);
        this->ui.cbFrameThree->setCurrentIndex(frame3_index);
        this->ui.cbPitFour->setCurrentIndex(pit4_index);
        this->ui.cbFrameFour->setCurrentIndex(frame4_index);
        this->ui.cbPitFive->setCurrentIndex(pit5_index);
        this->ui.cbFrameFive->setCurrentIndex(frame5_index);
        this->ui.cbPitSix->setCurrentIndex(pit6_index);
        this->ui.cbFrameSix->setCurrentIndex(frame6_index);

        // Set the save button text
        this->ui.btnSave->setText(this->btn_update_text);
    }
    else {
        // Tomb not found means we are asking to insert a new one
        this->setWindowTitle("Crea nuova");

        // Reset the form fields
        this->ui.leProgressive->setText(QString::number(this->tomb->getLastProgresive() + 1));
        this->ui.cbClient->addItems(client_names);
        this->ui.cbMaterial->addItems(material_names);
        this->ui.cbType->addItems(tomb_type_names);
        this->ui.cbFormat->addItems(tomb_format_names);
        this->ui.cbVase->addItems(vase_names);
        this->ui.cbLamp->addItems(lamp_names);
        this->ui.cbFlame->addItems(flame_names);
        this->ui.cbCross->addItems(cross_names);
        this->ui.cbDrawing->addItems(drawing_names);
        this->ui.cbSculpture->addItems(sculpture_names);
        this->ui.cbPitOne->addItems(pit_names);
        this->ui.cbFrameOne->addItems(frame_names);
        this->ui.cbPitTwo->addItems(pit_names);
        this->ui.cbFrameTwo->addItems(frame_names);
        this->ui.cbPitThree->addItems(pit_names);
        this->ui.cbFrameThree->addItems(frame_names);
        this->ui.cbPitFour->addItems(pit_names);
        this->ui.cbFrameFour->addItems(frame_names);
        this->ui.cbPitFive->addItems(pit_names);
        this->ui.cbFrameFive->addItems(frame_names);
        this->ui.cbPitSix->addItems(pit_names);
        this->ui.cbFrameSix->addItems(frame_names);

        // Set the save button text
        this->ui.btnSave->setText(this->btn_create_text);

        // After all the fields have been populated, set the default selections
        this->resetForm();
    }

    delete client;
}

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
        (this->ui.leScHeight->text().trimmed() == "" || this->ui.leScHeight->text().toFloat() <= 0)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("L'altezza della scultura non è corretta");
        message.exec();
        return false;
    }

    if (this->ui.rbEngraveYes->isChecked() &&
        (this->ui.leEpigraphAmount->text().trimmed() == "" || this->ui.leEpigraphAmount->text().toInt() < 0)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Il numero di epigrafi non è corretto");
        message.exec();
        return false;
    }

    if (
        (this->ui.cbPitTwo->currentIndex() != 0 && this->ui.cbPitOne->currentIndex() == 0)
        ||
        (this->ui.cbPitThree->currentIndex() != 0 &&
            (
                this->ui.cbPitTwo->currentIndex() == 0 || this->ui.cbPitOne->currentIndex() == 0
                )
            )
        ||
        (this->ui.cbPitFour->currentIndex() != 0 &&
            (
                this->ui.cbPitThree->currentIndex() == 0 ||
                this->ui.cbPitTwo->currentIndex() == 0 ||
                this->ui.cbPitOne->currentIndex() == 0
                )
            )
        ||
        (this->ui.cbPitFive->currentIndex() != 0 &&
            (
                this->ui.cbPitFour->currentIndex() == 0 ||
                this->ui.cbPitThree->currentIndex() == 0 ||
                this->ui.cbPitTwo->currentIndex() == 0 ||
                this->ui.cbPitOne->currentIndex() == 0
                )
            )
        ||
        (this->ui.cbPitSix->currentIndex() != 0 &&
            (
                this->ui.cbPitFive->currentIndex() == 0 ||
                this->ui.cbPitFour->currentIndex() == 0 ||
                this->ui.cbPitThree->currentIndex() == 0 ||
                this->ui.cbPitTwo->currentIndex() == 0 ||
                this->ui.cbPitOne->currentIndex() == 0
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
