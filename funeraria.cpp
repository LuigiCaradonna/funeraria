#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget* parent)
    : QMainWindow(parent)
{
    /*
     * Before to setup the application: check the config file and the database
     */

    // Check the config file before to instantiate the DatabaseManager object,
    // DatabaseManager needs to read the configuration file and we want to be sure
    // that the configuration is correctly set.
    this->config = new Config();
    
    // The config file is not valid
    if (!this->config->loaded) {
        // Close the application
        this->closeWindow();
    }

    this->db = new DatabaseManager(this);

    // The connection failed
    if (!this->db->connected) {
        // Close the application
        this->closeWindow();
    }
    else {
        this->ui.setupUi(this);

        // Sets an icon for the window
        this->setWindowIcon(QIcon("funeraria.png"));

        // Set the font for the table
        QFont font("Calibri", 12);
        this->ui.tableWidget->setFont(font);
        this->connect(this->ui.tableWidget->horizontalHeader(), &QHeaderView::sectionClicked, this, &Funeraria::slotHeaderClicked);

        // Connect the customContextMenuRequested signal to a slot
        connect(this->ui.tableWidget, &QTableWidget::customContextMenuRequested, this, &Funeraria::slotShowContextMenu);

        this->context_menu = new QMenu(this);
        this->client = new Client(this->db->db);
        this->client_ui = new ClientUi(this->db->db, this);
        this->settings_ui = new SettingsUi(this->db->db, this);
        this->tombUi = new TombUi(this->db->db, this);
        this->vase = new Accessory(this->db->db, "vases");
        this->lamp = new Accessory(this->db->db, "lamps");
        this->flame = new Accessory(this->db->db, "flames");
        this->material = new Accessory(this->db->db, "materials");

        // Insert the clients' names into the combobox
        this->updateClientsCombobox();

        // Populate the years combo box, years go back until 2020
        this->ui.cbYear->addItem("Tutti");
        QString this_year = QDate::currentDate().toString("yyyy");
        for (int i = this_year.toInt(); i >= 2020; i--) {
            this->ui.cbYear->addItem(QString::number(i));
        }

        // Set the current index to the current year (index 0 is "Tutti")
        this->ui.cbYear->setCurrentIndex(1);

        // Set the event listeners for main UI's elements
        this->connect(this->ui.btnToEngrave, &QPushButton::clicked, this, &Funeraria::slotTombsToEngrave);
        this->connect(this->ui.btnNewTomb, &QPushButton::clicked, this, &Funeraria::slotNewTomb);
        this->connect(this->ui.btnSearch, &QPushButton::clicked, this, &Funeraria::slotClientOrders);
        this->connect(this->ui.leDeceased, &QLineEdit::textChanged, this, &Funeraria::slotFilterClientOrders);

        // Signal emitted from the menu "Files"
        this->connect(this->ui.actionBackupCSV, SIGNAL(triggered()), this, SLOT(slotBackupDbToCSV()));
        this->connect(this->ui.actionSettings, SIGNAL(triggered()), this, SLOT(slotShowSettings()));

        // Signal emitted from the menu "Clienti"
        this->connect(this->ui.actionCList, SIGNAL(triggered()), this, SLOT(slotShowClients()));
        this->connect(this->ui.actionCNew, SIGNAL(triggered()), this, SLOT(slotNewClient()));

        // Signal emitted from the menu "Lapidi"
        this->connect(this->ui.actionTNew, SIGNAL(triggered()), this, SLOT(slotNewTomb()));
        this->connect(this->ui.actionTEngrave, SIGNAL(triggered()), this, SLOT(slotTombsToEngrave()));
        this->connect(this->ui.actionMAccessories, SIGNAL(triggered()), this, SLOT(slotAccessoriesToMount()));
        this->connect(this->ui.actionTPay, SIGNAL(triggered()), this, SLOT(slotTombsNotPaid()));
        this->connect(this->ui.actionSearchByProgressive, SIGNAL(triggered()), this, SLOT(slotTombByProgressive()));

        // Map the signal coming from the menu "Accessori" to call the same function (slotNewItem) with the proper parameter
        this->new_item_mapper = new QSignalMapper(this);
        this->connect(this->ui.actionMNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionVNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionLNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionFNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        new_item_mapper->setMapping(this->ui.actionMNew, "materials");
        new_item_mapper->setMapping(this->ui.actionVNew, "vases");
        new_item_mapper->setMapping(this->ui.actionLNew, "lamps");
        new_item_mapper->setMapping(this->ui.actionFNew, "flames");
        this->connect(new_item_mapper, &QSignalMapper::mappedString, this, &Funeraria::slotNewItem);

        // Map the signal coming from the menu "Accessori" to call the same function (slotShowItems) with the proper parameter
        this->show_items_mapper = new QSignalMapper(this);
        this->connect(this->ui.actionMList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionVList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionLList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionFList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        show_items_mapper->setMapping(this->ui.actionMList, "materials");
        show_items_mapper->setMapping(this->ui.actionVList, "vases");
        show_items_mapper->setMapping(this->ui.actionLList, "lamps");
        show_items_mapper->setMapping(this->ui.actionFList, "flames");
        this->connect(show_items_mapper, &QSignalMapper::mappedString, this, &Funeraria::slotShowItems);

        // Create a qPushButton for each client for the quick access bar
        this->updateQuickAccessNames();
    }
}

/********** DESTRUCTOR **********/

Funeraria::~Funeraria()
{
    // Delete the table content
    this->clearTable();

    delete this->db;
    delete this->context_menu;
    delete this->client_ui;
    delete this->settings_ui;
    delete this->tombUi;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
    delete this->material;
    delete this->show_items_mapper;
    delete this->new_item_mapper;
}

/********** SLOTS **********/
void Funeraria::slotShowContextMenu(const QPoint& pos) {
    // Clear from the menu the eventual entries shown before
    this->context_menu->clear();

    // Create the actions according to what is the selection or the point where clicked
    QAction* sumPrices = this->context_menu->addAction("Somma i prezzi selezionati");
    QAction* printList = this->context_menu->addAction("Genera lista");
    
    // Connect actions to slots
    connect(sumPrices, &QAction::triggered, this, &Funeraria::slotSumSelectedPrices);
    connect(printList, &QAction::triggered, this, &Funeraria::slotPrintToPayList);

    // Position where to show the context menu
    this->context_menu->popup(this->ui.tableWidget->viewport()->mapToGlobal(pos));
}

void Funeraria::slotSumSelectedPrices() {
    QList<QTableWidgetItem*> items = this->ui.tableWidget->selectedItems();

    if (items.isEmpty()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Nessun prezzo selezionato");
        message.exec();
        return;
    }

    float sum = 0;
    int prices = 0;
    for (int i = 0; i < items.size(); i++) {

        if (items[i]->data(Qt::DisplayRole).toString() != "0" && !items[i]->data(Qt::DisplayRole).toFloat()) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Warning);
            message.setText("Nella selezione sono presenti valori non numerici");
            message.exec();
            return;
        }

        sum += items[i]->data(Qt::DisplayRole).toFloat();
        prices++;
    }

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("La somma delle " +  QString::number(prices) + " lapidi selezionte è: € " + QString::number(sum));
    message.exec();
}

