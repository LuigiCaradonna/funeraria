#include "SettingsUi.h"

/********** CONSTRUCTOR **********/

SettingsUi::SettingsUi(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);
    this->updateForm();

    this->connect(this->ui.btnDbPath, &QPushButton::clicked, this, &SettingsUi::slotChangeDbPath);
    this->connect(this->ui.btnArchiveFolder, &QPushButton::clicked, this, &SettingsUi::slotChangeArchivePath);
    this->connect(this->ui.btnSculpturesFolder, &QPushButton::clicked, this, &SettingsUi::slotChangeSculpturesPath);
    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &SettingsUi::slotSave);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &SettingsUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

SettingsUi::~SettingsUi()
{
}

/********** PROTECTED SLOTS **********/

void SettingsUi::slotChangeDbPath()
{
    // Prompt the user to select the db file
    QString new_db_file = QFileDialog::getOpenFileName(this->parent, "Apri", "./", "Database (*.db *.sqlite *.sqlite3)");

    // Check if a file was selected
    if (!new_db_file.isEmpty()) {
        // Check if the file exists
        if (!QFile::exists(new_db_file)) {
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

    this->ui.lblDbFile->setText(new_db_file);
}

void SettingsUi::slotChangeArchivePath()
{
    // Prompt the user to select the archive folder
    QString new_archive_path = QFileDialog::getExistingDirectory(this, "Seleziona cartella");

    if (new_archive_path.isEmpty()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("La cartella indicata risulta inesistente.");
        message.exec();
        return;
    }

    this->ui.lblArchiveFolder->setText(new_archive_path);
}

void SettingsUi::slotChangeSculpturesPath()
{
    // Prompt the user to select the archive folder
    QString new_sculptures_path = QFileDialog::getExistingDirectory(this, "Seleziona cartella");

    if (new_sculptures_path.isEmpty()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("La cartella indicata risulta inesistente.");
        message.exec();
        return;
    }

    this->ui.lblSculpturesFolder->setText(new_sculptures_path);
}

void SettingsUi::slotSave()
{
    QMap<QString, QString> setting;
    bool stored = true;

    /*
        Update at first the DB path if necessary, it will also reload the DB in use
        In this way the following updates will affect the correct DB
    */
    Config* config = new Config();

    QString db_file = this->ui.lblDbFile->text();
    if (db_file != config->getDbFile()) {
        config->setDbFile(db_file);

        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("E' cambiato il DB in uso, è necessario riavviare il programma.");
        message.exec();
    }

    QString archive_path = this->ui.lblArchiveFolder->text();
    if (archive_path != config->getArchivePath()) {
        config->setArchivePath(archive_path);
    }

    QString sculptures_path = this->ui.lblSculpturesFolder->text();
    if (sculptures_path != config->getSculpturesPath()) {
        config->setSculpturesPath(sculptures_path);
    }

    delete config;

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

    Config* config = new Config();
    this->ui.lblDbFile->setText(config->getDbFile());
    this->ui.lblArchiveFolder->setText(config->getArchivePath());
    this->ui.lblSculpturesFolder->setText(config->getSculpturesPath());

    delete config;
    delete settings;
}

bool SettingsUi::store(const QMap<QString, QString>& setting) {

    Settings* settings = new Settings(this->db);
    bool result;
    result = settings->store(setting);
    
    delete settings;

    return result;
}