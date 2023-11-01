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
    Settings* settings = new Settings(this->db);

    QList<QMap<QString, QString>> settings_list;
    QMap<QString, QString> setting;

    setting["name"] = "backup_interval";
    setting["value"] = this->ui.leBkupInterval->text();
    settings_list.append(setting);
    setting["name"] = "backups_to_keep";
    setting["value"] = this->ui.leBkupKeep->text();
    settings_list.append(setting);

    if (settings->store(settings_list)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Impostazioni salvate.");
        message.exec();
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Critical);
        message.setText("Non è stato possibile salvare le impostazioni.");
        message.exec();
    }

    delete settings;
}

void SettingsUi::slotCloseDialog()
{
    this->close();
}

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