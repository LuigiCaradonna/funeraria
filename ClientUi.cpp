#include "ClientUi.h"

/********** CONSTRUCTOR **********/

ClientUi::ClientUi(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

    this->connect(this->ui.btnSave, &QPushButton::clicked, this, &ClientUi::slotSave);
    this->connect(this->ui.btnClose, &QPushButton::clicked, this, &ClientUi::slotCloseDialog);
}

/********** DESTRUCTOR **********/

ClientUi::~ClientUi()
{}

/********** PUBLIC FUNCTIONS **********/

void ClientUi::setName(const QString& name)
{
    this->name = name;
}

/********** PROTECTED SLOTS **********/

void ClientUi::slotSave()
{
    Client* client = new Client(this->db);

    QRegularExpression re("\\d+");
    QRegularExpressionMatch match = re.match(this->ui.lePosition->text().trimmed());
    if (!match.hasMatch() || this->ui.lePosition->text().trimmed().toInt() < 1) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("La posizione deve essere un numero intero maggiore di zero.");
        message.exec();

        return;
    }

    int active;
    if (this->ui.cbActive->isChecked()) {
        active = 1;
    }
    else {
        active = 0;
    }

    // An id set to 0 means that we are creating a new client
    if (this->ui.leId->text().toInt() == 0) {
        if (client->store(
            this->ui.lePosition->text().trimmed().toInt(),
            this->ui.leName->text().trimmed(),
            this->ui.teEmail->toPlainText().trimmed(),
            this->ui.leAddress->text().trimmed(),
            this->ui.tePhone->toPlainText().trimmed(),
            active
        )
            ) {
            delete client;
            // Close the dialog on success
            this->close();
        }
    }
    else {
        if (client->update(
            this->ui.leId->text().toInt(),
            this->ui.lePosition->text().trimmed().toInt(),
            this->ui.leName->text().trimmed(),
            this->ui.teEmail->toPlainText().trimmed(),
            this->ui.leAddress->text().trimmed(),
            this->ui.tePhone->toPlainText().trimmed(),
            active
        )
            ) {
            delete client;
            // Close the dialog on success
            this->close();
        }
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
    QMap<QString, QString> client_details = client->getDetails(this->name);

    delete client;

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

        // Set the save button text
        this->ui.btnSave->setText("Aggiorna");
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
    }
}