void Funeraria::slotPrintToPayList() {
    QList<QTableWidgetItem*> items = this->ui.tableWidget->selectedItems();

    if (items.isEmpty()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Nessun lavoro selezionato");
        message.exec();
        return;
    }

    // Get the top left and bottom right index of any selection made on the table
    QList<QTableWidgetSelectionRange> ranges = this->ui.tableWidget->selectedRanges();

    // Filter to show only txt files
    QString filter = "txt(*.txt)";
    QString filename = QFileDialog::getSaveFileName(this, tr("save_to"), "", filter);

    // If no file name is provided, stop the execution, the user just closed the dialog window
    if (filename.trimmed() == "") return;

    QFile list_file(filename);

    QTextStream out(&list_file);

    if (!list_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è possibile aprire il file selezionato, verificare che il percorso non sia protetto da scrittura.");
        message.exec();
        return;
    }

    double total = 0;

    // For all the selections
    for (int i = 0; i < ranges.size(); i++) {
        // Loop through the first and last row
        for (int j = ranges[i].topRow(); j <= ranges[i].bottomRow(); j++) {
            // If a name is defined, the material and the description are not necessary
            if (this->ui.tableWidget->item(j, 1)->data(Qt::DisplayRole).toString() != this->name_not_defined) {
                // Print the data of each row
                out << this->ui.tableWidget->item(j, 1)->data(Qt::DisplayRole).toString() + " "; // Name
            }
            else {
                // Print the data of each row
                out << this->ui.tableWidget->item(j, 2)->data(Qt::DisplayRole).toString() + " "; // Material
                out << this->ui.tableWidget->item(j, 5)->data(Qt::DisplayRole).toString() + " "; // Notes
            }

            out << "€ " + this->ui.tableWidget->item(j, 3)->data(Qt::DisplayRole).toString() + "\n\n"; // Price

            total += this->ui.tableWidget->item(j, 3)->data(Qt::DisplayRole).toDouble();
        }
    }

    out << "Totale € " + QString::number(total); // Total price

    list_file.close();

    QMessageBox message;
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Information);
    message.setText("Il file è stato generato.");
    message.exec();
    return;
}

