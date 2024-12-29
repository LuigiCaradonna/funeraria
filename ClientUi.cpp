#include "ClientUi.h"

/********** CONSTRUCTOR **********/

ClientUi::ClientUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent)
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

    // id initialization
    this->id = 0;

    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &ClientUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

ClientUi::~ClientUi()
{}

/********** PUBLIC FUNCTIONS **********/

void ClientUi::setId(int id)
{
    this->id = id;
    this->updateForm();
}

void ClientUi::setName(const QString& name)
{
    this->name = name;
    this->updateForm();
}

/********** PROTECTED SLOTS **********/

void ClientUi::slotSave()
{
    if (this->checkForm()) {
        Client* client = new Client(this->db);

        int active = 0;
        if (this->ui.cbActive->isChecked()) {
            active = 1;
        }

        int quick = 0;
        if (this->ui.cbQuick->isChecked()) {
            quick = 1;
        }

        if (client->store(
            this->ui.lePosition->text().trimmed().toInt(),
            this->ui.leName->text().trimmed(),
            this->ui.teEmail->toPlainText().trimmed(),
            this->ui.leAddress->text().trimmed(),
            this->ui.tePhone->toPlainText().trimmed(),
            active,
            quick)
        ) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Cliente creato.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Inserimento cliente non riuscito.");
            message.exec();
        }

        delete client;
        // Close the dialog
        this->close();
    }
}

void ClientUi::slotUpdate()
{
    if (this->checkForm()) {
        Client* client = new Client(this->db);

        int active = this->ui.cbActive->isChecked() ? 1 : 0;

        int quick = this->ui.cbQuick->isChecked() ? 1 : 0;

        if (client->update(
            this->ui.leId->text().toInt(),
            this->ui.lePosition->text().trimmed().toInt(),
            this->ui.leName->text().trimmed(),
            this->ui.teEmail->toPlainText().trimmed(),
            this->ui.leAddress->text().trimmed(),
            this->ui.tePhone->toPlainText().trimmed(),
            active,
            quick)
        ) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Cliente modificato.");
            message.exec();
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Aggiornamento cliente non riuscito.");
            message.exec();
        }

        delete client;
        // Close the dialog
        this->close();
    }
}

void ClientUi::slotCloseDialog()
{
    this->close();
}

/********** PRIVATE FUNCTIONS **********/

void ClientUi::updateForm()
{
    Client* client = new Client(this->db);
    // Get the selected client's data
    QMap<QString, QString> client_details = client->getDetailsById(this->id);

    if (!client_details.isEmpty()) {
        this->setWindowTitle("Modifica cliente");

        QStringList emails_list = client_details["email"].split(u',');
        QString emails = "";
        for (int j = 0; j < emails_list.length(); j++) {
            QString nl = "\n";
            if (j == emails_list.length() - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[j] + nl;
        }

        QStringList phones_list = client_details["phone"].split(u',');
        QString phones = "";
        for (int j = 0; j < phones_list.length(); j++) {
            QString nl = "\n";
            if (j == phones_list.length() - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[j] + nl;
        }

        // Fill the form fields with the selected client's data
        this->ui.leId->setText(client_details["id"]);
        this->ui.lePosition->setText(client_details["position"]);
        this->ui.leName->setText(client_details["name"]);
        this->ui.teEmail->setText(emails);
        this->ui.leAddress->setText(client_details["address"]);
        this->ui.tePhone->setText(phones);
        this->ui.cbActive->setChecked(client_details["active"] == "1");
        this->ui.cbQuick->setChecked(client_details["quick"] == "1");

        // Set the save button text
        this->ui.btnSave->setText("Aggiorna");
        // Disconnect previous bindings or multiple signals will be emitted
        this->ui.btnSave->disconnect();
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &ClientUi::slotUpdate);
    }
    else {
        // Client not found means we are asking to insert a new one
        this->setWindowTitle("Inserisci cliente");

        // Reset the form fields
        this->ui.leId->setText("0");
        this->ui.lePosition->setText(QString::number(client->getLastPosition() + 1));
        this->ui.leName->setText("");
        this->ui.teEmail->setText("");
        this->ui.leAddress->setText("");
        this->ui.tePhone->setText("");
        this->ui.cbActive->setChecked(true);

        // Set the save button text
        this->ui.btnSave->setText("Crea");
        // Disconnect previous bindings or multiple signals will be emitted
        this->ui.btnSave->disconnect();
        this->connect(this->ui.btnSave, &QPushButton::clicked, this, &ClientUi::slotSave);
    }

    delete client;
}

bool ClientUi::checkForm()
{
    QRegularExpression re("\\d+");
    QRegularExpressionMatch match = re.match(this->ui.lePosition->text().trimmed());
    if (!match.hasMatch() || this->ui.lePosition->text().trimmed().toInt() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("La posizione deve essere un numero intero maggiore di zero.");
        message.exec();

        return false;
    }

    return true;
}