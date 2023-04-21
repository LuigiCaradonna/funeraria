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
    this->tomb = new Tomb(this->db->db, this);
    // this->vase = new Vase(this->db->db, this);
    // this->lamp = new Lamp(this->db->db, this);
    this->flame = new Flame(this->db->db, this);

    QStringList cli = this->client->getNames();

    this->ui.clientListView->addItems(cli);

    /*
    QStringList headers{"Nome", "Email", "Indirizzo", "Tel"};

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
    */
    // Set the event listeners
    this->connect(this->ui.clientListView, &QListWidget::itemClicked, this, &Funeraria::slotShowData);
    // Signal emitted on table cell edit
    this->connect(this->ui.tableView, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(slotUpdateEntry(QTableWidgetItem*)));

    // Bind the menu entries to the corresponding slot
    this->connect(this->ui.actionFList, SIGNAL(triggered()), this, SLOT(slotFlames()));
    this->connect(this->ui.actionFNew, SIGNAL(triggered()), this, SLOT(slotNewFlame()));
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    delete this->config;
    delete this->db;
    delete this->client;
    delete this->tomb;
    // delete this->vase;
    // delete this->lamp;
    delete this->flame;
}

/********** SLOTS **********/

void Funeraria::slotShowData(QListWidgetItem* index)
{
    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableView);

    this->current_table = "clients";

    // Highlight the clicked item
    this->ui.clientListView->setCurrentItem(index);
    
    // This gets the string value of the clicked item
    // Qt::DisplayRole is required because data is of type QVariant
    // QMap<QString, QString> cliente = this->client->getDetails(index->data(Qt::DisplayRole).toString());
    // QMessageBox message;
    // message.setIcon(QMessageBox::Critical);
    // message.setText("Valore: " + cliente["name"]);
    // message.exec();

    int client_id = this->client->getId(index->data(Qt::DisplayRole).toString());
    // TODO: In base all'id del cliente, prendere tutte le sue lapidi
    QList<QStringList> tombs = this->tomb->get(client_id);

    QStringList headers{ "Numero", "Nome", "Prezzo", "Pagata", "Note", "Accessori",
        "Ordine", "Provino", "Conferma", "Incisione", "Consegna"};

    this->ui.tableView->setRowCount(tombs.size());
    this->ui.tableView->setColumnCount(11);
    // this->ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //this->ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableView->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < tombs.size(); i++) {
        // this->ui.tableView->setItem(i, 0, new QTableWidgetItem(tombs[i][0])); // id
        this->ui.tableView->setItem(i, 0, new QTableWidgetItem(tombs[i][0])); // Numero
        this->ui.tableView->setItem(i, 1, new QTableWidgetItem(tombs[i][1])); // Nome
        this->ui.tableView->setItem(i, 2, new QTableWidgetItem(tombs[i][2])); // Prezzo
        this->ui.tableView->setItem(i, 3, new QTableWidgetItem(tombs[i][3])); // Pagata
        this->ui.tableView->setItem(i, 4, new QTableWidgetItem(tombs[i][4])); // Note
        this->ui.tableView->setItem(i, 5, new QTableWidgetItem(tombs[i][5])); // Accessori
        this->ui.tableView->setItem(i, 6, new QTableWidgetItem(tombs[i][6])); // Ordine
        this->ui.tableView->setItem(i, 7, new QTableWidgetItem(tombs[i][7])); // Provino
        this->ui.tableView->setItem(i, 8, new QTableWidgetItem(tombs[i][8])); // Conferma
        this->ui.tableView->setItem(i, 9, new QTableWidgetItem(tombs[i][9])); // Incisione
        this->ui.tableView->setItem(i, 10, new QTableWidgetItem(tombs[i][10])); // Consegna
    }
}

void Funeraria::slotFlames()
{
    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableView);

    this->current_table = "flames";

    QList<QStringList> flames = this->flame->get();

    QStringList headers{ "id", "Nome" };

    this->ui.tableView->setRowCount(flames.size());
    this->ui.tableView->setColumnCount(2);
    this->ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //this->ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableView->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < flames.size(); i++) {
        this->ui.tableView->setItem(i, 0, new QTableWidgetItem(flames[i][0])); // id
        this->ui.tableView->setItem(i, 1, new QTableWidgetItem(flames[i][1])); // name
    }
}

void Funeraria::slotNewFlame()
{
    // TODO: creare un form popup per inserire i dati di una nuova fiamma
    // this->tool_translation = new ToolTranslation(this->folder);
    // this->tool_translation->show();
}

void Funeraria::slotUpdateEntry(QTableWidgetItem* item)
{
    int row = this->ui.tableView->currentRow();

    if (this->current_table == "tombs") {
        
    }
    else if (this->current_table == "vases") {
        
    }
    else if (this->current_table == "lamps") {
        
    }
    else if (this->current_table == "flames") {
        this->flame->update(
            this->ui.tableView->item(row, 0)->text(), // id
            this->ui.tableView->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "materials") {
        
    }
    else if (this->current_table == "clients") {
        
    }
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::closeWindow()
{
    this->close();
}