void Funeraria::slotHeaderClicked(int logical_index) {
    // Get the clicked label text
    QString label = this->ui.tableWidget->horizontalHeader()->model()->headerData(logical_index, Qt::Horizontal).toString();
    QString sort_by = this->db->getSortableColumnName(label);

    // The selected column can't be sorted
    if (!this->is_table_sortable || sort_by == "") {
        return;
    }

    // Decide the sorting direction
    this->sortColumnDirection(label);

    Tomb* tomb = new Tomb(this->db->db);

    this->current_table = "tombs";

    int client_id = this->client->getId(this->ui.cbClient->currentText());
    int year;
    QString name = this->ui.leDeceased->text().trimmed();

    if (this->ui.cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->ui.cbYear->currentText().toInt();
    }

    QList<QMap<QString, QString>> tombs = 
        tomb->getList(client_id, year, this->ui.chbEngraved->isChecked(), name, sort_by, this->sort_column_direction);

    this->showClientOrders(tombs);

    delete tomb;
}

void Funeraria::slotBackupDbToCSV() {
    QMessageBox message;
    message.setWindowTitle("Funeraria");
    if (this->db->backupToCSV()) {
        message.setIcon(QMessageBox::Information);
        message.setText("Esportazione del DB in CSV eseguita.");
    }
    else {
        message.setIcon(QMessageBox::Critical);
        message.setText("Esportazione del DB in CSV fallita.");
    }
    message.exec();
}

void Funeraria::slotShowSettings() {
    this->settings_ui->setModal(true);
    this->settings_ui->exec();
}

void Funeraria::slotFilterClientOrders()
{
    if (this->current_table != "tombs") {
        return;
    }

    this->slotClientOrders();
}

void Funeraria::slotClientOrders()
{
    Tomb* tomb = new Tomb(this->db->db);

    this->current_table = "tombs";

    int client_id = this->client->getId(this->ui.cbClient->currentText());
    int year;
    QString name = this->ui.leDeceased->text().trimmed();

    if (this->ui.cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->ui.cbYear->currentText().toInt();
    }

    QList<QMap<QString, QString>> tombs = tomb->getList(client_id, year, this->ui.chbEngraved->isChecked(), name);

    this->showClientOrders(tombs);

    delete tomb;
}

