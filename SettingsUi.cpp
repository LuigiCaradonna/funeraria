#include "SettingsUi.h"

/********** CONSTRUCTOR **********/

SettingsUi::SettingsUi(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);
    this->updateForm();

    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &SettingsUi::slotSave);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &SettingsUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

SettingsUi::~SettingsUi()
{
}

/********** PROTECTED SLOTS **********/

void SettingsUi::slotSave()
{
    QMap<QString, QString> setting;
    bool stored = true;

    setting["name"] = "backup_interval";
    setting["value"] = this->ui.leBkupInterval->text();
    if (!this->store(setting)) {
        stored = false;
    };

    setting["name"] = "backups_to_keep";
    setting["value"] = this->ui.leBkupKeep->text();
    if (!this->store(setting)) {
        stored = false;
    };

    if (!stored) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Una o più impostazioni non sono state salvate.");
        message.exec();
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Le impostazioni sono state salvate.");
        message.exec();
    }
}

void SettingsUi::slotChangeDbPath()
{
    // Prompt the user to select the db file
    QString new_db_path = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3)");

    // Check if a file was selected
    if (!new_db_path.isEmpty()) {
        // Check if the file exists
        if (!QFile::exists(new_db_path)) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Il file indicato risulta inesistente.");
            message.exec();
            return;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Nessun file selezionato.");
        message.exec();
        return;
    }
}

void SettingsUi::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

void SettingsUi::updateForm() {
    Settings* settings = new Settings(this->db);
    QList<QMap<QString, QString>> settings_list = settings->get();

    for (int i = 0; i < settings_list.size(); i++) {
        if (settings_list[i]["name"] == "backup_interval") {
            this->ui.leBkupInterval->setText(settings_list[i]["value"]);
        }
        else if (settings_list[i]["name"] == "backups_to_keep") {
            this->ui.leBkupKeep->setText(settings_list[i]["value"]);
        }
    }

    delete settings;
}

bool SettingsUi::store(const QMap<QString, QString>& setting) {

    Settings* settings = new Settings(this->db);
    bool result;
    result = settings->store(setting);
    
    delete settings;

    return result;
}