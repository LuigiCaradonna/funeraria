#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "###### FUNERARIA CONSTRUCTOR";
    ui.setupUi(this);

    this->config = new Config();
    this->db = new DatabaseManager(this->config->db_path, this);

    // The connection failed
    if (!this->db->db_connected) {
        qDebug() << "###### DB NOT CONNECTED";
        // Close the application
        this->closeWindow();
    }
    else {
        qDebug() << "###### Request Config Update";
        // Update the db path into the config file, the db manager could have had to 
        // create a new db file or select a different one, and we do not know here
        this->config->updateConfigFile("db_path", this->db->db_path);
    }

    connect(ui.clientListView, SIGNAL(clicked(const QModelIndex & index)), this, SLOT(slotShowData(QModelIndex index)));

    ui.clientListView->setModel(this->db->getClients());

    ui.tableView->setModel(this->db->getClientsTable());
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    delete this->config;
    delete this->db;
}

void Funeraria::slotShowData(QModelIndex index)
{
    QMessageBox::information(this, "Funeraria", QString::number(index.row()), QMessageBox::Ok);
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::closeWindow()
{
    this->close();
}