void Funeraria::slotQuickClientOrders()
{
    QString button_text;

    // Get the sender of the signal
    QPushButton* sender_button = qobject_cast<QPushButton*>(sender());

    if (sender_button) {
        button_text = sender_button->text();
    }
    else {
        return;
    }

    Tomb* tomb = new Tomb(this->db->db);

    this->current_table = "tombs";

    int client_id = this->client->getId(button_text);
    int year = QDate::currentDate().toString("yyyy").toInt();

    this->ui.cbClient->setCurrentText(button_text);
    this->ui.cbYear->setCurrentText(QDate::currentDate().toString("yyyy"));
    this->ui.leDeceased->setText("");

    // Search params: client's id, current year, no filter for the deceased's name
    QList<QMap<QString, QString>> tombs = tomb->getList(client_id, year, this->ui.chbEngraved->isChecked());

    this->showClientOrders(tombs);

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

void Funeraria::slotTombFolder()
{
    Tomb* tomb = new Tomb(this->db->db);

    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();

    int progressive = this->ui.tableWidget->item(row, 0)->text().toInt();
    QString name = this->ui.tableWidget->item(row, 1)->text();

    QString path = tomb->getFolderPath(progressive, name);

    delete tomb;
    QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

void Funeraria::slotNewTomb()
{
    this->current_table = "tombs";

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

    this->current_table = "clients";

    this->is_table_sortable = false;

    // Reset the table's content
    this->clearTable();

    QList<QMap<QString, QString>> clients = this->client->get();

    QStringList headers{ "Posizione", "Nome", "Email", "Telefono", "", "" };

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
        pb_details->setText("Modifica");
        QPushButton* pb_delete = new QPushButton(this->ui.tableWidget);
        pb_delete->setText("Elimina");

        QStringList emails_list = clients[i]["email"].split(u',');
        int num_emails = emails_list.length();
        QString emails = "";
        for (int j = 0; j < num_emails; j++) {
            QString nl = "\n";
            if (j == num_emails - 1) {
                nl = ""; // No new line after the last email address
            }
            emails += emails_list[j] + nl;
        }

        QStringList phones_list = clients[i]["phone"].split(u',');
        int num_phones = phones_list.length();
        QString phones = "";
        for (int j = 0; j < num_phones; j++) {
            QString nl = "\n";
            if (j == num_phones - 1) {
                nl = ""; // No new line after the last phone number
            }
            phones += phones_list[j] + nl;
        }

        // Phone numbers and email addresses are shown one per line
        // get the maximum number of lines to calculate the row height
        int lines = std::max(num_emails, num_phones);

        // If the client has no email and phone numbers
        if (lines == 0) {
            // set the lines value to 1
            lines = 1;
        }

        // Set the row height to 21px multiplied by the number of lines
        this->ui.tableWidget->setRowHeight(i, 30*lines);

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
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDeleteItem);

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

    // Clear the combobox content
    this->ui.cbClient->clear();
    // Update the combobox with the active clients
    this->updateClientsCombobox();
    // Update the quick access bar with the active clients
    this->updateQuickAccessNames();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowClients();
}

void Funeraria::slotNewClient()
{
    this->current_table = "clients";

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

    this->is_table_sortable = false;

    // Reset the table's content
    this->ui.tableWidget->clear();

    QList<QMap<QString, QString>> accessories;

    // Get the the list of the required accessories
    if (type == "vases") {
        accessories = this->vase->get();
    }
    else if (type == "lamps") {
        accessories = this->lamp->get();
    }
    else if (type == "flames") {
        accessories = this->flame->get();
    }
    else if (type == "materials") {
        accessories = this->material->get();
    }
    else {
        // The type requested is not valid
        return;
    }

    QStringList headers{ "Codice", "Nome", "", ""};

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

        QPushButton* pb_edit = new QPushButton(this->ui.tableWidget);
        pb_edit->setText("Modifica");

        QPushButton* pb_del = new QPushButton(this->ui.tableWidget);
        pb_del->setText("Elimina");

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
        this->ui.tableWidget->setCellWidget(i, 2, pb_edit); // Edit button
        this->ui.tableWidget->setCellWidget(i, 3, pb_del); // Delete button
        this->connect(pb_edit, &QPushButton::clicked, this, &Funeraria::slotEditItem);
        this->connect(pb_del, &QPushButton::clicked, this, &Funeraria::slotDeleteItem);

        row_number++;
    }
}

void Funeraria::slotNewItem(const QString& type)
{
    this->current_table = type;

    if (type == "vases") {
        AccessoryUi* vase_ui = new AccessoryUi(this->db->db, "vases", this);
        vase_ui->updateForm();
        vase_ui->setModal(true);
        vase_ui->exec();

        delete vase_ui;
    }
    else if (type == "lamps") {
        AccessoryUi* lamp_ui = new AccessoryUi(this->db->db, "lamps", this);
        lamp_ui->updateForm();
        lamp_ui->setModal(true);
        lamp_ui->exec();

        delete lamp_ui;
    }
    else if (type == "flames") {
        AccessoryUi* flame_ui = new AccessoryUi(this->db->db, "flames", this);
        flame_ui->updateForm();
        flame_ui->setModal(true);
        flame_ui->exec();

        delete flame_ui;
    }
    else if (type == "materials") {
        AccessoryUi* material_ui = new AccessoryUi(this->db->db, "materials", this);
        material_ui->updateForm();
        material_ui->setModal(true);
        material_ui->exec();

        delete material_ui;
    }
    else {
        // The type requested is not valid
        return;
    }

    this->slotShowItems(this->current_table);
}

