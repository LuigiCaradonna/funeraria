#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    this->ui.setupUi(this);

    this->config = new Config();
    this->db = new DatabaseManager(this->config->db_path, this);

    // The connection failed
    if (!this->db->connected) {
        // Close the application
        this->closeWindow();
    }
    else {
        // Update the db path into the config file, the db manager could have had to 
        // create a new db file or select a different one, but we do not know about that here
        this->config->updateConfigFile("db_path", this->db->path);
    }

    this->client = new Client(this->db->db, this);

    QStringList headers{"Nome", "Email", "Indirizzo", "Tel"};

    QStringList cli = this->client->getNames();

    this->ui.clientListView->addItems(cli);

    QList< QMap<QString, QString>> clients = this->client->getDetails();
    this->ui.tableView->setRowCount(clients.size());
    this->ui.tableView->setColumnCount(4);
    this->ui.tableView->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < clients.size(); i++) {
        this->ui.tableView->setItem(i, 0, new QTableWidgetItem(clients[i].value("name")));
        this->ui.tableView->setItem(i, 1, new QTableWidgetItem(clients[i].value("email")));
        this->ui.tableView->setItem(i, 2, new QTableWidgetItem(clients[i].value("address")));
        this->ui.tableView->setItem(i, 3, new QTableWidgetItem(clients[i].value("phone")));
    }

    // Set the event listeners
    this->connect(this->ui.clientListView, &QListWidget::itemClicked, this, &Funeraria::slotShowData);
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    delete this->config;
    delete this->db;
    delete this->client;
}

void Funeraria::slotShowData(QListWidgetItem* index)
{
    // Highlight the clicked item
    this->ui.clientListView->setCurrentItem(index);
    
    // This gets the string value of the clicked item
    // Qt::DisplayRole is required because data is of type QVariant
    QMap<QString, QString> cliente = this->client->getDetails(index->data(Qt::DisplayRole).toString());
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::closeWindow()
{
    this->close();
}
