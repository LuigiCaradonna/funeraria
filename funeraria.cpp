#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget *parent)
    : QMainWindow(parent)
{
    this->ui.setupUi(this);

    // Set the font for the table
    QFont font("Calibri", 12);
    this->ui.tableWidget->setFont(font);

    /*
    QFontMetrics fm(font);
    int pixelsWide = fm.horizontalAdvance("What's the advance width of this text?");
    int pixelsHigh = fm.height();
    */

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
    QStringList clients = this->client->getNames();
    // Add the clients to the combo box
    this->ui.cbClient->addItem(this->client_placeholder);
    this->ui.cbClient->addItems(clients);

    // Populate the years combo box
    this->ui.cbYear->addItem("Tutti");
    QString this_year = QDate::currentDate().toString("yyyy");
    for (int i = this_year.toInt(); i >= 2020; i--) {
        this->ui.cbYear->addItem(QString::number(i));
    }

    // Set the event listeners
    this->connect(this->ui.btnSearch, &QPushButton::clicked, this, &Funeraria::slotClientOrders);
    this->connect(this->ui.leDeceased, &QLineEdit::textChanged, this, &Funeraria::slotFilterClientOrders);
    // Signal emitted on table cell edit
    this->connect(this->ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(slotUpdateEntry()));
    // Signal emitted from the menu item actionCList
    this->connect(this->ui.actionCList, SIGNAL(triggered()), this, SLOT(slotShowClients()));
    // Signal emitted from the menu item actionCNew
    this->connect(this->ui.actionCNew, SIGNAL(triggered()), this, SLOT(slotNewClient()));
    // Signal emitted from the menu item actionTNew
    this->connect(this->ui.actionTNew, SIGNAL(triggered()), this, SLOT(slotNewTomb()));

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
void Funeraria::slotFilterClientOrders()
{
    if (this->ui.cbClient->currentText() == this->client_placeholder || this->current_table != "tomb") {
        return;
    }

    this->slotClientOrders();
}

void Funeraria::slotClientOrders()
{
    if (this->ui.cbClient->currentText() == this->client_placeholder) {
        return;
    }

    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->current_table = "tomb";

    // Reset the table's content
    this->ui.tableWidget->clear();

    int client_id = this->client->getId(this->ui.cbClient->currentText());
    int year;
    QString filter = this->ui.leDeceased->text().trimmed();

    if (this->ui.cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->ui.cbYear->currentText().toInt();
    }
    
    QList<QMap<QString, QString>> tombs = this->tomb->getList(client_id, year, filter);

    QStringList headers{ "Numero", "Defunto", "Prezzo", "Pagata", "Note", "Accessori",
        "Ordine", "Provino", "Conferma", "Incisione", "Consegna", "Azioni"};

    this->ui.tableWidget->setRowCount(tombs.size());
    this->ui.tableWidget->setColumnCount(12);
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < tombs.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");

        // Generate the cells' content and set them as not editable
        QTableWidgetItem* progressive = new QTableWidgetItem(tombs[i]["progressive"]);
        progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* name = new QTableWidgetItem(tombs[i]["name"]);
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* price = new QTableWidgetItem(tombs[i]["price"]);
        price->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* paid = new QTableWidgetItem(tombs[i]["paid"]);
        paid->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* notes = new QTableWidgetItem(tombs[i]["notes"]);
        notes->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* accessories_mounted = new QTableWidgetItem(tombs[i]["accessories_mounted"]);
        accessories_mounted->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* ordered_at = new QTableWidgetItem(Helpers::dateSqlToIta(tombs[i]["ordered_at"]));
        ordered_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* proofed_at = new QTableWidgetItem(Helpers::dateSqlToIta(tombs[i]["proofed_at"]));
        proofed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* confirmed_at = new QTableWidgetItem(Helpers::dateSqlToIta(tombs[i]["confirmed_at"]));
        confirmed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* engraved_at = new QTableWidgetItem(Helpers::dateSqlToIta(tombs[i]["engraved_at"]));
        engraved_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* delivered_at = new QTableWidgetItem(Helpers::dateSqlToIta(tombs[i]["delivered_at"]));
        delivered_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        this->ui.tableWidget->setItem(i, 0, progressive);
        this->ui.tableWidget->setItem(i, 1, name);
        this->ui.tableWidget->setItem(i, 2, price);
        this->ui.tableWidget->setItem(i, 3, paid);
        this->ui.tableWidget->setItem(i, 4, notes);
        this->ui.tableWidget->setItem(i, 5, accessories_mounted);
        this->ui.tableWidget->setItem(i, 6, ordered_at);
        this->ui.tableWidget->setItem(i, 7, proofed_at);
        this->ui.tableWidget->setItem(i, 8, confirmed_at);
        this->ui.tableWidget->setItem(i, 9, engraved_at);
        this->ui.tableWidget->setItem(i, 10, delivered_at);
        this->ui.tableWidget->setCellWidget(i, 11, pb_details); // Details button

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotTombDetails);
    }
}