void Funeraria::slotEditItem() {
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the name property of the Client object to the name present in the clicked row
    if (this->current_table == "vases") {
        AccessoryUi* vase_ui = new AccessoryUi(this->db->db, "vases", this);
        vase_ui->updateForm(this->ui.tableWidget->item(row, 0)->text(), this->ui.tableWidget->item(row, 1)->text());
        vase_ui->setModal(true);
        vase_ui->exec();

        delete vase_ui;
    }
    else if (this->current_table == "lamps") {
        AccessoryUi* lamp_ui = new AccessoryUi(this->db->db, "lamps", this);
        lamp_ui->updateForm(this->ui.tableWidget->item(row, 0)->text(), this->ui.tableWidget->item(row, 1)->text());
        lamp_ui->setModal(true);
        lamp_ui->exec();

        delete lamp_ui;
    }
    else if (this->current_table == "flames") {
        AccessoryUi* flame_ui = new AccessoryUi(this->db->db, "flames", this);
        flame_ui->updateForm(this->ui.tableWidget->item(row, 0)->text(), this->ui.tableWidget->item(row, 1)->text());
        flame_ui->setModal(true);
        flame_ui->exec();

        delete flame_ui;
    }
    else if (this->current_table == "materials") {
        AccessoryUi* material_ui = new AccessoryUi(this->db->db, "materials", this);
        material_ui->updateForm(this->ui.tableWidget->item(row, 0)->text(), this->ui.tableWidget->item(row, 1)->text());
        material_ui->setModal(true);
        material_ui->exec();

        delete material_ui;
    }

    this->slotShowItems(this->current_table);
}

