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

    this->client = new Client(&this->db->db, this);
    this->tomb = new Tomb(&this->db->db, this);
    this->vase = new Accessory(&this->db->db, "vases", this);
    this->lamp = new Accessory(&this->db->db, "lamps", this);
    this->flame = new Accessory(&this->db->db, "flames", this);
    this->material = new Accessory(&this->db->db, "materials", this);

    // List of clients' names
    QStringList cli = this->client->getNames();

    // Add the clients' list to the left column
    this->ui.clientListView->addItems(cli);

    // Set the event listeners
    this->connect(this->ui.clientListView, &QListWidget::itemClicked, this, &Funeraria::slotClientOrders);
    // Signal emitted on table cell edit
    this->connect(this->ui.tableView, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(slotUpdateEntry()));
    // Signal emitted from the menu item actionCList
    this->connect(this->ui.actionCList, SIGNAL(triggered()), this, SLOT(slotShowClients()));
    // Signal emitted from the menu item actionCNew
    this->connect(this->ui.actionCNew, SIGNAL(triggered()), this, SLOT(slotNewClient()));

    // Map the signal coming from the menu items to call the same function (slotShowItems) with the proper parameter
    this->showItemsMapper = new QSignalMapper(this);
    this->connect(this->ui.actionVList, SIGNAL(triggered()), showItemsMapper, SLOT(map()));
    this->connect(this->ui.actionLList, SIGNAL(triggered()), showItemsMapper, SLOT(map()));
    this->connect(this->ui.actionFList, SIGNAL(triggered()), showItemsMapper, SLOT(map()));
    this->connect(this->ui.actionMList, SIGNAL(triggered()), showItemsMapper, SLOT(map()));
    showItemsMapper->setMapping(this->ui.actionVList, "vase");
    showItemsMapper->setMapping(this->ui.actionLList, "lamp");
    showItemsMapper->setMapping(this->ui.actionFList, "flame");
    showItemsMapper->setMapping(this->ui.actionMList, "material");
    this->connect(showItemsMapper, &QSignalMapper::mappedString, this, &Funeraria::slotShowItems);

    // Map the signal coming from the menu items to call the same function (slotNewItem) with the proper parameter
    this->newItemMapper = new QSignalMapper(this);
    this->connect(this->ui.actionVNew, SIGNAL(triggered()), newItemMapper, SLOT(map()));
    this->connect(this->ui.actionLNew, SIGNAL(triggered()), newItemMapper, SLOT(map()));
    this->connect(this->ui.actionFNew, SIGNAL(triggered()), newItemMapper, SLOT(map()));
    this->connect(this->ui.actionMNew, SIGNAL(triggered()), newItemMapper, SLOT(map()));
    newItemMapper->setMapping(this->ui.actionVNew, "vase");
    newItemMapper->setMapping(this->ui.actionLNew, "lamp");
    newItemMapper->setMapping(this->ui.actionFNew, "flame");
    newItemMapper->setMapping(this->ui.actionMNew, "material");
    this->connect(newItemMapper, &QSignalMapper::mappedString, this, &Funeraria::slotNewItem);
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    delete this->config;
    delete this->db;
    delete this->client;
    delete this->tomb;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
    delete this->material;
    delete this->showItemsMapper;
    delete this->newItemMapper;
}

/********** SLOTS **********/

void Funeraria::slotClientOrders(QListWidgetItem* index)
{
    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableView);

    this->current_table = "tomb";

    // Highlight the clicked item
    this->ui.clientListView->setCurrentItem(index);
    
    int client_id = this->client->getId(index->data(Qt::DisplayRole).toString());
    
    QList<QStringList> tombs = this->tomb->get(client_id);

    QStringList headers{ "Numero", "Nome", "Prezzo", "Pagata", "Note", "Accessori",
        "Ordine", "Provino", "Conferma", "Incisione", "Consegna"};

    this->ui.tableView->setRowCount(tombs.size());
    this->ui.tableView->setColumnCount(11);
    // this->ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // this->ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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

void Funeraria::slotShowClients()
{
    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableView);

    this->current_table = "client";

    QList<QMap<QString, QString>> clients = this->client->get();

    QStringList headers{ "Ordine", "Nome", "Email", "Telefono", "", ""};

    this->ui.tableView->setRowCount(clients.size());
    this->ui.tableView->setColumnCount(6);
    this->ui.tableView->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < clients.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableView);
        pb_details->setText("Dettagli");
        QPushButton* pb_delete = new QPushButton(this->ui.tableView);
        pb_delete->setText("Elimina");

        QStringList emails_list = clients[i]["email"].split(u',');
        QString emails = "";
        for (int j = 0; j < emails_list.length(); j++) {
            QString nl = "\n";
            if (j == emails_list.length() - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[j] + nl;
        }

        QStringList phones_list = clients[i]["phone"].split(u',');
        QString phones = "";
        for (int j = 0; j < phones_list.length(); j++) {
            QString nl = "\n";
            if (j == phones_list.length() - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[j] + nl;
        }

        this->ui.tableView->setItem(i, 0, new QTableWidgetItem(clients[i]["position"])); // position
        this->ui.tableView->setItem(i, 1, new QTableWidgetItem(clients[i]["name"])); // name
        this->ui.tableView->setItem(i, 2, new QTableWidgetItem(emails)); // emails
        this->ui.tableView->setItem(i, 3, new QTableWidgetItem(phones)); // phones
        this->ui.tableView->setCellWidget(i, 4, pb_details); // Details button
        this->ui.tableView->setCellWidget(i, 5, pb_delete); // Delete button

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotClientDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDelete);
    }
}

