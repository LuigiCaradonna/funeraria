#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    this->ui.setupUi(this);

    this->config = new Config();
    this->db = new DatabaseManager(this->config->db_path, this);

    // The connection failed
    if (!this->db->db_connected) {
        // Close the application
        this->closeWindow();
    }
    else {
        // Update the db path into the config file, the db manager could have had to 
        // create a new db file or select a different one, and we do not know here
        this->config->updateConfigFile("db_path", this->db->db_path);
    }

    this->ui.clientListView->setModel(this->db->getClients());

    this->ui.tableView->setModel(this->db->getClientsTable());

    // Set the event listeners
    this->connect(this->ui.clientListView, &QListView::clicked, this, &Funeraria::slotShowData);
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    delete this->config;
    delete this->db;
}

void Funeraria::slotShowData(QModelIndex index)
{
    // Highlight the clicked item
    this->ui.clientListView->setCurrentIndex(index);
    
    // This gets the string value of the clicked item
    // index.data(Qt::DisplayRole).toString();
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::closeWindow()
{
    this->close();
}