void Funeraria::slotDeleteItem() {
    int row = this->ui.tableWidget->currentRow();

    QMessageBox message;
    QPushButton* proceed_btn = message.addButton("Elimina", QMessageBox::ActionRole);
    QPushButton* abort_btn = message.addButton("Annulla", QMessageBox::ActionRole);
    message.setWindowTitle("Funeraria");
    message.setIcon(QMessageBox::Warning);
    message.setText("Vuoi eliminare questo elemento?");
    message.exec();

    if (message.clickedButton() == proceed_btn) {
        // Delete the item

        if (this->current_table == "tombs") {

        }
        else if (this->current_table == "vases") {
            this->vase->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "lamps") {
            this->lamp->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "flames") {
            this->flame->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "materials") {
            this->material->remove(this->ui.tableWidget->item(row, 0)->text());
        }
        else if (this->current_table == "clients") {
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

    this->is_table_sortable = false;

    if (tombs.size() > 0) {
        this->setupTombsToEngraveTable(tombs.size());

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

            QString date = Helpers::dateSqlToIta(tombs[i]["confirmed_at"]);
            QTableWidgetItem* confirmed_at = new QTableWidgetItem(date);
            // Set the field as not editable
            confirmed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
            pb_details->setText("Dettagli");

            QPushButton* pb_set_engraved = new QPushButton(this->ui.tableWidget);
            pb_set_engraved->setText("Incisa");

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
            confirmed_at->setBackground(QBrush(row_bg));

            this->ui.tableWidget->setItem(i, 0, progressive);
            this->ui.tableWidget->setItem(i, 1, deceased);
            this->ui.tableWidget->setItem(i, 2, material);
            this->ui.tableWidget->setItem(i, 3, client);
            this->ui.tableWidget->setItem(i, 4, confirmed_at);
            this->ui.tableWidget->setCellWidget(i, 5, pb_details); // Details button
            this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotTombDetails);

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

    this->is_table_sortable = false;

    if (accessories.size() > 0) {
        this->setupAccessoriesToMountTable(accessories.size());

        int row_number = 1;
        for (int i = 0; i < accessories.size(); i++) {
            QTableWidgetItem* progressive = new QTableWidgetItem(accessories[i]["progressive"]);
            // Set the field as not editable
            progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

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

            QPushButton* pb_set_mounted = new QPushButton(this->ui.tableWidget);
            pb_set_mounted->setText("Montati");

            if (row_number % 2 == 0) {
                this->row_bg = this->row_even;
            }
            else {
                this->row_bg = this->row_odd;
            }

            progressive->setBackground(QBrush(row_bg));
            deceased->setBackground(QBrush(row_bg));
            material->setBackground(QBrush(row_bg));
            vase->setBackground(QBrush(row_bg));
            lamp->setBackground(QBrush(row_bg));
            flame->setBackground(QBrush(row_bg));
            client->setBackground(QBrush(row_bg));

            this->ui.tableWidget->setItem(i, 0, progressive);
            this->ui.tableWidget->setItem(i, 1, deceased);
            this->ui.tableWidget->setItem(i, 2, material);
            this->ui.tableWidget->setItem(i, 3, vase);
            this->ui.tableWidget->setItem(i, 4, lamp);
            this->ui.tableWidget->setItem(i, 5, flame);
            this->ui.tableWidget->setItem(i, 6, client);
            this->ui.tableWidget->setCellWidget(i, 7, pb_set_mounted); // Set mounted accessories button

            this->connect(pb_set_mounted, &QPushButton::clicked, this, &Funeraria::slotSetAccessoriesMounted);

            row_number++;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non risultano lapidi con accessori da montare.");
        message.exec();

        this->setupAccessoriesToMountTable(0);
    }

    delete tomb;
}

void Funeraria::slotTombsNotPaid()
{
    Tomb* tomb = new Tomb(this->db->db);

    QList<QMap<QString, QString>> tombs = tomb->tombsToPay();

    this->is_table_sortable = false;

    if (tombs.size() > 0) {
        this->setupTombsNotPaidTable(tombs.size());

        int row_number = 1;
        for (int i = 0; i < tombs.size(); i++) {
            QTableWidgetItem* progressive = new QTableWidgetItem(tombs[i]["progressive"]);
            // Set the field as not editable
            progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            QTableWidgetItem* deceased = new QTableWidgetItem(tombs[i]["deceased"]);
            // Set the field as not editable
            deceased->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            QTableWidgetItem* price = new QTableWidgetItem(tombs[i]["price"]);
            // Set the field as not editable
            price->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            QTableWidgetItem* client = new QTableWidgetItem(tombs[i]["client"]);
            // Set the field as not editable
            client->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            QPushButton* pb_set_paid = new QPushButton(this->ui.tableWidget);
            pb_set_paid->setText("Pagata");

            if (row_number % 2 == 0) {
                this->row_bg = this->row_even;
            }
            else {
                this->row_bg = this->row_odd;
            }

            progressive->setBackground(QBrush(row_bg));
            deceased->setBackground(QBrush(row_bg));
            price->setBackground(QBrush(row_bg));
            client->setBackground(QBrush(row_bg));

            this->ui.tableWidget->setItem(i, 0, progressive);
            this->ui.tableWidget->setItem(i, 1, deceased);
            this->ui.tableWidget->setItem(i, 2, price);
            this->ui.tableWidget->setItem(i, 3, client);
            this->ui.tableWidget->setCellWidget(i, 4, pb_set_paid); // Set mounted accessories button

            this->connect(pb_set_paid, &QPushButton::clicked, this, &Funeraria::slotSetPaidTomb);

            row_number++;
        }
    }
    else {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non risultano lapidi da pagare.");
        message.exec();

        this->setupTombsNotPaidTable(0);
    }

    delete tomb;
}

void Funeraria::slotTombByProgressive()
{
    bool ok;

    QString progressive = QInputDialog::getText(this, "Numero di lapide da cercare",
        "Numero: ", QLineEdit::Normal, "", &ok);

    if (ok && !progressive.isEmpty()) {
        Tomb* tomb = new Tomb(this->db->db);

        QMap<QString, QString> item = tomb->getByProgressive(progressive.toInt());

        if (!item.isEmpty())
            this->showClientOrder(item);

        delete tomb;
    }
}

void Funeraria::slotSetPaidTomb()
{
    Tomb* tomb = new Tomb(this->db->db);

    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    int progressive = this->ui.tableWidget->item(row, 0)->text().toInt();

    if (!tomb->setPaid(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile impostare la lapide come pagata.");
        message.exec();
    }
    else {
        // Reload the table
        this->slotClientOrders();
    }
    
    delete tomb;
}

void Funeraria::slotSetAccessoriesMounted()
{
    Tomb* tomb = new Tomb(this->db->db);

    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    int progressive = this->ui.tableWidget->item(row, 0)->text().toInt();

    if (!tomb->setAccessoriesMounted(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile impostare gli accessori come montati.");
        message.exec();
    }
    else {
        // Reload the table
        this->slotAccessoriesToMount();
    }

    delete tomb;
}
/********** PRIVATE FUNCTIONS **********/

void Funeraria::clearTable()
{
    int row_number = 1;
    for (int i = 0; i < this->ui.tableWidget->rowCount(); i++) {
        this->ui.tableWidget->setRowHeight(i, 30);
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

void Funeraria::showClientOrders(const QList<QMap<QString, QString>> &tombs)
{
    this->setupClientOrdersTable(tombs.size());

    for (int i = 0; i < tombs.size(); i++) {
        this->addClientOrdersTableRow(tombs[i], i);
    }
}

void Funeraria::showClientOrder(const QMap<QString, QString>& tomb)
{
    this->setupClientOrdersTable(1);

    this->addClientOrdersTableRow(tomb, 0);
}

void Funeraria::updateClientsCombobox() {
    // List of clients' names
    QStringList clients = this->client->getActiveNames();
    // Add the clients to the combo box
    this->ui.cbClient->addItem(this->client_placeholder);
    this->ui.cbClient->addItems(clients);
}

void Funeraria::updateQuickAccessNames() {
    // List of the clients to be shown inside the quick access bar
    QStringList quick_access_clients = this->client->getQuickAccessNames();

    QFont font;
    font.setPointSize(12);

    // Clear the quick access buttons
    QLayoutItem* item;
    while ((item = this->ui.quickClientsAccess->takeAt(0)) != 0) {
        delete item->widget();
    }

    // Add the buttons to the bar
    for (const auto& item : std::as_const(quick_access_clients)) {
        QPushButton* btn = new QPushButton();
        btn->setFont(font);
        btn->setText(item);
        btn->setMinimumSize(QSize(0, 30));
        this->ui.quickClientsAccess->addWidget(btn);

        this->connect(btn, &QPushButton::clicked, this, &Funeraria::slotQuickClientOrders);
    }

    // Add an horizontal spacer as last item to compact the buttons to the left
    QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->ui.quickClientsAccess->addSpacerItem(spacer);
}

void Funeraria::closeWindow()
{
    this->up = false;
    this->close();
}

void Funeraria::sortColumnDirection(const QString& column)
{
    // If no column was selected before or the currently selected column is not the last selected
    if (this->last_sorting_column == "" || this->last_sorting_column != column) {
        this->last_sorting_column = column;
        this->sort_column_direction = "ASC";
    }
    // If the currently selected column is the same that was selected the last time
    else if (this->last_sorting_column == column) {
        if (this->sort_column_direction == "ASC") {
            this->sort_column_direction = "DESC";
        }
        else {
            this->sort_column_direction = "ASC";
        }
    }
}

void Funeraria::setupClientOrdersTable(int tombs_count)
{
    this->is_table_sortable = true;

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Materiale", "€", "Pagata", "Note", "Accessori",
        "Ordine", "Provino", "Conferma", "Incisione", "Consegna", "", "", "" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 60);    // Progressive
    this->ui.tableWidget->setColumnWidth(1, 200);   // Name
    this->ui.tableWidget->setColumnWidth(2, 145);   // Material
    this->ui.tableWidget->setColumnWidth(3, 45);    // Price
    this->ui.tableWidget->setColumnWidth(4, 60);    // Paid
    this->ui.tableWidget->setColumnWidth(5, 600);   // Notes
    this->ui.tableWidget->setColumnWidth(6, 80);    // Accessories mounted
    this->ui.tableWidget->setColumnWidth(7, 90);    // Ordered at
    this->ui.tableWidget->setColumnWidth(8, 90);    // Proofed at
    this->ui.tableWidget->setColumnWidth(9, 90);    // Confirmed at
    this->ui.tableWidget->setColumnWidth(10, 90);   // Engraved at
    this->ui.tableWidget->setColumnWidth(11, 90);   // Delivered at
    this->ui.tableWidget->setColumnWidth(12, 80);   // Details Button
    this->ui.tableWidget->setColumnWidth(13, 70);   // Open folder Button
    this->ui.tableWidget->setColumnWidth(14, 70);   // Set paid tomb button

}

void Funeraria::addClientOrdersTableRow(const QMap<QString, QString>& tomb, int row)
{
    QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
    pb_details->setText("Dettagli");

    QPushButton* pb_open_folder = new QPushButton(this->ui.tableWidget);
    pb_open_folder->setText("Apri");

    QPushButton* pb_set_paid = new QPushButton(this->ui.tableWidget);
    pb_set_paid->setText("Pagata");

    // Generate the cells' content and set them as not editable
    QTableWidgetItem* progressive = new QTableWidgetItem(tomb["progressive"]);
    progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* name = new QTableWidgetItem(tomb["name"]);
    name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* material = new QTableWidgetItem(this->material->getNameFromCode(tomb["material"]));
    material->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* price = new QTableWidgetItem(tomb["price"]);
    price->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* paid = new QTableWidgetItem("");
    paid->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* notes = new QTableWidgetItem(tomb["notes"]);
    notes->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* accessories_mounted = new QTableWidgetItem("");
    accessories_mounted->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QString order_date = Helpers::dateSqlToIta(tomb["ordered_at"]);
    QTableWidgetItem* ordered_at = new QTableWidgetItem(order_date);
    ordered_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QString proof_date = Helpers::dateSqlToIta(tomb["proofed_at"]);
    QTableWidgetItem* proofed_at = new QTableWidgetItem(proof_date);
    proofed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QString confirm_date = Helpers::dateSqlToIta(tomb["confirmed_at"]);
    QTableWidgetItem* confirmed_at = new QTableWidgetItem(confirm_date);
    confirmed_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QString engrave_date;
    if (tomb["engraved_at"] == this->not_engraved) {
        engrave_date = this->not_engraved;
    }
    else {
        engrave_date = Helpers::dateSqlToIta(tomb["engraved_at"]);
    }

    QTableWidgetItem* engraved_at = new QTableWidgetItem(engrave_date);
    engraved_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QString delivery_date;
    if (tomb["delivered_at"] == "Consegnata") {
        delivery_date = "Consegnata";
    }
    else {
        delivery_date = Helpers::dateSqlToIta(tomb["delivered_at"]);
    }

    QTableWidgetItem* delivered_at = new QTableWidgetItem(delivery_date);
    delivered_at->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if (row % 2 == 0) {
        this->row_bg = this->row_even;
    }
    else {
        this->row_bg = this->row_odd;
    }

    if (delivery_date != "") {
        if (this->row_bg == this->row_even) {
            this->row_bg = this->tomb_delivered_even;
        }
        else {
            this->row_bg = this->tomb_delivered_odd;
        }
    }
    else if (tomb["engraved"].toInt() == 1 && confirm_date != "" && engrave_date == "") {
        this->row_bg = this->tomb_to_engrave;
    }

    this->paid_cell = this->row_bg;
    this->mounted_cell = this->row_bg;

    if (tomb["paid"] == "0" && delivery_date != "" && tomb["price"] != "0") {
        this->paid_cell = this->warning_bg;
    }

    if (tomb["accessories_mounted"] == "0" && engrave_date != "") {
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

    this->ui.tableWidget->setItem(row, 0, progressive);
    this->ui.tableWidget->setItem(row, 1, name);
    this->ui.tableWidget->setItem(row, 2, material);
    this->ui.tableWidget->setItem(row, 3, price);
    this->ui.tableWidget->setItem(row, 4, paid);
    this->ui.tableWidget->setItem(row, 5, notes);
    this->ui.tableWidget->setItem(row, 6, accessories_mounted);
    this->ui.tableWidget->setItem(row, 7, ordered_at);
    this->ui.tableWidget->setItem(row, 8, proofed_at);
    this->ui.tableWidget->setItem(row, 9, confirmed_at);
    this->ui.tableWidget->setItem(row, 10, engraved_at);
    this->ui.tableWidget->setItem(row, 11, delivered_at);
    this->ui.tableWidget->setCellWidget(row, 12, pb_details); // Details button
    this->ui.tableWidget->setCellWidget(row, 13, pb_open_folder); // Open folder button
    this->ui.tableWidget->setCellWidget(row, 14, pb_set_paid); // Set paid tomb button

    this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotTombDetails);
    this->connect(pb_open_folder, &QPushButton::clicked, this, &Funeraria::slotTombFolder);
    this->connect(pb_set_paid, &QPushButton::clicked, this, &Funeraria::slotSetPaidTomb);
}

void Funeraria::setupAccessoriesToMountTable(int tombs_count)
{
    this->current_table = "accessories";

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Materiale", "Vaso", "Lampada", "Fiamma", "Cliente", "Azioni" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 60);    // Progressive
    this->ui.tableWidget->setColumnWidth(1, 250);   // Name
    this->ui.tableWidget->setColumnWidth(2, 300);   // Material
    this->ui.tableWidget->setColumnWidth(3, 200);   // Vase
    this->ui.tableWidget->setColumnWidth(4, 200);   // Lamp
    this->ui.tableWidget->setColumnWidth(5, 200);   // Flame
    this->ui.tableWidget->setColumnWidth(6, 300);   // Client
    this->ui.tableWidget->setColumnWidth(7, 80);    // Actions
}

void Funeraria::setupTombsNotPaidTable(int tombs_count)
{
    this->current_table = "tombs";

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Prezzo", "Cliente", "Azioni" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 60);
    this->ui.tableWidget->setColumnWidth(1, 250);
    this->ui.tableWidget->setColumnWidth(2, 90);
    this->ui.tableWidget->setColumnWidth(3, 250);
    this->ui.tableWidget->setColumnWidth(4, 70);

}

void Funeraria::setupTombsToEngraveTable(int tombs_count)
{
    this->current_table = "tombs";

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Materiale", "Cliente", "Conferma", "" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 90);
    this->ui.tableWidget->setColumnWidth(1, 250);
    this->ui.tableWidget->setColumnWidth(2, 250);
    this->ui.tableWidget->setColumnWidth(3, 200);
    this->ui.tableWidget->setColumnWidth(4, 100);
    this->ui.tableWidget->setColumnWidth(5, 100);
}