void Funeraria::slotClientDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableView->currentRow();
    // Set the name property of the Client object to the name present in the clicked row
    this->client->setName(this->ui.tableView->item(row, 1)->text());
    this->client->setModal(true);
    this->client->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowClients();
}

void Funeraria::slotNewClient()
{
    this->current_table = "client";

    // Set the name property of the Client object to an empty string
    this->client->setName("");
    this->client->setModal(true);
    this->client->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowClients();
}

void Funeraria::slotShowItems(const QString& type)
{
    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableView);

    this->current_table = type;

    QList<QStringList> accessories;

    if (type == "vase") {
        accessories = this->vase->get();
    }
    else if (type == "lamp") {
        accessories = this->lamp->get();
    }
    else if (type == "flame") {
        accessories = this->flame->get();
    }
    else if (type == "material") {
        accessories = this->material->get();
    }
    else {
        // The type requested is not valid
        return;
    }

    QStringList headers{ "id", "Nome", "Azioni"};

    this->ui.tableView->setRowCount(accessories.size());
    this->ui.tableView->setColumnCount(3);
    // this->ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //this->ui.tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableView->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < accessories.size(); i++) {
        QPushButton* pb = new QPushButton(this->ui.tableView);
        pb->setText("Elimina");
        this->ui.tableView->setItem(i, 0, new QTableWidgetItem(accessories[i][0])); // id
        this->ui.tableView->setItem(i, 1, new QTableWidgetItem(accessories[i][1])); // name
        this->ui.tableView->setCellWidget(i, 2, pb); // Delete button
        this->connect(pb, &QPushButton::clicked, this, &Funeraria::slotDelete);
    }
}

void Funeraria::slotDelete() {
    int row = this->ui.tableView->currentRow();

    QMessageBox message;
    QPushButton* proceedBtn = message.addButton("Elimina", QMessageBox::ActionRole);
    QPushButton* abortBtn = message.addButton("Annulla", QMessageBox::ActionRole);
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Warning);
    message.setText("Vuoi eliminare questo elemento?");
    message.exec();

    if (message.clickedButton() == proceedBtn) {
        // Delete the item

        if (this->current_table == "tomb") {

        }
        else if (this->current_table == "vase") {
            this->vase->remove(this->ui.tableView->item(row, 0)->text());
        }
        else if (this->current_table == "lamp") {
            this->lamp->remove(this->ui.tableView->item(row, 0)->text());
        }
        else if (this->current_table == "flame") {
            this->flame->remove(this->ui.tableView->item(row, 0)->text());
        }
        else if (this->current_table == "material") {
            this->material->remove(this->ui.tableView->item(row, 0)->text());
        }
        else if (this->current_table == "client") {
            this->client->remove(this->client->getId(this->ui.tableView->item(row, 1)->text()));
            this->slotShowClients();
            return;
        }

        this->slotShowItems(this->current_table);
    }
}

void Funeraria::slotNewItem(const QString& type)
{
    this->current_table = type;

    if (type == "vase") {
        this->vase->setModal(true);
        this->vase->exec();
    }
    else if (type == "lamp") {
        this->lamp->setModal(true);
        this->lamp->exec();
    }
    else if (type == "flame") {
        this->flame->setModal(true);
        this->flame->exec();
    }
    else if (type == "material") {
        this->material->setModal(true);
        this->material->exec();
    }
    else {
        // The type requested is not valid
        return;
    }

    this->slotShowItems(this->current_table);
}

void Funeraria::slotUpdateEntry()
{
    int row = this->ui.tableView->currentRow();
    if (this->current_table == "tomb") {
        
    }
    else if (this->current_table == "vase") {
        this->vase->update(
            this->ui.tableView->item(row, 0)->text(), // id
            this->ui.tableView->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "lamp") {
        this->lamp->update(
            this->ui.tableView->item(row, 0)->text(), // id
            this->ui.tableView->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "flame") {
        this->flame->update(
            this->ui.tableView->item(row, 0)->text(), // id
            this->ui.tableView->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "material") {
        this->material->update(
            this->ui.tableView->item(row, 0)->text(), // id
            this->ui.tableView->item(row, 1)->text() // name
        );
    }
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::closeWindow()
{
    this->close();
}