void Funeraria::slotTombDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    this->tomb->setProgressive(this->ui.tableWidget->item(row, 0)->text().toInt());
    this->tomb->setModal(true);
    this->tomb->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotClientOrders();
}

void Funeraria::slotNewTomb()
{
    this->current_table = "tomb";

    // Set the name property of the Client object to an empty string
    this->tomb->setProgressive(0);
    this->tomb->setModal(true);
    this->tomb->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotClientOrders();
}

void Funeraria::slotShowClients()
{
    // Block the segnals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->current_table = "client";

    // Reset the table's content
    this->ui.tableWidget->clear();

    QList<QMap<QString, QString>> clients = this->client->get();

    QStringList headers{ "Ordine", "Nome", "Email", "Telefono", "", ""};

    this->ui.tableWidget->setRowCount(clients.size());
    this->ui.tableWidget->setColumnCount(6);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < clients.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");
        QPushButton* pb_delete = new QPushButton(this->ui.tableWidget);
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

        this->ui.tableWidget->setItem(i, 0, new QTableWidgetItem(clients[i]["position"])); // position
        this->ui.tableWidget->setItem(i, 1, new QTableWidgetItem(clients[i]["name"])); // name
        this->ui.tableWidget->setItem(i, 2, new QTableWidgetItem(emails)); // emails
        this->ui.tableWidget->setItem(i, 3, new QTableWidgetItem(phones)); // phones
        this->ui.tableWidget->setCellWidget(i, 4, pb_details); // Details button
        this->ui.tableWidget->setCellWidget(i, 5, pb_delete); // Delete button

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotClientDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDelete);
    }
}

void Funeraria::slotClientDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the name property of the Client object to the name present in the clicked row
    this->client->setName(this->ui.tableWidget->item(row, 1)->text());
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
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->current_table = type;

    // Reset the table's content
    this->ui.tableWidget->clear();

    QList<QMap<QString, QString>> accessories;

    // Get the the list of the required accessories
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

    QStringList headers{ "Codice", "Nome", "Azioni"};

    this->ui.tableWidget->setRowCount(accessories.size());
    this->ui.tableWidget->setColumnCount(3);
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < accessories.size(); i++) {
        QPushButton* pb = new QPushButton(this->ui.tableWidget);
        pb->setText("Elimina");
        this->ui.tableWidget->setItem(i, 0, new QTableWidgetItem(accessories[i]["code"]));
        this->ui.tableWidget->setItem(i, 1, new QTableWidgetItem(accessories[i]["name"]));
        this->ui.tableWidget->setCellWidget(i, 2, pb); // Delete button
        this->connect(pb, &QPushButton::clicked, this, &Funeraria::slotDelete);
    }
}

void Funeraria::slotDelete() {
    int row = this->ui.tableWidget->currentRow();

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
            this->vase->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "lamp") {
            this->lamp->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "flame") {
            this->flame->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "material") {
            this->material->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "client") {
            this->client->remove(this->client->getId(this->ui.tableWidget->item(row, 1)->text()));
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
    int row = this->ui.tableWidget->currentRow();
    if (this->current_table == "tomb") {
        
    }
    else if (this->current_table == "vase") {
        this->vase->update(
            this->ui.tableWidget->item(row, 0)->text(), // code
            this->ui.tableWidget->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "lamp") {
        this->lamp->update(
            this->ui.tableWidget->item(row, 0)->text(), // code
            this->ui.tableWidget->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "flame") {
        this->flame->update(
            this->ui.tableWidget->item(row, 0)->text(), // code
            this->ui.tableWidget->item(row, 1)->text() // name
        );
    }
    else if (this->current_table == "material") {
        this->material->update(
            this->ui.tableWidget->item(row, 0)->text(), // code
            this->ui.tableWidget->item(row, 1)->text() // name
        );
    }
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::closeWindow()
{
    this->close();
}
