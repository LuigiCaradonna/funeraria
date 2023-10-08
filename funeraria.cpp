#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget* parent)
    : QMainWindow(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));

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

    this->client = new Client(this->db->db);
    this->client_ui = new ClientUi(this->db->db, this);
    this->tombUi = new TombUi(this->db->db, this);
    this->vase = new Accessory(this->db->db, "vases");
    this->lamp = new Accessory(this->db->db, "lamps");
    this->flame = new Accessory(this->db->db, "flames");
    this->material = new Accessory(this->db->db, "materials");

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

    // Set the current index to the current year (index 0 is "Tutti")
    this->ui.cbYear->setCurrentIndex(1);

    // Set the event listeners
    this->connect(this->ui.btnSearch, &QPushButton::clicked, this, &Funeraria::slotClientOrders);
    this->connect(this->ui.leDeceased, &QLineEdit::textChanged, this, &Funeraria::slotFilterClientOrders);

    // Signal emitted on table cell edit
    this->connect(this->ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(slotUpdateEntry()));
    // Signal emitted from the menu items
    this->connect(this->ui.actionCList, SIGNAL(triggered()), this, SLOT(slotShowClients()));
    this->connect(this->ui.actionCNew, SIGNAL(triggered()), this, SLOT(slotNewClient()));
    this->connect(this->ui.actionTNew, SIGNAL(triggered()), this, SLOT(slotNewTomb()));
    this->connect(this->ui.actionTEngrave, SIGNAL(triggered()), this, SLOT(slotTombsToEngrave()));
    this->connect(this->ui.actionMAccessories, SIGNAL(triggered()), this, SLOT(slotAccessoriesToMount()));
    this->connect(this->ui.actionTPay, SIGNAL(triggered()), this, SLOT(slotTombsNotPaid()));

    // Map the signal coming from the menu items to call the same function (slotNewItem) with the proper parameter
    this->new_item_mapper = new QSignalMapper(this);
    this->connect(this->ui.actionMNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
    this->connect(this->ui.actionVNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
    this->connect(this->ui.actionLNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
    this->connect(this->ui.actionFNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
    new_item_mapper->setMapping(this->ui.actionMNew, "material");
    new_item_mapper->setMapping(this->ui.actionVNew, "vase");
    new_item_mapper->setMapping(this->ui.actionLNew, "lamp");
    new_item_mapper->setMapping(this->ui.actionFNew, "flame");
    this->connect(new_item_mapper, &QSignalMapper::mappedString, this, &Funeraria::slotNewItem);

    // Map the signal coming from the menu items to call the same function (slotShowItems) with the proper parameter
    this->show_items_mapper = new QSignalMapper(this);
    this->connect(this->ui.actionMList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
    this->connect(this->ui.actionVList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
    this->connect(this->ui.actionLList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
    this->connect(this->ui.actionFList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
    show_items_mapper->setMapping(this->ui.actionMList, "material");
    show_items_mapper->setMapping(this->ui.actionVList, "vase");
    show_items_mapper->setMapping(this->ui.actionLList, "lamp");
    show_items_mapper->setMapping(this->ui.actionFList, "flame");
    this->connect(show_items_mapper, &QSignalMapper::mappedString, this, &Funeraria::slotShowItems);
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    // Delete the table content
    this->clearTable();

    delete this->config;
    delete this->db;
    delete this->client_ui;
    delete this->tombUi;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
    delete this->material;
    delete this->show_items_mapper;
    delete this->new_item_mapper;
}

/********** SLOTS **********/
void Funeraria::slotFilterClientOrders()
{
    if (this->current_table != "tomb") {
        return;
    }

    this->slotClientOrders();
}

void Funeraria::slotClientOrders()
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    Tomb* tomb = new Tomb(this->db->db);

    this->current_table = "tomb";

    // Reset the table's content
    this->clearTable();

    int client_id = this->client->getId(this->ui.cbClient->currentText());
    int year;
    QString filter = this->ui.leDeceased->text().trimmed();

    if (this->ui.cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->ui.cbYear->currentText().toInt();
    }

    QList<QMap<QString, QString>> tombs = tomb->getList(client_id, year, filter);

    QStringList headers{ "Numero", "Defunto", "Materiale", "Prezzo", "Pagata", "Note", "Accessori",
        "Ordine", "Provino", "Conferma", "Incisione", "Consegna", "Azioni" };

    this->ui.tableWidget->setRowCount(tombs.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 60);    // Progressive
    this->ui.tableWidget->setColumnWidth(1, 230);   // Name
    this->ui.tableWidget->setColumnWidth(2, 150);   // Material
    this->ui.tableWidget->setColumnWidth(3, 60);    // Price
    this->ui.tableWidget->setColumnWidth(4, 60);    // Paid
    this->ui.tableWidget->setColumnWidth(5, 650);   // Notes
    this->ui.tableWidget->setColumnWidth(6, 80);    // Accessories mounted
    this->ui.tableWidget->setColumnWidth(7, 90);    // Ordered at
    this->ui.tableWidget->setColumnWidth(8, 90);    // Proofed at
    this->ui.tableWidget->setColumnWidth(9, 90);    // Confirmed at
    this->ui.tableWidget->setColumnWidth(10, 90);   // Engraved at
    this->ui.tableWidget->setColumnWidth(11, 90);   // Delivered at
    this->ui.tableWidget->setColumnWidth(12, 90);   // Buttons

    int row_number = 1;
    for (int i = 0; i < tombs.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");

        // Generate the cells' content and set them as not editable
        QTableWidgetItem* progressive = new QTableWidgetItem(tombs[i]["progressive"]);
        progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* name = new QTableWidgetItem(tombs[i]["name"]);
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* material = new QTableWidgetItem(this->material->getNameFromId(tombs[i]["material"]));
        material->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* price = new QTableWidgetItem(tombs[i]["price"]);
        price->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* paid = new QTableWidgetItem("");
        paid->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* notes = new QTableWidgetItem(tombs[i]["notes"]);
        notes->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* accessories_mounted = new QTableWidgetItem("");
        accessories_mounted->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString order_date = Helpers::dateSqlToIta(tombs[i]["ordered_at"]);
        QTableWidgetItem* ordered_at = new QTableWidgetItem(order_date);
        ordered_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString proof_date = Helpers::dateSqlToIta(tombs[i]["proofed_at"]);
        QTableWidgetItem* proofed_at = new QTableWidgetItem(proof_date);
        proofed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString confirm_date = Helpers::dateSqlToIta(tombs[i]["confirmed_at"]);
        QTableWidgetItem* confirmed_at = new QTableWidgetItem(confirm_date);
        confirmed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString engrave_date = Helpers::dateSqlToIta(tombs[i]["engraved_at"]);
        QTableWidgetItem* engraved_at = new QTableWidgetItem(engrave_date);
        engraved_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString deliver_date = Helpers::dateSqlToIta(tombs[i]["delivered_at"]);
        QTableWidgetItem* delivered_at = new QTableWidgetItem(deliver_date);
        delivered_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        if (row_number % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        if (deliver_date != "") {
            this->row_bg = this->tomb_delivered;
        }
        else if (confirm_date != "" && engrave_date == "") {
            this->row_bg = this->tomb_to_engrave;
        }

        this->paid_cell = this->row_bg;
        this->mounted_cell = this->row_bg;

        if (tombs[i]["paid"] == "0" && deliver_date != "" && tombs[i]["price"] != "0") {
            this->paid_cell = this->warning_bg;
        }

        if (tombs[i]["accessories_mounted"] == "0" && engrave_date != "") {
            this->mounted_cell = this->warning_bg;
        }

        progressive->setBackground(QBrush(this->row_bg));
        name->setBackground(QBrush(this->row_bg));
        material->setBackground(QBrush(this->row_bg));
        price->setBackground(QBrush(this->row_bg));
        paid->setBackground(QBrush(this->paid_cell));
        notes->setBackground(QBrush(this->row_bg));
        accessories_mounted->setBackground(QBrush(this->mounted_cell));
        ordered_at->setBackground(QBrush(this->row_bg));
        proofed_at->setBackground(QBrush(this->row_bg));
        confirmed_at->setBackground(QBrush(this->row_bg));
        engraved_at->setBackground(QBrush(this->row_bg));
        delivered_at->setBackground(QBrush(this->row_bg));

        this->ui.tableWidget->setItem(i, 0, progressive);
        this->ui.tableWidget->setItem(i, 1, name);
        this->ui.tableWidget->setItem(i, 2, material);
        this->ui.tableWidget->setItem(i, 3, price);
        this->ui.tableWidget->setItem(i, 4, paid);
        this->ui.tableWidget->setItem(i, 5, notes);
        this->ui.tableWidget->setItem(i, 6, accessories_mounted);
        this->ui.tableWidget->setItem(i, 7, ordered_at);
        this->ui.tableWidget->setItem(i, 8, proofed_at);
        this->ui.tableWidget->setItem(i, 9, confirmed_at);
        this->ui.tableWidget->setItem(i, 10, engraved_at);
        this->ui.tableWidget->setItem(i, 11, delivered_at);
        this->ui.tableWidget->setCellWidget(i, 12, pb_details); // Details button

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotTombDetails);

        row_number++;
    }

    delete tomb;
}

void Funeraria::slotTombDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    this->tombUi->setProgressive(this->ui.tableWidget->item(row, 0)->text().toInt());
    this->tombUi->setModal(true);
    this->tombUi->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotClientOrders();
}

void Funeraria::slotNewTomb()
{
    this->current_table = "tomb";

    // Set the name property of the Client object to an empty string
    this->tombUi->setProgressive(0);
    this->tombUi->setModal(true);
    this->tombUi->exec();

    // If the table shows any tomb
    if (this->ui.tableWidget->rowCount() > 0) {
        // Reload the table when the popup is closed, the user could have made some changes
        this->slotClientOrders();
    }
}

void Funeraria::slotShowClients()
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->current_table = "client";

    // Reset the table's content
    this->clearTable();

    QList<QMap<QString, QString>> clients = this->client->get();

    QStringList headers{ "Ordine", "Nome", "Email", "Telefono", "", "" };

    this->ui.tableWidget->setRowCount(clients.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 60);
    this->ui.tableWidget->setColumnWidth(1, 300);
    this->ui.tableWidget->setColumnWidth(2, 300);
    this->ui.tableWidget->setColumnWidth(3, 300);
    this->ui.tableWidget->setColumnWidth(4, 90);
    this->ui.tableWidget->setColumnWidth(5, 90);

    int row_number = 1;
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

        QTableWidgetItem* position_widget = new QTableWidgetItem(clients[i]["position"]);
        // Set the field as not editable
        position_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* name_widget = new QTableWidgetItem(clients[i]["name"]);
        // Set the field as not editable
        name_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* emails_widget = new QTableWidgetItem(emails);
        // Set the field as not editable
        emails_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* phones_widget = new QTableWidgetItem(phones);
        // Set the field as not editable
        phones_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        if (row_number % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        position_widget->setBackground(QBrush(row_bg));
        name_widget->setBackground(QBrush(row_bg));
        emails_widget->setBackground(QBrush(row_bg));
        phones_widget->setBackground(QBrush(row_bg));

        this->ui.tableWidget->setItem(i, 0, position_widget);
        this->ui.tableWidget->setItem(i, 1, name_widget);
        this->ui.tableWidget->setItem(i, 2, emails_widget);
        this->ui.tableWidget->setItem(i, 3, phones_widget);
        this->ui.tableWidget->setCellWidget(i, 4, pb_details);
        this->ui.tableWidget->setCellWidget(i, 5, pb_delete);

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotClientDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDelete);

        row_number++;
    }
}

void Funeraria::slotClientDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the name property of the Client object to the name present in the clicked row
    this->client_ui->setName(this->ui.tableWidget->item(row, 1)->text());
    this->client_ui->setModal(true);
    this->client_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowClients();
}

void Funeraria::slotNewClient()
{
    this->current_table = "client";

    // Set the name property of the Client object to an empty string
    this->client_ui->setName("");
    this->client_ui->setModal(true);
    this->client_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowClients();
}

void Funeraria::slotShowItems(const QString& type)
{
    // Block the signals while building the table
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

    QStringList headers{ "Codice", "Nome", "Azioni" };

    this->ui.tableWidget->setRowCount(accessories.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 70);
    this->ui.tableWidget->setColumnWidth(1, 300);
    this->ui.tableWidget->setColumnWidth(2, 90);

    int row_number = 1;
    for (int i = 0; i < accessories.size(); i++) {
        QTableWidgetItem* code = new QTableWidgetItem(accessories[i]["code"]);
        // Set the field as not editable
        code->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QTableWidgetItem* name = new QTableWidgetItem(accessories[i]["name"]);
        // Set the field as not editable
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        QPushButton* pb = new QPushButton(this->ui.tableWidget);
        pb->setText("Elimina");

        if (row_number % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        code->setBackground(QBrush(row_bg));
        name->setBackground(QBrush(row_bg));

        this->ui.tableWidget->setItem(i, 0, code);
        this->ui.tableWidget->setItem(i, 1, name);
        this->ui.tableWidget->setCellWidget(i, 2, pb); // Delete button
        this->connect(pb, &QPushButton::clicked, this, &Funeraria::slotDelete);

        row_number++;
    }
}

void Funeraria::slotNewItem(const QString& type)
{
    this->current_table = type;

    if (type == "vase") {
        AccessoryUi* vaseUi = new AccessoryUi(this->db->db, "vases", this);
        vaseUi->setModal(true);
        vaseUi->exec();

        delete vaseUi;
    }
    else if (type == "lamp") {
        AccessoryUi* lampUi = new AccessoryUi(this->db->db, "lamp", this);
        lampUi->setModal(true);
        lampUi->exec();

        delete lampUi;
    }
    else if (type == "flame") {
        AccessoryUi* flameUi = new AccessoryUi(this->db->db, "flame", this);
        flameUi->setModal(true);
        flameUi->exec();

        delete flameUi;
    }
    else if (type == "material") {
        AccessoryUi* materialUi = new AccessoryUi(this->db->db, "material", this);
        materialUi->setModal(true);
        materialUi->exec();

        delete materialUi;
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

void Funeraria::slotTombsToEngrave()
{
    Tomb* tomb = new Tomb(this->db->db);

    QList<QMap<QString, QString>> tombs = tomb->tombsToEngrave();

    if (tombs.size() > 0) {
        // Block the signals while building the table
        const QSignalBlocker blocker(this->ui.tableWidget);

        this->current_table = "tomb";

        // Reset the table's content
        this->clearTable();

        QStringList headers{ "Numero", "Defunto", "Materiale", "Cliente" };

        this->ui.tableWidget->setRowCount(tombs.size());
        this->ui.tableWidget->setColumnCount(headers.size());
        this->ui.tableWidget->setHorizontalHeaderLabels(headers);

        this->ui.tableWidget->setColumnWidth(0, 90);
        this->ui.tableWidget->setColumnWidth(1, 250);
        this->ui.tableWidget->setColumnWidth(2, 250);
        this->ui.tableWidget->setColumnWidth(3, 200);

        int row_number = 1;
        for (int i = 0; i < tombs.size(); i++) {
            QTableWidgetItem* progressive = new QTableWidgetItem(tombs[i]["progressive"]);
            // Set the field as not editable
            progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* deceased = new QTableWidgetItem(tombs[i]["deceased"]);
            // Set the field as not editable
            deceased->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* material = new QTableWidgetItem(tombs[i]["material"]);
            // Set the field as not editable
            material->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* client = new QTableWidgetItem(tombs[i]["client"]);
            // Set the field as not editable
            client->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            if (row_number % 2 == 0) {
                this->row_bg = this->row_even;
            }
            else {
                this->row_bg = this->row_odd;
            }

            progressive->setBackground(QBrush(row_bg));
            deceased->setBackground(QBrush(row_bg));
            material->setBackground(QBrush(row_bg));
            client->setBackground(QBrush(row_bg));

            this->ui.tableWidget->setItem(i, 0, progressive);
            this->ui.tableWidget->setItem(i, 1, deceased);
            this->ui.tableWidget->setItem(i, 2, material);
            this->ui.tableWidget->setItem(i, 3, client);

            row_number++;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non risultano lapidi da incidere.");
        message.exec();
    }

    delete tomb;
}

void Funeraria::slotAccessoriesToMount()
{
    Tomb* tomb = new Tomb(this->db->db);

    QList<QMap<QString, QString>> accessories = tomb->accessoriesToMount();

    if (accessories.size() > 0) {
        // Block the signals while building the table
        const QSignalBlocker blocker(this->ui.tableWidget);

        this->current_table = "accessories";

        // Reset the table's content
        this->clearTable();

        QStringList headers{ "Defunto", "Materiali", "Vasi", "Lampade", "Fiamme", "Cliente" };

        this->ui.tableWidget->setRowCount(accessories.size());
        this->ui.tableWidget->setColumnCount(headers.size());
        // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        this->ui.tableWidget->setHorizontalHeaderLabels(headers);

        this->ui.tableWidget->setColumnWidth(0, 250);
        this->ui.tableWidget->setColumnWidth(1, 300);
        this->ui.tableWidget->setColumnWidth(2, 200);
        this->ui.tableWidget->setColumnWidth(3, 200);
        this->ui.tableWidget->setColumnWidth(4, 200);
        this->ui.tableWidget->setColumnWidth(5, 300);

        int row_number = 1;
        for (int i = 0; i < accessories.size(); i++) {
            QTableWidgetItem* deceased = new QTableWidgetItem(accessories[i]["deceased"]);
            // Set the field as not editable
            deceased->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* material = new QTableWidgetItem(accessories[i]["material"]);
            // Set the field as not editable
            material->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* vase = new QTableWidgetItem(accessories[i]["vase"]);
            // Set the field as not editable
            vase->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* lamp = new QTableWidgetItem(accessories[i]["lamp"]);
            // Set the field as not editable
            lamp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* flame = new QTableWidgetItem(accessories[i]["flame"]);
            // Set the field as not editable
            flame->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* client = new QTableWidgetItem(accessories[i]["client"]);
            // Set the field as not editable
            client->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            if (row_number % 2 == 0) {
                this->row_bg = this->row_even;
            }
            else {
                this->row_bg = this->row_odd;
            }

            deceased->setBackground(QBrush(row_bg));
            material->setBackground(QBrush(row_bg));
            vase->setBackground(QBrush(row_bg));
            lamp->setBackground(QBrush(row_bg));
            flame->setBackground(QBrush(row_bg));
            client->setBackground(QBrush(row_bg));

            this->ui.tableWidget->setItem(i, 0, deceased);
            this->ui.tableWidget->setItem(i, 1, material);
            this->ui.tableWidget->setItem(i, 2, vase);
            this->ui.tableWidget->setItem(i, 3, lamp);
            this->ui.tableWidget->setItem(i, 4, flame);
            this->ui.tableWidget->setItem(i, 5, client);

            row_number++;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non risultano lapidi con accessori da montare.");
        message.exec();
    }

    delete tomb;
}

void Funeraria::slotTombsNotPaid()
{
    Tomb* tomb = new Tomb(this->db->db);

    QList<QMap<QString, QString>> tombs = tomb->tombsToPay();

    if (tombs.size() > 0) {
        // Block the signals while building the table
        const QSignalBlocker blocker(this->ui.tableWidget);

        this->current_table = "tomb";

        // Reset the table's content
        this->clearTable();

        QStringList headers{ "Defunto", "Prezzo", "Cliente" };

        this->ui.tableWidget->setRowCount(tombs.size());
        this->ui.tableWidget->setColumnCount(headers.size());
        this->ui.tableWidget->setHorizontalHeaderLabels(headers);

        this->ui.tableWidget->setColumnWidth(0, 250);
        this->ui.tableWidget->setColumnWidth(1, 90);
        this->ui.tableWidget->setColumnWidth(2, 200);

        int row_number = 1;
        for (int i = 0; i < tombs.size(); i++) {
            QTableWidgetItem* deceased = new QTableWidgetItem(tombs[i]["deceased"]);
            // Set the field as not editable
            deceased->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* price = new QTableWidgetItem(tombs[i]["price"]);
            // Set the field as not editable
            price->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            QTableWidgetItem* client = new QTableWidgetItem(tombs[i]["client"]);
            // Set the field as not editable
            client->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            if (row_number % 2 == 0) {
                this->row_bg = this->row_even;
            }
            else {
                this->row_bg = this->row_odd;
            }

            deceased->setBackground(QBrush(row_bg));
            price->setBackground(QBrush(row_bg));
            client->setBackground(QBrush(row_bg));

            this->ui.tableWidget->setItem(i, 0, deceased);
            this->ui.tableWidget->setItem(i, 1, price);
            this->ui.tableWidget->setItem(i, 2, client);

            row_number++;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non risultano lapidi da pagare.");
        message.exec();
    }

    delete tomb;
}

/********** PRIVATE FUNCTIONS **********/

void Funeraria::clearTable()
{
    for (int i = 0; i < this->ui.tableWidget->rowCount(); i++) {
        for (int j = 0; j < this->ui.tableWidget->columnCount(); j++) {
            // Usually the last column holds a button
            if (j == this->ui.tableWidget->columnCount() - 1) {
                QPushButton* pbutton = qobject_cast<QPushButton*>(this->ui.tableWidget->cellWidget(i, j));
                if (pbutton) {
                    pbutton->disconnect(); // Disconnect any connections
                    pbutton->setParent(nullptr); // Reparent the button
                    delete pbutton;
                    pbutton = nullptr;
                }
                else {
                    QTableWidgetItem* item = this->ui.tableWidget->item(i, j);
                    delete item;
                    item = nullptr;
                }
            }
            else {
                QTableWidgetItem* item = this->ui.tableWidget->item(i, j);
                delete item;
                item = nullptr;
            }
        }
    }

    this->ui.tableWidget->clear();
}

void Funeraria::closeWindow()
{
    this->close();
}
