#include "funeraria.h"

/********** CONSTRUCTOR **********/

Funeraria::Funeraria(QWidget* parent)
    : QMainWindow(parent)
{
    /*
     * Before to setup the application: check the config file and the database
     */

    /*
     * Check the config file before to instantiate the DatabaseManager object,
     * DatabaseManager needs to read the configuration file and we want to be sure
     * that the configuration is correctly set.
     */
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
        this->setWindowIcon(QIcon(this->icons_folder + "funeraria.png"));

        // Load the stylesheet for the UI
        QString style = Helpers::getStyle(this->css_folder);

        if (!style.isEmpty()) {
            this->setStyleSheet(style);
        }

        // Set the font for the table
        QFont font("Calibri", 12);
        this->ui.tableWidget->setFont(font);
        this->connect(
            this->ui.tableWidget->horizontalHeader(), 
            &QHeaderView::sectionClicked, 
            this, 
            &Funeraria::slotSortColumn
        );

        // Connect the customContextMenuRequested signal to the relative slot
        this->connect(
            this->ui.tableWidget, 
            &QTableWidget::customContextMenuRequested, 
            this, 
            &Funeraria::slotShowContextMenu
        );

        // Set icons for the menu items

        // File
        this->ui.actionBackupCSV->setIcon(QIcon(this->icons_folder + "backup-50.png"));
        this->ui.actionSettings->setIcon(QIcon(this->icons_folder + "settings-50.png"));
        // Clienti
        this->ui.actionCNew->setIcon(QIcon(this->icons_folder + "new-client-50.png"));
        this->ui.actionCList->setIcon(QIcon(this->icons_folder + "client-list-48.png"));
        // Materiali
        this->ui.actionMtNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionMtList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        // Sculture
        this->ui.actionScNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionScList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        // Decorazioni
        this->ui.menuPits->setIcon(QIcon(this->icons_folder + "pit-50.png"));
        this->ui.actionPitNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionPitList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        this->ui.menuFrames->setIcon(QIcon(this->icons_folder + "frame-50.png"));
        this->ui.actionFrNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionFrList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        this->ui.menuCrosses->setIcon(QIcon(this->icons_folder + "cross-72.png"));
        this->ui.actionCrNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionCrList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        this->ui.menuDrawings->setIcon(QIcon(this->icons_folder + "jesus-50.png"));
        this->ui.actionImNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionImList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        // Accessori
        this->ui.menuVases->setIcon(QIcon(this->icons_folder + "vase-50.png"));
        this->ui.actionVNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionVList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        this->ui.menuLamps->setIcon(QIcon(this->icons_folder + "lamp-50.png"));
        this->ui.actionLNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionLList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        this->ui.menuFlames->setIcon(QIcon(this->icons_folder + "flame-50.png"));
        this->ui.actionFNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionFList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        // Lapidi
        this->ui.actionTNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionTEngrave->setIcon(QIcon(this->icons_folder + "engrave-64.png"));
        this->ui.actionTPay->setIcon(QIcon(this->icons_folder + "moneybag-64.png"));
        this->ui.actionMAccessories->setIcon(QIcon(this->icons_folder + "mount-50.png"));
        this->ui.menuTypes->setIcon(QIcon(this->icons_folder + "tomb-64.png"));
        this->ui.actionTTNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionTTList->setIcon(QIcon(this->icons_folder + "list-50.png"));
        this->ui.menuFormats->setIcon(QIcon(this->icons_folder + "tomb-64.png"));
        this->ui.actionTFNew->setIcon(QIcon(this->icons_folder + "add-50.png"));
        this->ui.actionTFList->setIcon(QIcon(this->icons_folder + "list-50.png"));

        // Instantiate objects
        this->context_menu = new QMenu(this);
        this->client = new Client(this->db->db);
        this->client_ui = new ClientUi(this->db->db, this->css_folder, this->icons_folder, this);
        this->sculpture = new Sculpture(this->db->db);
        this->sculpture_ui = new SculptureUi(this->db->db, this->css_folder, this->icons_folder, this->images_folder, this);
        this->cross = new Cross(this->db->db);
        this->cross_ui = new CrossUi(this->db->db, this->css_folder, this->icons_folder, this->images_folder, this);
        this->drawing = new Drawing(this->db->db);
        this->drawing_ui = new DrawingUi(this->db->db, this->css_folder, this->icons_folder, this->images_folder, this);
        this->settings_ui = new SettingsUi(this->db->db, this->css_folder, this->icons_folder, this);
        this->report_ui = new ReportUi(this->db->db, this->css_folder, this->icons_folder, this);
        this->tomb_ui = new TombUi(this->db->db, this->css_folder, this->icons_folder, this);
        this->tombsAlike_ui = new TombsAlikeUi(this->db->db, this->css_folder, this->icons_folder, this);
        this->vase = new Accessory(this->db->db, "vase");
        this->lamp = new Accessory(this->db->db, "lamp");
        this->flame = new Accessory(this->db->db, "flame");
        this->pit = new Accessory(this->db->db, "pit");
        this->frame = new Accessory(this->db->db, "frame");
        this->material = new Accessory(this->db->db, "material");
        this->tomb_type = new Accessory(this->db->db, "tomb_type");
        this->tomb_format = new Accessory(this->db->db, "tomb_format");

        // Top bars' init, must be after the objects instantiation, some of them are required to build the bars
        this->initTopBarQuickAccess();
        this->initTombsTopBar();
        this->initSculpturesTopBar();
        this->initCrossesTopBar();
        this->initDrawingsTopBar();
        this->initClientsTopBar();

        // Signal emitted from the menu "Files"
        this->connect(this->ui.actionBackupCSV, SIGNAL(triggered()), this, SLOT(slotBackupDbToCSV()));
        this->connect(this->ui.actionSettings, SIGNAL(triggered()), this, SLOT(slotShowSettings()));

        // Signal emitted from the menu "Clienti"
        this->connect(this->ui.actionCNew, SIGNAL(triggered()), this, SLOT(slotNewClient()));
        this->connect(this->ui.actionCList, SIGNAL(triggered()), this, SLOT(slotShowClients()));

        // Signal emitted from the menu "Lapidi"
        this->connect(this->ui.actionTNew,                SIGNAL(triggered()), this, SLOT(slotNewTomb()));
        this->connect(this->ui.actionTEngrave,            SIGNAL(triggered()), this, SLOT(slotTombsToEngrave()));
        this->connect(this->ui.actionMAccessories,        SIGNAL(triggered()), this, SLOT(slotAccessoriesToMount()));
        this->connect(this->ui.actionTPay,                SIGNAL(triggered()), this, SLOT(slotTombsNotPaid()));
        this->connect(this->ui.actionSearchByProgressive, SIGNAL(triggered()), this, SLOT(slotTombByProgressive()));

        // Signal emitted from the menu "Sculture"
        this->connect(this->ui.actionScNew, SIGNAL(triggered()), this, SLOT(slotNewSculpture()));
        this->connect(this->ui.actionScList, SIGNAL(triggered()), this, SLOT(slotShowSculptures()));

        // Signal emitted from the menu "Croci"
        this->connect(this->ui.actionCrNew, SIGNAL(triggered()), this, SLOT(slotNewCross()));
        this->connect(this->ui.actionCrList, SIGNAL(triggered()), this, SLOT(slotShowCrosses()));

        // Signal emitted from the menu "Disegni"
        this->connect(this->ui.actionImNew, SIGNAL(triggered()), this, SLOT(slotNewDrawing()));
        this->connect(this->ui.actionImList, SIGNAL(triggered()), this, SLOT(slotShowDrawing()));

        /* 
         * Map the signal coming from the menues "Accessori, Materiali, Lapidi" to call the same function (slotNewItem) 
         * with the proper parameter
         */
        this->new_item_mapper = new QSignalMapper(this);
        this->connect(this->ui.actionMNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionVNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionLNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionFNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionFrNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionPitNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionTTNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        this->connect(this->ui.actionTFNew, SIGNAL(triggered()), new_item_mapper, SLOT(map()));
        new_item_mapper->setMapping(this->ui.actionMNew, "material");
        new_item_mapper->setMapping(this->ui.actionVNew, "vase");
        new_item_mapper->setMapping(this->ui.actionLNew, "lamp");
        new_item_mapper->setMapping(this->ui.actionFNew, "flame");
        new_item_mapper->setMapping(this->ui.actionFrNew, "frame");
        new_item_mapper->setMapping(this->ui.actionPitNew, "pit");
        new_item_mapper->setMapping(this->ui.actionTTNew, "tomb_type");
        new_item_mapper->setMapping(this->ui.actionTFNew, "tomb_format");
        this->connect(new_item_mapper, &QSignalMapper::mappedString, this, &Funeraria::slotNewItem);

        /* 
         * Map the signal coming from the menu "Accessori, Materiali, Lapidi" to call the same function (slotShowItems) 
         * with the proper parameter
         */
        this->show_items_mapper = new QSignalMapper(this);
        this->connect(this->ui.actionMList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionVList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionLList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionFList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionFrList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionPitList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionTTList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        this->connect(this->ui.actionTFList, SIGNAL(triggered()), show_items_mapper, SLOT(map()));
        show_items_mapper->setMapping(this->ui.actionMList, "material");
        show_items_mapper->setMapping(this->ui.actionVList, "vase");
        show_items_mapper->setMapping(this->ui.actionLList, "lamp");
        show_items_mapper->setMapping(this->ui.actionFList, "flame");
        show_items_mapper->setMapping(this->ui.actionFrList, "frame");
        show_items_mapper->setMapping(this->ui.actionPitList, "pit");
        show_items_mapper->setMapping(this->ui.actionTTList, "tomb_type");
        show_items_mapper->setMapping(this->ui.actionTFList, "tomb_format");
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

    delete this->context_menu;
    delete this->client;
    delete this->client_ui;
    delete this->sculpture;
    delete this->sculpture_ui;
    delete this->frame;
    delete this->cross;
    delete this->cross_ui;
    delete this->settings_ui;
    delete this->report_ui;
    delete this->tomb_ui;
    delete this->tombsAlike_ui;
    delete this->vase;
    delete this->lamp;
    delete this->flame;
    delete this->pit;
    delete this->material;
    delete this->tomb_type;
    delete this->tomb_format;
    delete this->show_items_mapper;
    delete this->new_item_mapper;

    delete this->btnQuickReports;
    delete this->btnQuickClients;
    delete this->btnQuickSculptures;
    delete this->btnQuickToEngrave;
    delete this->btnQuickToMount;
    delete this->btnQuickToPay;
    delete this->btnQuickNewTomb;
    delete this->topQuickAccessSpacer;

    delete this->lblClient;
    delete this->cbClient;
    delete this->lblYear;
    delete this->cbYear;
    delete this->lblDeceased;
    delete this->leDeceased;
    delete this->chbEngraved;
    delete this->btnSearch;
    delete this->tombSpacer;
    delete this->lblSearchByProgressive;
    delete this->leSearchByProgressive;
    delete this->btnSearchByProgressive;

    delete this->lblScName;
    delete this->leScName;
    delete this->sculptureSpacer;

    delete this->lblCrName;
    delete this->leCrName;
    delete this->crossSpacer;

    delete this->lblImName;
    delete this->leImName;
    delete this->drawingSpacer;

    delete this->lblClName;
    delete this->leClName;
    delete this->clientSpacer;

    delete this->config;
    delete this->db;
}

/********** SLOTS **********/

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
        delete tomb;
    }
    else {
        delete tomb;

        // Reload the table
        this->slotAccessoriesToMount();
    }
}

void Funeraria::slotAccessoriesToMount()
{
    this->showTopBar("tomb");

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

void Funeraria::slotClientDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the name property of the Client object to the name present in the clicked row
    this->client_ui->setId(this->ui.tableWidget->item(row, 0)->text().toInt());
    this->client_ui->setName(this->ui.tableWidget->item(row, 1)->text());
    this->client_ui->setModal(true);
    this->client_ui->exec();

    // Clear the combobox content
    this->cbClient->clear();
    // Update the combobox with the active clients
    this->updateClientsCombobox();
    // Update the quick access bar with the active clients
    this->updateQuickAccessNames();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowClients(row);
}

void Funeraria::slotClientOrders(bool reload, int row)
{
    qDebug() << "CLICK";
    if (reload) {

        Tomb* tomb = new Tomb(this->db->db);

        this->showTopBar("tomb");

        int client_id = this->client->getId(this->cbClient->currentText());
        int year;
        QString name = this->leDeceased->text().trimmed();

        if (this->cbYear->currentText() == "Tutti") {
            year = 0;
        }
        else {
            year = this->cbYear->currentText().toInt();
        }

        QList<QMap<QString, QString>> tombs = tomb->getList(client_id, year, this->chbEngraved->isChecked(), name);

        this->showClientOrders(tombs, row);

        delete tomb;
    }
}

void Funeraria::slotCrossDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    this->cross_ui->setCode(this->ui.tableWidget->item(row, 0)->text());
    this->cross_ui->setModal(true);
    this->cross_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowCrosses(row);
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
        bool errors = false;

        // Delete the item
        if (this->current_table == "vase") {
            if (!this->vase->remove(this->ui.tableWidget->item(row, 0)->text())) errors = true;
        }
        else if (this->current_table == "lamp") {
            if (!this->lamp->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "flame") {
            if (!this->flame->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "pit") {
            if (!this->pit->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "material") {
            if (!this->material->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "client") {
            if (!this->client->remove(this->ui.tableWidget->item(row, 0)->text().toInt()))  errors = true;
        }
        else if (this->current_table == "sculpture") {
            if (!this->sculpture->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "cross") {
            if (!this->cross->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "frame") {
            if (!this->frame->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "drawing") {
            if (!this->drawing->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "tomb_type") {
            if (!this->tomb_type->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }
        else if (this->current_table == "tomb_format") {
            if (!this->tomb_format->remove(this->ui.tableWidget->item(row, 0)->text()))  errors = true;
        }

        if (!errors) {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Information);
            message.setText("Eliminazione eseguita.");
            message.exec();

            // Reload the table

            if (this->current_table == "vase" ||
                this->current_table == "lamp" ||
                this->current_table == "flame" ||
                this->current_table == "pit" ||
                this->current_table == "frame" ||
                this->current_table == "material" ||
                this->current_table == "tomb_type" ||
                this->current_table == "tomb_format")
            {
                this->slotShowItems(this->current_table);
            }
            else if (this->current_table == "client") {
                this->slotShowClients();
            }
            else if (this->current_table == "sculpture") {
                this->slotShowSculptures();
            }
            else if (this->current_table == "cross") {
                this->slotShowCrosses();
            }
            else if (this->current_table == "drawing") {
                this->slotShowDrawing();
            }
        }
        else {
            QMessageBox message;
            message.setWindowTitle("Funeraria");
            message.setIcon(QMessageBox::Critical);
            message.setText("Eliminazione non riuscita.");
            message.exec();
        }
    }
}

void Funeraria::slotDrawingDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    this->drawing_ui->setCode(this->ui.tableWidget->item(row, 0)->text());
    this->drawing_ui->setModal(true);
    this->drawing_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowDrawing(row);
}

void Funeraria::slotEditItem() {
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the name property of the Client object to the name present in the clicked row
    if (this->current_table == "vase") {
        AccessoryUi* vase_ui = new AccessoryUi(this->db->db, "vase", this->css_folder, this->icons_folder, this);
        vase_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        vase_ui->setModal(true);
        vase_ui->exec();

        delete vase_ui;
    }
    else if (this->current_table == "lamp") {
        AccessoryUi* lamp_ui = new AccessoryUi(this->db->db, "lamp", this->css_folder, this->icons_folder, this);
        lamp_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        lamp_ui->setModal(true);
        lamp_ui->exec();

        delete lamp_ui;
    }
    else if (this->current_table == "flame") {
        AccessoryUi* flame_ui = new AccessoryUi(this->db->db, "flame", this->css_folder, this->icons_folder, this);
        flame_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        flame_ui->setModal(true);
        flame_ui->exec();

        delete flame_ui;
    }
    else if (this->current_table == "frame") {
        AccessoryUi* frame_ui = new AccessoryUi(this->db->db, "frame", this->css_folder, this->icons_folder, this);
        frame_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        frame_ui->setModal(true);
        frame_ui->exec();

        delete frame_ui;
    }
    else if (this->current_table == "pit") {
        AccessoryUi* pit_ui = new AccessoryUi(this->db->db, "pit", this->css_folder, this->icons_folder, this);
        pit_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        pit_ui->setModal(true);
        pit_ui->exec();

        delete pit_ui;
    }
    else if (this->current_table == "material") {
        AccessoryUi* material_ui = new AccessoryUi(this->db->db, "material", this->css_folder, this->icons_folder, this);
        material_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        material_ui->setModal(true);
        material_ui->exec();

        delete material_ui;
    }
    else if (this->current_table == "tomb_type") {
        AccessoryUi* tomb_type_ui = new AccessoryUi(this->db->db, "tomb_type", this->css_folder, this->icons_folder, this);
        tomb_type_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        tomb_type_ui->setModal(true);
        tomb_type_ui->exec();

        delete tomb_type_ui;
    }
    else if (this->current_table == "tomb_format") {
        AccessoryUi* tomb_format_ui = new AccessoryUi(this->db->db, "tomb_format", this->css_folder, this->icons_folder, this);
        tomb_format_ui->updateForm(
            this->ui.tableWidget->item(row, 0)->text(),
            this->ui.tableWidget->item(row, 1)->text()
        );
        tomb_format_ui->setModal(true);
        tomb_format_ui->exec();

        delete tomb_format_ui;
    }

    this->showItems(this->current_table, row);
}

void Funeraria::slotFilterClientOrders()
{
    // If the tombs table is not shown
    if (this->current_table != "tomb") {
        return;
    }

    this->slotClientOrders();
}

void Funeraria::slotFilterClients()
{
    // If the tombs table is not shown
    if (this->current_table != "client") {
        return;
    }

    this->slotShowClients();
}

void Funeraria::slotFilterCrosses()
{
    // If the cross table is not shown
    if (this->current_table != "cross") {
        return;
    }

    this->slotShowCrosses();
}

void Funeraria::slotFilterDrawing()
{
    // If the image table is not shown
    if (this->current_table != "drawing") {
        return;
    }

    this->slotShowDrawing();
}

void Funeraria::slotFilterSculptures()
{
    // If the tombs table is not shown
    if (this->current_table != "sculpture") {
        return;
    }

    this->slotShowSculptures();
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

void Funeraria::slotNewCross()
{
    this->current_table = "cross";

    // Set the id property of the Cross object to 0
    this->cross_ui->setCode("0");
    this->cross_ui->setModal(true);
    this->cross_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowCrosses();
}

void Funeraria::slotNewDrawing()
{
    this->current_table = "drawing";

    // Set the id property of the Drawing object to 0
    this->drawing_ui->setCode("0");
    this->drawing_ui->setModal(true);
    this->drawing_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowDrawing();
}

void Funeraria::slotNewItem(const QString& type)
{
    this->current_table = type;

    if (type == "vase") {
        AccessoryUi* vase_ui = new AccessoryUi(this->db->db, "vase", this->css_folder, this->icons_folder, this);
        vase_ui->updateForm();
        vase_ui->setModal(true);
        vase_ui->exec();

        delete vase_ui;
    }
    else if (type == "lamp") {
        AccessoryUi* lamp_ui = new AccessoryUi(this->db->db, "lamp", this->css_folder, this->icons_folder, this);
        lamp_ui->updateForm();
        lamp_ui->setModal(true);
        lamp_ui->exec();

        delete lamp_ui;
    }
    else if (type == "flame") {
        AccessoryUi* flame_ui = new AccessoryUi(this->db->db, "flame", this->css_folder, this->icons_folder, this);
        flame_ui->updateForm();
        flame_ui->setModal(true);
        flame_ui->exec();

        delete flame_ui;
    }
    else if (type == "material") {
        AccessoryUi* material_ui = new AccessoryUi(this->db->db, "material", this->css_folder, this->icons_folder, this);
        material_ui->updateForm();
        material_ui->setModal(true);
        material_ui->exec();

        delete material_ui;
    }
    else if (type == "frame") {
        AccessoryUi* frame_ui = new AccessoryUi(this->db->db, "frame", this->css_folder, this->icons_folder, this);
        frame_ui->updateForm();
        frame_ui->setModal(true);
        frame_ui->exec();

        delete frame_ui;
    }
    else if (type == "pit") {
        AccessoryUi* pit_ui = new AccessoryUi(this->db->db, "pit", this->css_folder, this->icons_folder, this);
        pit_ui->updateForm();
        pit_ui->setModal(true);
        pit_ui->exec();

        delete pit_ui;
    }
    else if (type == "tomb_type") {
        AccessoryUi* tomb_type_ui = new AccessoryUi(this->db->db, "tomb_type", this->css_folder, this->icons_folder, this);
        tomb_type_ui->updateForm();
        tomb_type_ui->setModal(true);
        tomb_type_ui->exec();

        delete tomb_type_ui;
    }
    else if (type == "tomb_format") {
        AccessoryUi* tomb_format_ui = new AccessoryUi(this->db->db, "tomb_format", this->css_folder, this->icons_folder, this);
        tomb_format_ui->updateForm();
        tomb_format_ui->setModal(true);
        tomb_format_ui->exec();

        delete tomb_format_ui;
    }
    else {
        // The type requested is not valid
        return;
    }

    this->slotShowItems(this->current_table);
}

void Funeraria::slotNewSculpture()
{
    this->current_table = "sculpture";

    // Set the id property of the Sculpture object to 0
    this->sculpture_ui->setCode("0");
    this->sculpture_ui->setModal(true);
    this->sculpture_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowSculptures();
}

void Funeraria::slotNewTomb()
{
    this->current_table = "tomb";

    // Set the name property of the Client object to an empty string
    this->tomb_ui->setProgressive(0);
    this->tomb_ui->setModal(true);
    this->tomb_ui->exec();

    // If the table shows any tomb
    if (this->ui.tableWidget->rowCount() > 0) {
        // Reload the table when the popup is closed, the user could have made some changes
        this->slotClientOrders();
    }
}

void Funeraria::slotPrintToPayListPdf()
{
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

    // Filter to show only pdf files
    QString filter = "pdf(*.pdf)";
    QString filename = QFileDialog::getSaveFileName(this, tr("save_to"), "", filter);

    // If no file name is provided, stop the execution, the user just closed the dialog window
    if (filename.trimmed() == "") return;

    // Load the font to be used inside the table
    QString full_path_font(qApp->applicationDirPath() + "/" + this->fonts_folder + "cour.ttf");
    int font_ID = QFontDatabase::addApplicationFont(full_path_font);
    QString font_family = QFontDatabase::applicationFontFamilies(font_ID).at(0);

    QString html = "";
    html += "<table style='font-family: " + font_family + ";font-size:220px;' cellpadding='5' width='80%'>";

    html += "<tr><th colspan='2' style='text-align:left'>Elenco lavori</th></tr>";

    double total = 0;

    // For all the selections
    for (int i = 0; i < ranges.size(); i++) {
        // Loop from the first to the last row
        for (int j = ranges[i].topRow(); j <= ranges[i].bottomRow(); j++) {
            // If a name is defined, the material and the description are not necessary
            if (this->ui.tableWidget->item(j, 1)->data(Qt::DisplayRole).toString() != this->name_not_defined) {
                // Print the data of each row
                html += "<tr><td width='80%'>" + this->ui.tableWidget->item(j, 1)->data(Qt::DisplayRole).toString() + "</td>"; // Name
            }
            else {
                // Print the data of each row
                html += "<tr><td width='80%'>" + this->ui.tableWidget->item(j, 2)->data(Qt::DisplayRole).toString() + " - "; // Material
                html += this->ui.tableWidget->item(j, 6)->data(Qt::DisplayRole).toString() + "</td>"; // Notes
            }

            QString price = this->ui.tableWidget->item(j, 4)->data(Qt::DisplayRole).toString();
            // Number of spaces to add after the euro symbol, depending on the price's digits
            // price's max predicted digits are at most 4, thus at least 1 space will be added
            int num_spaces = 5 - price.size();
            QString spaces = "";

            for (int i = 0; i < num_spaces; i++) {
                spaces += "&nbsp;";
            }

            html += "<td align='right'> €" + spaces + price + "</td></tr>"; // Price

            total += this->ui.tableWidget->item(j, 4)->data(Qt::DisplayRole).toDouble();
        }
    }

    html += "<tr><td></td><td><hr></td></tr>";

    // Number of spaces to add after the euro symbol, depending on the total's digits
    // total's max predicted digits are at most 4, thus at least 1 space will be added
    int num_spaces = 5 - QString::number(total).size();
    QString spaces = "";

    for (int i = 0; i < num_spaces; i++) {
        spaces += "&nbsp;";
    }

    html += "<tr><td>Totale</td><td style='text-align:right'>€" +
        spaces + QString::number(total) +
        "</td></tr></table>"; // Total price

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(filename);
    printer.setPageMargins(QMarginsF(5, 5, 5, 5));

    document.print(&printer);
}

void Funeraria::slotPrintToPayListTxt() {
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

    if (!list_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è possibile aprire il file selezionato, verificare che il percorso non sia protetto da scrittura.");
        message.exec();
        return;
    }

    double total = 0;

    QTextStream out(&list_file);

    // For all the selections
    for (int i = 0; i < ranges.size(); i++) {
        // Loop from the first to the last row
        for (int j = ranges[i].topRow(); j <= ranges[i].bottomRow(); j++) {
            // If a name is defined, the material and the description are not necessary
            if (this->ui.tableWidget->item(j, 1)->data(Qt::DisplayRole).toString() != this->name_not_defined) {
                // Print the data
                out << this->ui.tableWidget->item(j, 1)->data(Qt::DisplayRole).toString() + " "; // Name
            }
            else {
                // Print the data
                out << this->ui.tableWidget->item(j, 2)->data(Qt::DisplayRole).toString() + " "; // Material
                out << this->ui.tableWidget->item(j, 6)->data(Qt::DisplayRole).toString() + " "; // Notes
            }

            out << "€ " + this->ui.tableWidget->item(j, 4)->data(Qt::DisplayRole).toString() + "\n\n"; // Price

            total += this->ui.tableWidget->item(j, 4)->data(Qt::DisplayRole).toDouble();
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

void Funeraria::slotQuickClientOrders()
{
    // The button's text, it's the client's name
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

    this->showTopBar("tomb");

    int client_id = this->client->getId(button_text);
    int year = QDate::currentDate().toString("yyyy").toInt();

    this->cbClient->setCurrentText(button_text);
    this->cbYear->setCurrentText(QDate::currentDate().toString("yyyy"));
    this->leDeceased->setText("");

    // Search params: client's id, current year, no filter for the deceased's name
    QList<QMap<QString, QString>> tombs = tomb->getList(client_id, year, this->chbEngraved->isChecked());

    delete tomb;

    this->showClientOrders(tombs);
}

void Funeraria::slotReport()
{
    this->current_table = "tomb";

    this->report_ui->setModal(true);
    this->report_ui->exec();
}

void Funeraria::slotSculptureDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    this->sculpture_ui->setCode(this->ui.tableWidget->item(row, 0)->text());
    this->sculpture_ui->setModal(true);
    this->sculpture_ui->exec();

    // Reload the table when the popup is closed, the user could have made some changes
    this->slotShowSculptures(row);
}

void Funeraria::slotSearchByProgressive()
{
    Tomb* tomb = new Tomb(this->db->db);
    QList<QMap<QString, QString>> tombs;

    this->showTopBar("tomb");

    int progressive = this->leSearchByProgressive->text().trimmed().toInt();

    QMap<QString, QString> tomb_found = tomb->getByProgressive(progressive);

    delete tomb;

    if (tomb_found.isEmpty()) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Information);
        message.setText("Non è stata trovata alcuna lapide col numero progressivo fornito.");
        message.exec();

        return;
    }

    tombs.append(tomb_found);

    this->showClientOrders(tombs);
}

void Funeraria::slotSetConfirmedTomb()
{
    Tomb* tomb = new Tomb(this->db->db);

    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    int progressive = this->ui.tableWidget->item(row, 0)->text().toInt();

    if (!tomb->setConfirmed(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile impostare la lapide come confermata.");
        message.exec();
        delete tomb;

    }
    else {
        delete tomb;

        // Reload the table
        this->slotClientOrders(true, row);
    }
}

void Funeraria::slotSetDeliveredTomb()
{
    Tomb* tomb = new Tomb(this->db->db);

    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    int progressive = this->ui.tableWidget->item(row, 0)->text().toInt();

    if (!tomb->setDelivered(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile impostare la lapide come consegnata.");
        message.exec();
        delete tomb;

    }
    else {
        delete tomb;

        // Reload the table
        this->slotClientOrders(true, row);
    }
}

void Funeraria::slotSetEngravedTomb()
{
    Tomb* tomb = new Tomb(this->db->db);

    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    int progressive = this->ui.tableWidget->item(row, 0)->text().toInt();

    if (!tomb->setEngraved(progressive)) {
        QMessageBox message;
        message.setWindowTitle("Funeraria");
        message.setIcon(QMessageBox::Warning);
        message.setText("Non è stato possibile impostare la lapide come incisa.");
        message.exec();
        delete tomb;

    }
    else {
        delete tomb;

        // Reload the table
        this->slotClientOrders(row);
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
        delete tomb;

    }
    else {
        delete tomb;

        // Reload the table
        this->slotClientOrders(true, row);
    }
}

void Funeraria::slotShowClients(int row)
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->showTopBar("client");

    this->is_table_sortable = false;

    // Reset the table's content
    this->clearTable();

    QString name = this->leClName->text().trimmed();

    QList<QMap<QString, QString>> clients = this->client->get(name);

    QStringList headers{ "ID", "Posizione", "Nome", "Email", "Telefono", "", "" };

    this->ui.tableWidget->setRowCount(clients.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_id);
    this->ui.tableWidget->setColumnWidth(1, this->col_width_position);
    this->ui.tableWidget->setColumnWidth(2, this->col_width_client_name);
    this->ui.tableWidget->setColumnWidth(3, this->col_width_email);
    this->ui.tableWidget->setColumnWidth(4, this->col_width_phone);
    this->ui.tableWidget->setColumnWidth(5, this->col_width_action);
    this->ui.tableWidget->setColumnWidth(6, this->col_width_action);

    int row_number = 1;
    for (int i = 0; i < clients.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");
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

        // If the client has no emails nor phone numbers
        if (lines == 0) {
            // set the lines value to 1
            lines = 1;
        }

        // Set the row height
        this->ui.tableWidget->setRowHeight(i, this->row_height * lines);

        QTableWidgetItem* id_widget = new QTableWidgetItem(clients[i]["id"]);
        // Set the field as not editable
        id_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

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

        id_widget->setBackground(QBrush(row_bg));
        position_widget->setBackground(QBrush(row_bg));
        name_widget->setBackground(QBrush(row_bg));
        emails_widget->setBackground(QBrush(row_bg));
        phones_widget->setBackground(QBrush(row_bg));

        this->ui.tableWidget->setItem(i, 0, id_widget);
        this->ui.tableWidget->setItem(i, 1, position_widget);
        this->ui.tableWidget->setItem(i, 2, name_widget);
        this->ui.tableWidget->setItem(i, 3, emails_widget);
        this->ui.tableWidget->setItem(i, 4, phones_widget);
        this->ui.tableWidget->setCellWidget(i, 5, pb_details);
        this->ui.tableWidget->setCellWidget(i, 6, pb_delete);

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotClientDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDeleteItem);

        row_number++;
    }

    this->leClName->setFocus();
}

void Funeraria::slotShowContextMenu(const QPoint& pos) {
    // Clear from the menu the eventual entries shown before
    this->context_menu->clear();

    // Create the actions according to what is the selection or the point where clicked
    QAction* sum_prices = this->context_menu->addAction("Somma i prezzi selezionati");

    QMenu* list = this->context_menu->addMenu("Genera lista");
    QAction* print_listTxt = list->addAction("Formato TXT");
    QAction* print_listPdf = list->addAction("Formato PDF");

    sum_prices->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomFitBest));
    list->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::Printer));
    print_listTxt->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
    print_listPdf->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrintPreview));

    // Connect actions to slots
    connect(sum_prices, &QAction::triggered, this, &Funeraria::slotSumSelectedPrices);
    connect(print_listTxt, &QAction::triggered, this, &Funeraria::slotPrintToPayListTxt);
    connect(print_listPdf, &QAction::triggered, this, &Funeraria::slotPrintToPayListPdf);

    // Position where to show the context menu
    this->context_menu->popup(this->ui.tableWidget->viewport()->mapToGlobal(pos));
}

void Funeraria::slotShowCrosses(int row)
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->showTopBar("cross");

    this->is_table_sortable = false;

    // Reset the table's content
    this->clearTable();

    QString name = this->leCrName->text().trimmed();

    QList<QMap<QString, QString>> crosses = this->cross->getListByName(name);

    QStringList headers{ "Codice", "Img", "Nome", "Larghezza", "Altezza", "", "" };

    int img_cell_width = 100;
    int img_cell_height = 150;

    this->ui.tableWidget->setRowCount(crosses.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_code); // Code
    this->ui.tableWidget->setColumnWidth(1, img_cell_width);  // Img
    this->ui.tableWidget->setColumnWidth(2, this->col_width_acc_name); // Name
    this->ui.tableWidget->setColumnWidth(3, this->col_width_size);  // Width
    this->ui.tableWidget->setColumnWidth(4, this->col_width_size);  // Height
    this->ui.tableWidget->setColumnWidth(5, this->col_width_action);  // Details button
    this->ui.tableWidget->setColumnWidth(6, this->col_width_action);  // Delete button

    int row_number = 1;

    for (int i = 0; i < crosses.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");
        QPushButton* pb_delete = new QPushButton(this->ui.tableWidget);
        pb_delete->setText("Elimina");

        // Specific row height to contain the image
        this->ui.tableWidget->setRowHeight(i, img_cell_height);

        QTableWidgetItem* code_widget = new QTableWidgetItem(crosses[i]["code"]);
        code_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString pic_path = this->config->getCrossesPath() + "/" + crosses[i]["img"];

        // Set the not found image if the provided one is missing
        QFile img_file(pic_path);
        if (!img_file.exists() || crosses[i]["img"] == "") {
            pic_path = this->images_folder + "notfound.jpg";
        }

        QPixmap pic(pic_path);
        QPixmap resized, resized_h, resized_w;
        resized_h = pic.scaledToHeight(img_cell_height);
        resized_w = pic.scaledToWidth(img_cell_width);

        if (resized_h.width() <= img_cell_width && resized_h.height() <= img_cell_height) {
            resized = resized_h;
        }
        else {
            resized = resized_w;
        }

        QLabel* image_widget = new QLabel(this->ui.tableWidget);
        image_widget->setText("");
        image_widget->setScaledContents(false);
        image_widget->setPixmap(resized);

        QTableWidgetItem* name_widget = new QTableWidgetItem(crosses[i]["name"]);
        // Set the field as not editable
        name_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QTableWidgetItem* width_widget = new QTableWidgetItem(crosses[i]["width"]);
        width_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        width_widget->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* height_widget = new QTableWidgetItem(crosses[i]["height"]);
        height_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        height_widget->setTextAlignment(Qt::AlignCenter);

        if (row_number % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        code_widget->setBackground(QBrush(row_bg));
        name_widget->setBackground(QBrush(row_bg));
        width_widget->setBackground(QBrush(row_bg));
        height_widget->setBackground(QBrush(row_bg));

        this->ui.tableWidget->setItem(i, 0, code_widget);
        this->ui.tableWidget->setCellWidget(i, 1, image_widget);
        this->ui.tableWidget->setItem(i, 2, name_widget);
        this->ui.tableWidget->setItem(i, 3, width_widget);
        this->ui.tableWidget->setItem(i, 4, height_widget);
        this->ui.tableWidget->setCellWidget(i, 5, pb_details);
        this->ui.tableWidget->setCellWidget(i, 6, pb_delete);

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotCrossDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDeleteItem);

        row_number++;
    }

    // Set the table scroll to have the current row positioned at the center
    this->ui.tableWidget->scrollToItem(
        this->ui.tableWidget->item(row, 0),
        QAbstractItemView::PositionAtCenter
    );

    this->leCrName->setFocus();
}

void Funeraria::slotShowDrawing(int row)
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->showTopBar("drawing");

    this->is_table_sortable = false;

    // Reset the table's content
    this->clearTable();

    QString name = this->leImName->text().trimmed();

    QList<QMap<QString, QString>> drawings = this->drawing->getListByName(name);

    QStringList headers{ "Codice", "Img", "Nome", "Larghezza", "Altezza", "", "" };

    int img_cell_width = 150;
    int img_cell_height = 100;

    this->ui.tableWidget->setRowCount(drawings.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_code); // Code
    this->ui.tableWidget->setColumnWidth(1, img_cell_width);  // Img
    this->ui.tableWidget->setColumnWidth(2, this->col_width_drawing_name); // Name
    this->ui.tableWidget->setColumnWidth(3, this->col_width_size);  // Width
    this->ui.tableWidget->setColumnWidth(4, this->col_width_size);  // Height
    this->ui.tableWidget->setColumnWidth(5, this->col_width_action);  // Details button
    this->ui.tableWidget->setColumnWidth(6, this->col_width_action);  // Delete button

    int row_number = 1;

    for (int i = 0; i < drawings.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");
        QPushButton* pb_delete = new QPushButton(this->ui.tableWidget);
        pb_delete->setText("Elimina");

        // Specific row height to contain the image
        this->ui.tableWidget->setRowHeight(i, img_cell_height);

        QTableWidgetItem* code_widget = new QTableWidgetItem(drawings[i]["code"]);
        code_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString pic_path = this->config->getDrawingPath() + "/" + drawings[i]["img"];

        // Set the not found image if the provided one is missing
        QFile img_file(pic_path);
        if (!img_file.exists() || drawings[i]["img"] == "") {
            pic_path = this->images_folder + "notfound.jpg";
        }

        QPixmap pic(pic_path);
        QPixmap resized, resized_h, resized_w;
        resized_h = pic.scaledToHeight(img_cell_height);
        resized_w = pic.scaledToWidth(img_cell_width);

        if (resized_h.width() <= img_cell_width && resized_h.height() <= img_cell_height) {
            resized = resized_h;
        }
        else {
            resized = resized_w;
        }

        QLabel* image_widget = new QLabel(this->ui.tableWidget);
        image_widget->setText("");
        image_widget->setScaledContents(false);
        image_widget->setPixmap(resized);

        QTableWidgetItem* name_widget = new QTableWidgetItem(drawings[i]["name"]);
        // Set the field as not editable
        name_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QTableWidgetItem* width_widget = new QTableWidgetItem(drawings[i]["width"]);
        width_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        width_widget->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* height_widget = new QTableWidgetItem(drawings[i]["height"]);
        height_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        height_widget->setTextAlignment(Qt::AlignCenter);

        if (row_number % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        code_widget->setBackground(QBrush(row_bg));
        name_widget->setBackground(QBrush(row_bg));
        width_widget->setBackground(QBrush(row_bg));
        height_widget->setBackground(QBrush(row_bg));

        this->ui.tableWidget->setItem(i, 0, code_widget);
        this->ui.tableWidget->setCellWidget(i, 1, image_widget);
        this->ui.tableWidget->setItem(i, 2, name_widget);
        this->ui.tableWidget->setItem(i, 3, width_widget);
        this->ui.tableWidget->setItem(i, 4, height_widget);
        this->ui.tableWidget->setCellWidget(i, 5, pb_details);
        this->ui.tableWidget->setCellWidget(i, 6, pb_delete);

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotDrawingDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDeleteItem);

        row_number++;
    }

    // Set the table scroll to have the current row positioned at the center
    this->ui.tableWidget->scrollToItem(
        this->ui.tableWidget->item(row, 0),
        QAbstractItemView::PositionAtCenter
    );

    this->leImName->setFocus();
}

void Funeraria::slotShowItems(const QString& type)
{
    this->showItems(type, 1);
}

void Funeraria::slotShowSculptures(int row)
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->showTopBar("sculpture");

    this->is_table_sortable = false;

    // Reset the table's content
    this->clearTable();

    QString name = this->leScName->text().trimmed();

    QList<QMap<QString, QString>> sculptures = this->sculpture->getListByName(name);

    QStringList headers{ "Codice", "Img", "Nome", "Larghezza", "Altezza", "Profondità", "Rid Z", "Rid XY", "", "" };

    int img_cell_width = 150;
    int img_cell_height = 100;

    this->ui.tableWidget->setRowCount(sculptures.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_code); // Code
    this->ui.tableWidget->setColumnWidth(1, img_cell_width);  // Img
    this->ui.tableWidget->setColumnWidth(2, this->col_width_acc_name); // Name
    this->ui.tableWidget->setColumnWidth(3, this->col_width_size);  // Width
    this->ui.tableWidget->setColumnWidth(4, this->col_width_size);  // Height
    this->ui.tableWidget->setColumnWidth(5, this->col_width_size);  // Depth
    this->ui.tableWidget->setColumnWidth(6, this->col_width_size);  // Riduzione Z default (per profondità 11mm)
    this->ui.tableWidget->setColumnWidth(7, this->col_width_size);  // Riduzione XY default (per altezza 200mm)
    this->ui.tableWidget->setColumnWidth(8, this->col_width_action);  // Details button
    this->ui.tableWidget->setColumnWidth(9, this->col_width_action);  // Delete button

    QFont rid_font("Calibri", 16);
    int row_number = 1;

    for (int i = 0; i < sculptures.size(); i++) {
        QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
        pb_details->setText("Dettagli");
        QPushButton* pb_delete = new QPushButton(this->ui.tableWidget);
        pb_delete->setText("Elimina");

        // Specific row height to contain the image
        this->ui.tableWidget->setRowHeight(i, img_cell_height);

        QTableWidgetItem* code_widget = new QTableWidgetItem(sculptures[i]["code"]);
        code_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QString pic_path = this->config->getSculpturesPath() + "/" + sculptures[i]["img"];

        // Set the not found image if the provided one is missing
        QFile img_file(pic_path);
        if (!img_file.exists() || sculptures[i]["img"] == "") {
            pic_path = this->images_folder + "notfound.jpg";
        }

        QPixmap pic(pic_path);
        QPixmap resized, resized_h, resized_w;
        resized_h = pic.scaledToHeight(img_cell_height);
        resized_w = pic.scaledToWidth(img_cell_width);

        if (resized_h.width() <= img_cell_width && resized_h.height() <= img_cell_height) {
            resized = resized_h;
        }
        else {
            resized = resized_w;
        }

        QLabel* image_widget = new QLabel(this->ui.tableWidget);
        image_widget->setText("");
        image_widget->setScaledContents(false);
        image_widget->setPixmap(resized);

        QTableWidgetItem* name_widget = new QTableWidgetItem(sculptures[i]["name"]);
        // Set the field as not editable
        name_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QTableWidgetItem* width_widget = new QTableWidgetItem(sculptures[i]["width"]);
        width_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        width_widget->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* height_widget = new QTableWidgetItem(sculptures[i]["height"]);
        height_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        height_widget->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* depth_widget = new QTableWidgetItem(sculptures[i]["depth"]);
        depth_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        depth_widget->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* rid_z_widget = new QTableWidgetItem(
            QString::number(Helpers::scaleFactor(sculptures[i]["depth"].toFloat(), 11.0))
        );
        rid_z_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        rid_z_widget->setTextAlignment(Qt::AlignCenter);
        rid_z_widget->setFont(rid_font);

        QTableWidgetItem* rid_xy_widget = new QTableWidgetItem(
            QString::number(Helpers::scaleFactor(sculptures[i]["height"].toFloat(), 200.0))
        );
        rid_xy_widget->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        rid_xy_widget->setTextAlignment(Qt::AlignCenter);
        rid_xy_widget->setFont(rid_font);

        if (row_number % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        code_widget->setBackground(QBrush(row_bg));
        name_widget->setBackground(QBrush(row_bg));
        width_widget->setBackground(QBrush(row_bg));
        height_widget->setBackground(QBrush(row_bg));
        depth_widget->setBackground(QBrush(row_bg));
        rid_z_widget->setBackground(QBrush(row_bg));
        rid_xy_widget->setBackground(QBrush(row_bg));

        this->ui.tableWidget->setItem(i, 0, code_widget);
        this->ui.tableWidget->setCellWidget(i, 1, image_widget);
        this->ui.tableWidget->setItem(i, 2, name_widget);
        this->ui.tableWidget->setItem(i, 3, width_widget);
        this->ui.tableWidget->setItem(i, 4, height_widget);
        this->ui.tableWidget->setItem(i, 5, depth_widget);
        this->ui.tableWidget->setItem(i, 6, rid_z_widget);
        this->ui.tableWidget->setItem(i, 7, rid_xy_widget);
        this->ui.tableWidget->setCellWidget(i, 8, pb_details);
        this->ui.tableWidget->setCellWidget(i, 9, pb_delete);

        this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotSculptureDetails);
        this->connect(pb_delete, &QPushButton::clicked, this, &Funeraria::slotDeleteItem);

        row_number++;
    }

    // Set the table scroll to have the current row positioned at the center
    this->ui.tableWidget->scrollToItem(
        this->ui.tableWidget->item(row, 0),
        QAbstractItemView::PositionAtCenter
    );

    this->leScName->setFocus();
}

void Funeraria::slotShowSettings() {
    this->settings_ui->setModal(true);
    this->settings_ui->exec();
}

void Funeraria::slotSortColumn(int logical_index) {
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

    this->showTopBar("tomb");

    int client_id = this->client->getId(this->cbClient->currentText());
    int year;
    QString name = this->leDeceased->text().trimmed();

    if (this->cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->cbYear->currentText().toInt();
    }

    QList<QMap<QString, QString>> tombs =
        tomb->getList(client_id, year, this->chbEngraved->isChecked(), name, sort_by, this->sort_column_direction);

    this->showClientOrders(tombs);

    delete tomb;
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
        // Check if among the selected cells there are not numeric values
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
    message.setText("La somma delle " + QString::number(prices) + " lapidi selezionte è: € " + QString::number(sum));
    message.exec();
}

void Funeraria::slotTombDetails()
{
    // Row index of the clicked button
    int row = this->ui.tableWidget->currentRow();
    // Set the progressive property of the Tomb object to the progressive number present in the clicked row
    this->tomb_ui->setProgressive(this->ui.tableWidget->item(row, 0)->text().toInt());
    this->tomb_ui->setModal(true);
    this->tomb_ui->exec();

    this->slotClientOrders(this->tomb_ui->doReload(), row);
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

void Funeraria::slotTombsAlike()
{
    this->tombsAlike_ui->reset();
    this->tombsAlike_ui->setModal(true);
    if (this->tombsAlike_ui->exec() == 1) {
        this->tombsAlike(
            this->client->getId(this->tombsAlike_ui->getClient()),
            this->material->getCode(this->tombsAlike_ui->getMaterial()),
            this->tombsAlike_ui->getEpAmount(),
            this->tombsAlike_ui->getPitsAmount(),
            this->tombsAlike_ui->getRelief(),
            this->tombsAlike_ui->getInscription(),
            this->tombsAlike_ui->getMount(),
            this->tombsAlike_ui->getProvided(),
            this->tombsAlike_ui->getCross(),
            this->tombsAlike_ui->getDrawing(),
            this->tombsAlike_ui->getSculpture()
        );
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
            this->ui.tableWidget->setCellWidget(i, 6, pb_set_engraved); // Details button
            this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotTombDetails);
            this->connect(pb_set_engraved, &QPushButton::clicked, this, &Funeraria::slotSetEngravedTomb);

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

void Funeraria::slotTombsNotPaid()
{
    this->showTopBar("tomb");

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

/********** PRIVATE FUNCTIONS **********/

void Funeraria::addClientOrdersTableRow(const QMap<QString, QString>& tomb, int row)
{
    QPushButton* pb_details = new QPushButton(this->ui.tableWidget);
    pb_details->setIcon(QIcon(this->icons_folder + "detail-50.png"));
    pb_details->setToolTip("Dettagli");
    this->connect(pb_details, &QPushButton::clicked, this, &Funeraria::slotTombDetails);

    QPushButton* pb_open_folder = new QPushButton(this->ui.tableWidget);
    pb_open_folder->setIcon(QIcon(this->icons_folder + "open-folder-50.png"));
    pb_open_folder->setToolTip("Apri");
    this->connect(pb_open_folder, &QPushButton::clicked, this, &Funeraria::slotTombFolder);

    Tomb* tomb_to_check = new Tomb(this->db->db);

    QPushButton* pb_set_paid = new QPushButton(this->ui.tableWidget);
    if (tomb_to_check->isPaid(tomb["progressive"].toInt())) {
        pb_set_paid->setIcon(QIcon(this->icons_folder + "ok-64.png"));
        pb_set_paid->setToolTip("Pagata");
        pb_set_paid->setEnabled(false);
    }
    else {
        pb_set_paid->setIcon(QIcon(this->icons_folder + "moneybag-64.png"));
        pb_set_paid->setToolTip("Conferma pagamento");
        this->connect(pb_set_paid, &QPushButton::clicked, this, &Funeraria::slotSetPaidTomb);
    }

    QPushButton* pb_dynamic = new QPushButton(this->ui.tableWidget);

    if (!tomb_to_check->isConfirmed(tomb["progressive"].toInt())) {
        pb_dynamic->setIcon(QIcon(this->icons_folder + "approved-64.png"));
        pb_dynamic->setToolTip("Imposta confermata");
        this->connect(pb_dynamic, &QPushButton::clicked, this, &Funeraria::slotSetConfirmedTomb);
    }
    else if (!tomb_to_check->isEngraved(tomb["progressive"].toInt())) {
        pb_dynamic->setIcon(QIcon(this->icons_folder + "engraved-64.png"));
        pb_dynamic->setToolTip("Conferma incisione");
        this->connect(pb_dynamic, &QPushButton::clicked, this, &Funeraria::slotSetEngravedTomb);
    }
    else if (!tomb_to_check->isDelivered(tomb["progressive"].toInt())) {
        pb_dynamic->setIcon(QIcon(this->icons_folder + "delivered-80.png"));
        pb_dynamic->setToolTip("Conferma consegna");
        this->connect(pb_dynamic, &QPushButton::clicked, this, &Funeraria::slotSetDeliveredTomb);
    }
    else if (tomb_to_check->isDelivered(tomb["progressive"].toInt())) {
        pb_dynamic->setIcon(QIcon(this->icons_folder + "ok-64.png"));
        pb_dynamic->setToolTip("Ordine completo");
        pb_dynamic->setEnabled(false);
    }

    delete tomb_to_check;

    // Generate the cells' content and set them as not editable
    QTableWidgetItem* progressive = new QTableWidgetItem(tomb["progressive"]);
    progressive->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* name = new QTableWidgetItem(tomb["name"]);
    name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* material = new QTableWidgetItem(this->material->getNameFromCode(tomb["material_code"]));
    material->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* type = new QTableWidgetItem(this->tomb_type->getNameFromCode(tomb["type_code"]));
    type->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* price = new QTableWidgetItem(tomb["price"]);
    price->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* paid = new QTableWidgetItem("");
    paid->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    paid->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem* notes = new QTableWidgetItem(tomb["notes"]);
    notes->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* accessories_mounted = new QTableWidgetItem("");
    accessories_mounted->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    accessories_mounted->setTextAlignment(Qt::AlignCenter);
    
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

    if (tomb["price"] == "0") {
        paid->setText(this->sign_nd);
    }
    else if (tomb["paid"] == "1") {
        paid->setText(this->sign_yes);
    }
    else if (delivery_date != "") {
        this->paid_cell = this->warning_bg;
        paid->setText(this->sign_no);
    }

    if (tomb["accessories_mounted"] == "1") {
        accessories_mounted->setText(this->sign_yes);
    }
    else if (tomb["accessories_mounted"] == "-") {
        accessories_mounted->setText(this->sign_nd);
    }
    else if (tomb["accessories_mounted"] == "0" && engrave_date != "") {
        this->mounted_cell = this->warning_bg;
        accessories_mounted->setText(this->sign_no);
    }

    progressive->setBackground(QBrush(this->row_bg));
    name->setBackground(QBrush(this->row_bg));
    material->setBackground(QBrush(this->row_bg));
    price->setBackground(QBrush(this->row_bg));
    paid->setBackground(QBrush(this->paid_cell));
    notes->setBackground(QBrush(this->row_bg));
    accessories_mounted->setBackground(QBrush(this->mounted_cell));
    type->setBackground(QBrush(this->row_bg));
    confirmed_at->setBackground(QBrush(this->row_bg));
    engraved_at->setBackground(QBrush(this->row_bg));
    delivered_at->setBackground(QBrush(this->row_bg));

    this->ui.tableWidget->setItem(row, 0, progressive);
    this->ui.tableWidget->setItem(row, 1, name);
    this->ui.tableWidget->setItem(row, 2, material);
    this->ui.tableWidget->setItem(row, 3, type);
    this->ui.tableWidget->setItem(row, 4, price);
    this->ui.tableWidget->setItem(row, 5, paid);
    this->ui.tableWidget->setItem(row, 6, notes);
    this->ui.tableWidget->setItem(row, 7, accessories_mounted);
    this->ui.tableWidget->setItem(row, 8, confirmed_at);
    this->ui.tableWidget->setItem(row, 9, engraved_at);
    this->ui.tableWidget->setItem(row, 10, delivered_at);
    this->ui.tableWidget->setCellWidget(row, 11, pb_details); // Details button
    this->ui.tableWidget->setCellWidget(row, 12, pb_open_folder); // Open folder button
    this->ui.tableWidget->setCellWidget(row, 13, pb_set_paid); // Paid tomb button
    this->ui.tableWidget->setCellWidget(row, 14, pb_dynamic); // Dynamic tomb button
}

void Funeraria::clearContainer(QBoxLayout* container)
{
    // Clear the bottom bar
    QLayoutItem* item;
    while ((item = container->takeAt(0)) != 0) {
        // If the item is a pushbutton
        QPushButton* pbutton = qobject_cast<QPushButton*>(item->widget());
        if (pbutton) {
            // Disconnect any connections
            pbutton->disconnect();
            // Reparent the button
            pbutton->setParent(nullptr); // Reparent the button

            delete pbutton;
            pbutton = nullptr;
        }
        else {
            delete item->widget();
        }

        delete item;
    }
}

void Funeraria::clearTable()
{
    int rows = this->ui.tableWidget->rowCount();
    int columns = this->ui.tableWidget->columnCount();

    for (int i = 0; i < rows; i++) {
        this->ui.tableWidget->setRowHeight(i, this->row_height);
        for (int j = 0; j < columns; j++) {
            // Check if the cell's content is a pushbutton
            QPushButton* pbutton = qobject_cast<QPushButton*>(this->ui.tableWidget->cellWidget(i, j));
            if (pbutton) {
                // Disconnect any connections
                pbutton->disconnect();
                // Reparent the button
                pbutton->setParent(nullptr);

                delete pbutton;
                pbutton = nullptr;
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
    this->up = false;
    this->close();
}

void Funeraria::initClientsTopBar()
{
    QFont font;
    font.setPointSize(12);

    // Client's name label
    this->lblClName = new QLabel();
    this->lblClName->setFont(font);
    this->lblClName->setText("Nome");

    // Client's name line edit
    this->leClName = new QLineEdit();
    this->leClName->setFont(font);
    this->leClName->setMaximumWidth(300);

    // Horizontal spacer
    this->clientSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Horizontal layout containing the tombs top bar's elements
    QHBoxLayout* clientsTopBarLayout = new QHBoxLayout(this->ui.clientsTopBarWidget);

    clientsTopBarLayout->addWidget(this->lblClName);
    clientsTopBarLayout->addWidget(this->leClName);
    clientsTopBarLayout->addSpacerItem(this->clientSpacer);
    // Connect the code line edit to the relative slot
    this->connect(this->leClName, &QLineEdit::textChanged, this, &Funeraria::slotFilterClients);

    // Set the widget as visible
    this->ui.clientsTopBarWidget->setVisible(false);
}

void Funeraria::initCrossesTopBar()
{
    QFont font;
    font.setPointSize(12);

    // Code label
    this->lblCrName = new QLabel();
    this->lblCrName->setFont(font);
    this->lblCrName->setText("Nome");

    // Deceased line edit
    this->leCrName = new QLineEdit();
    this->leCrName->setFont(font);
    this->leCrName->setMaximumWidth(300);

    // Horizontal spacer
    this->crossSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Horizontal layout containing the tombs top bar's elements
    QHBoxLayout* crossesTopBarLayout = new QHBoxLayout(this->ui.crossesTopBarWidget);

    crossesTopBarLayout->addWidget(this->lblCrName);
    crossesTopBarLayout->addWidget(this->leCrName);
    crossesTopBarLayout->addSpacerItem(this->crossSpacer);
    // Connect the code line edit to the relative slot
    this->connect(this->leCrName, &QLineEdit::textChanged, this, &Funeraria::slotFilterCrosses);

    // Set the widget as visible
    this->ui.crossesTopBarWidget->setVisible(false);
}

void Funeraria::initDrawingsTopBar()
{
    QFont font;
    font.setPointSize(12);

    // Code label
    this->lblImName = new QLabel();
    this->lblImName->setFont(font);
    this->lblImName->setText("Nome");

    // Deceased line edit
    this->leImName = new QLineEdit();
    this->leImName->setFont(font);
    this->leImName->setMaximumWidth(300);

    // Horizontal spacer
    this->drawingSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Horizontal layout containing the tombs top bar's elements
    QHBoxLayout* drawingTopBarLayout = new QHBoxLayout(this->ui.drawingTopBarWidget);

    drawingTopBarLayout->addWidget(this->lblImName);
    drawingTopBarLayout->addWidget(this->leImName);
    drawingTopBarLayout->addSpacerItem(this->drawingSpacer);
    // Connect the code line edit to the relative slot
    this->connect(this->leImName, &QLineEdit::textChanged, this, &Funeraria::slotFilterDrawing);

    // Set the widget as visible
    this->ui.drawingTopBarWidget->setVisible(false);
}

void Funeraria::initSculpturesTopBar()
{
    QFont font;
    font.setPointSize(12);

    // Code label
    this->lblScName = new QLabel();
    this->lblScName->setFont(font);
    this->lblScName->setText("Nome");

    // Deceased line edit
    this->leScName = new QLineEdit();
    this->leScName->setFont(font);
    this->leScName->setMaximumWidth(300);

    // Horizontal spacer
    this->sculptureSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Horizontal layout containing the tombs top bar's elements
    QHBoxLayout* sculpturesTopBarLayout = new QHBoxLayout(this->ui.sculpturesTopBarWidget);

    sculpturesTopBarLayout->addWidget(this->lblScName);
    sculpturesTopBarLayout->addWidget(this->leScName);
    sculpturesTopBarLayout->addSpacerItem(this->sculptureSpacer);
    // Connect the code line edit to the relative slot
    this->connect(this->leScName, &QLineEdit::textChanged, this, &Funeraria::slotFilterSculptures);

    // Set the widget as visible
    this->ui.sculpturesTopBarWidget->setVisible(false);
}

void Funeraria::initTombsTopBar()
{
    QFont font;
    font.setPointSize(12);

    // Client label
    this->lblClient = new QLabel();
    this->lblClient->setFont(font);
    this->lblClient->setText("Cliente");

    // Combobox containing the clients' list
    this->cbClient = new QComboBox();
    this->cbClient->setFont(font);
    // List of clients' names
    QStringList clients = this->client->getActiveNames();
    // Add the clients to the combo box
    this->cbClient->addItem(this->client_placeholder);
    this->cbClient->addItems(clients);

    // Year label
    this->lblYear = new QLabel();
    this->lblYear->setFont(font);
    this->lblYear->setText("Anno");

    // Combobox containing the years
    this->cbYear = new QComboBox();
    this->cbYear->setFont(font);
    this->cbYear->addItem("Tutti");
    QString this_year = QDate::currentDate().toString("yyyy");
    for (int i = this_year.toInt(); i >= 2020; i--) {
        this->cbYear->addItem(QString::number(i));
    }
    this->cbYear->setCurrentIndex(1);

    // Deceased label
    this->lblDeceased = new QLabel();
    this->lblDeceased->setFont(font);
    this->lblDeceased->setText("Defunto");

    // Deceased line edit
    this->leDeceased = new QLineEdit();
    this->leDeceased->setFont(font);

    // Engraved checkbox
    this->chbEngraved = new QCheckBox();
    this->chbEngraved->setFont(font);
    this->chbEngraved->setText("Incise");
    this->chbEngraved->setCheckable(true);
    this->chbEngraved->setChecked(false);

    // Search by client button
    this->btnSearch = new QPushButton();
    this->btnSearch->setMinimumSize(QSize(0, 30));
    this->btnSearch->setFont(font);
    this->btnSearch->setText("Cerca");

    // Horizontal spacer
    this->tombSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Progressive label
    this->lblSearchByProgressive = new QLabel();
    this->lblSearchByProgressive->setFont(font);
    this->lblSearchByProgressive->setText("Numero");

    // Progressive line edit
    this->leSearchByProgressive = new QLineEdit();
    this->leSearchByProgressive->setFont(font);
    this->leSearchByProgressive->setMaximumWidth(100);

    // Search by progressive button
    this->btnSearchByProgressive = new QPushButton();
    this->btnSearchByProgressive->setMinimumSize(QSize(0, 30));
    this->btnSearchByProgressive->setFont(font);
    this->btnSearchByProgressive->setText("Cerca");

    // Horizontal layout containing the tombs top bar's elements
    QHBoxLayout* tombsTopBarLayout = new QHBoxLayout(this->ui.tombsTopBarWidget);

    // Add the elements to the layout
    tombsTopBarLayout->addWidget(this->lblClient);
    tombsTopBarLayout->addWidget(this->cbClient);
    tombsTopBarLayout->addWidget(this->lblYear);
    tombsTopBarLayout->addWidget(this->cbYear);
    tombsTopBarLayout->addWidget(this->lblDeceased);
    tombsTopBarLayout->addWidget(this->leDeceased);
    tombsTopBarLayout->addWidget(this->chbEngraved);
    tombsTopBarLayout->addWidget(this->btnSearch);
    tombsTopBarLayout->addSpacerItem(this->tombSpacer);
    tombsTopBarLayout->addWidget(this->lblSearchByProgressive);
    tombsTopBarLayout->addWidget(this->leSearchByProgressive);
    tombsTopBarLayout->addWidget(this->btnSearchByProgressive);

    // Connect the buttons to the relative slot
    this->connect(this->btnSearch, &QPushButton::clicked, this, [this] { slotClientOrders(true, 1); });
    this->connect(this->btnSearchByProgressive, &QPushButton::clicked, this, &Funeraria::slotSearchByProgressive);
    // Connect the deceased line edit to the relative slot
    this->connect(this->leDeceased, &QLineEdit::textChanged, this, &Funeraria::slotFilterClientOrders);

    // Set the widget as visible
    this->ui.tombsTopBarWidget->setVisible(true);
}

void Funeraria::initTopBarQuickAccess()
{
    int tooltip_duration = 2500;

    // Reports button
    this->btnQuickReports = new QPushButton();
    this->btnQuickReports->setMinimumSize(QSize(48, 48));
    this->btnQuickReports->setIcon(QIcon(this->icons_folder + "report-64.png"));
    this->btnQuickReports->setIconSize(QSize(32, 32));
    this->btnQuickReports->setToolTip("Reports");
    this->btnQuickReports->setToolTipDuration(tooltip_duration);

    // Clients button
    this->btnQuickClients = new QPushButton();
    this->btnQuickClients->setMinimumSize(QSize(48, 48));
    this->btnQuickClients->setIcon(QIcon(this->icons_folder + "client-list-48.png"));
    this->btnQuickClients->setIconSize(QSize(32, 32));
    this->btnQuickClients->setToolTip("Clienti");
    this->btnQuickClients->setToolTipDuration(tooltip_duration);

    // Sculpture button
    this->btnQuickSculptures = new QPushButton();
    this->btnQuickSculptures->setMinimumSize(QSize(48, 48));
    this->btnQuickSculptures->setIcon(QIcon(this->icons_folder + "sculpture-48.png"));
    this->btnQuickSculptures->setIconSize(QSize(32, 32));
    this->btnQuickSculptures->setToolTip("Sculture");
    this->btnQuickSculptures->setToolTipDuration(tooltip_duration);

    // Sculpture button
    this->btnQuickCrosses = new QPushButton();
    this->btnQuickCrosses->setMinimumSize(QSize(48, 48));
    this->btnQuickCrosses->setIcon(QIcon(this->icons_folder + "cross-72.png"));
    this->btnQuickCrosses->setIconSize(QSize(32, 32));
    this->btnQuickCrosses->setToolTip("Croci");
    this->btnQuickCrosses->setToolTipDuration(tooltip_duration);

    // Drawing button
    this->btnQuickDrawing = new QPushButton();
    this->btnQuickDrawing->setMinimumSize(QSize(48, 48));
    this->btnQuickDrawing->setIcon(QIcon(this->icons_folder + "jesus-50.png"));
    this->btnQuickDrawing->setIconSize(QSize(32, 32));
    this->btnQuickDrawing->setToolTip("Disegni");
    this->btnQuickDrawing->setToolTipDuration(tooltip_duration);

    // Tombs to engrave button
    this->btnQuickToEngrave = new QPushButton();
    this->btnQuickToEngrave->setMinimumSize(QSize(48, 48));
    this->btnQuickToEngrave->setIcon(QIcon(this->icons_folder + "engrave-64.png"));
    this->btnQuickToEngrave->setIconSize(QSize(32, 32));
    this->btnQuickToEngrave->setToolTip("Lapidi da incidere");
    this->btnQuickToEngrave->setToolTipDuration(tooltip_duration);

    // Tombs to mount button
    this->btnQuickToMount = new QPushButton();
    this->btnQuickToMount->setMinimumSize(QSize(48, 48));
    this->btnQuickToMount->setIcon(QIcon(this->icons_folder + "mount-50.png"));
    this->btnQuickToMount->setIconSize(QSize(32, 32));
    this->btnQuickToMount->setToolTip("Lapidi da montare");
    this->btnQuickToMount->setToolTipDuration(tooltip_duration);

    // Tombs to pay button
    this->btnQuickToPay = new QPushButton();
    this->btnQuickToPay->setMinimumSize(QSize(48, 48));
    this->btnQuickToPay->setIcon(QIcon(this->icons_folder + "moneybag-64.png"));
    this->btnQuickToPay->setIconSize(QSize(32, 32));
    this->btnQuickToPay->setToolTip("Lapidi da saldare");
    this->btnQuickToPay->setToolTipDuration(tooltip_duration);

    // Search similar tomb button
    this->btnQuickTombsAlike = new QPushButton();
    this->btnQuickTombsAlike->setMinimumSize(QSize(48, 48));
    this->btnQuickTombsAlike->setIcon(QIcon(this->icons_folder + "detail-50.png"));
    this->btnQuickTombsAlike->setIconSize(QSize(32, 32));
    this->btnQuickTombsAlike->setToolTip("Cerca lapidi simili");
    this->btnQuickTombsAlike->setToolTipDuration(tooltip_duration);

    // New tomb button
    this->btnQuickNewTomb = new QPushButton();
    this->btnQuickNewTomb->setMinimumSize(QSize(48, 48));
    this->btnQuickNewTomb->setIcon(QIcon(this->icons_folder + "add-50.png"));
    this->btnQuickNewTomb->setIconSize(QSize(32, 32));
    this->btnQuickNewTomb->setToolTip("Nuova lapide");
    this->btnQuickNewTomb->setToolTipDuration(tooltip_duration);

    // Horizontal spacer
    this->topQuickAccessSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Horizontal layout containing the top quick access bar's elements
    QHBoxLayout* topBarQuickAccessLayout = new QHBoxLayout(this->ui.topBarQuickAccess);

    // Add the elements to the layout
    topBarQuickAccessLayout->addWidget(this->btnQuickReports);
    topBarQuickAccessLayout->addWidget(this->btnQuickClients);
    topBarQuickAccessLayout->addWidget(this->btnQuickSculptures);
    topBarQuickAccessLayout->addWidget(this->btnQuickCrosses);
    topBarQuickAccessLayout->addWidget(this->btnQuickDrawing);
    topBarQuickAccessLayout->addWidget(this->btnQuickToEngrave);
    topBarQuickAccessLayout->addWidget(this->btnQuickToMount);
    topBarQuickAccessLayout->addWidget(this->btnQuickToPay);
    topBarQuickAccessLayout->addWidget(this->btnQuickTombsAlike);
    topBarQuickAccessLayout->addWidget(this->btnQuickNewTomb);
    topBarQuickAccessLayout->addSpacerItem(this->topQuickAccessSpacer);

    // Connect the top bar's quick buttons to the corresponding slots
    this->connect(this->btnQuickReports, &QPushButton::clicked, this, &Funeraria::slotReport);
    this->connect(this->btnQuickClients, &QPushButton::clicked, this, &Funeraria::slotShowClients);
    this->connect(this->btnQuickSculptures, &QPushButton::clicked, this, &Funeraria::slotShowSculptures);
    this->connect(this->btnQuickCrosses, &QPushButton::clicked, this, &Funeraria::slotShowCrosses);
    this->connect(this->btnQuickDrawing, &QPushButton::clicked, this, &Funeraria::slotShowDrawing);
    this->connect(this->btnQuickToEngrave, &QPushButton::clicked, this, &Funeraria::slotTombsToEngrave);
    this->connect(this->btnQuickToMount, &QPushButton::clicked, this, &Funeraria::slotAccessoriesToMount);
    this->connect(this->btnQuickToPay, &QPushButton::clicked, this, &Funeraria::slotTombsNotPaid);
    this->connect(this->btnQuickTombsAlike, &QPushButton::clicked, this, &Funeraria::slotTombsAlike);
    this->connect(this->btnQuickNewTomb, &QPushButton::clicked, this, &Funeraria::slotNewTomb);
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
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_progressive);   // Progressive
    this->ui.tableWidget->setColumnWidth(1, this->col_width_ord_name);      // Name
    this->ui.tableWidget->setColumnWidth(2, this->col_width_ord_material);  // Material
    this->ui.tableWidget->setColumnWidth(3, this->col_width_acc_name);      // Vase
    this->ui.tableWidget->setColumnWidth(4, this->col_width_acc_name);      // Lamp
    this->ui.tableWidget->setColumnWidth(5, this->col_width_acc_name);      // Flame
    this->ui.tableWidget->setColumnWidth(6, this->col_width_client_name);   // Client
    this->ui.tableWidget->setColumnWidth(7, this->col_width_action);        // Actions
}

void Funeraria::setupClientOrdersTable(int tombs_count)
{
    this->is_table_sortable = true;

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Materiale", "Tipologia", "€", "Pagata", "Note", "Accessori",
        "Conferma", "Incisione", "Consegna", "", "", "", "" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_progressive);       // Progressive
    this->ui.tableWidget->setColumnWidth(1, this->col_width_ord_name);          // Name
    this->ui.tableWidget->setColumnWidth(2, this->col_width_ord_material);      // Material
    this->ui.tableWidget->setColumnWidth(3, this->col_width_ord_type);          // Type
    this->ui.tableWidget->setColumnWidth(4, this->col_width_ord_price);         // Price
    this->ui.tableWidget->setColumnWidth(5, this->col_width_ord_paid);          // Paid
    this->ui.tableWidget->setColumnWidth(6, this->col_width_ord_notes);         // Notes
    this->ui.tableWidget->setColumnWidth(7, this->col_width_ord_accessories);   // Accessories mounted
    this->ui.tableWidget->setColumnWidth(8, this->col_width_ord_date);          // Confirmed at
    this->ui.tableWidget->setColumnWidth(9, this->col_width_ord_date);          // Engraved at
    this->ui.tableWidget->setColumnWidth(10, this->col_width_ord_date);         // Delivered at
    this->ui.tableWidget->setColumnWidth(11, this->col_width_ord_btn);          // Details Button
    this->ui.tableWidget->setColumnWidth(12, this->col_width_ord_btn);          // Open folder Button
    this->ui.tableWidget->setColumnWidth(13, this->col_width_ord_btn);          // Set paid tomb button
    this->ui.tableWidget->setColumnWidth(14, this->col_width_ord_btn);          // Dynamc Button, changes according to the tomb's status
}

void Funeraria::setupTombsNotPaidTable(int tombs_count)
{
    this->current_table = "tomb";

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Prezzo", "Cliente", "Azioni" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_progressive);
    this->ui.tableWidget->setColumnWidth(1, this->col_width_ord_name);
    this->ui.tableWidget->setColumnWidth(2, this->col_width_ord_price);
    this->ui.tableWidget->setColumnWidth(3, this->col_width_client_name);
    this->ui.tableWidget->setColumnWidth(4, this->col_width_action);

}

void Funeraria::setupTombsToEngraveTable(int tombs_count)
{
    this->current_table = "tomb";

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Numero", "Nome", "Materiale", "Cliente", "Conferma", "", "" };

    this->ui.tableWidget->setRowCount(tombs_count);
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_progressive);
    this->ui.tableWidget->setColumnWidth(1, this->col_width_ord_name);
    this->ui.tableWidget->setColumnWidth(2, this->col_width_ord_material);
    this->ui.tableWidget->setColumnWidth(3, this->col_width_client_name);
    this->ui.tableWidget->setColumnWidth(4, this->col_width_ord_date);
    this->ui.tableWidget->setColumnWidth(5, this->col_width_action);
    this->ui.tableWidget->setColumnWidth(6, this->col_width_action);
}

void Funeraria::showClientOrder(const QMap<QString, QString>& tomb)
{
    this->setupClientOrdersTable(1);

    this->addClientOrdersTableRow(tomb, 0);
}

void Funeraria::showClientOrders(const QList<QMap<QString, QString>>& tombs, int row)
{
    this->showTopBar("tomb");

    this->setupClientOrdersTable(tombs.size());

    for (int i = 0; i < tombs.size(); i++) {
        this->addClientOrdersTableRow(tombs[i], i);
    }

    // Set the table scroll to have the current row positioned at the center
    this->ui.tableWidget->scrollToItem(
        this->ui.tableWidget->item(row, 0),
        QAbstractItemView::PositionAtCenter
    );

    this->leDeceased->setFocus();
}

void Funeraria::showTopBar(const QString& bar)
{
    /*
        If the bar string is equal to the current table set
        there is no need to switch the top bar, the correct
        one is already visible
     */
    if (bar == this->current_table) return;

    // Set the current table
    this->current_table = bar;

    // Hide all the top bars
    this->ui.tombsTopBarWidget->setVisible(false);
    this->ui.sculpturesTopBarWidget->setVisible(false);
    this->ui.clientsTopBarWidget->setVisible(false);
    this->ui.crossesTopBarWidget->setVisible(false);
    this->ui.drawingTopBarWidget->setVisible(false);

    // Show the required bar
    if (bar == "tomb") {
        this->ui.tombsTopBarWidget->setVisible(true);
    }
    else if (bar == "sculpture") {
        this->ui.sculpturesTopBarWidget->setVisible(true);
    }
    else if (bar == "client") {
        this->ui.clientsTopBarWidget->setVisible(true);
    }
    else if (bar == "cross") {
        this->ui.crossesTopBarWidget->setVisible(true);
    }
    else if (bar == "drawing") {
        this->ui.drawingTopBarWidget->setVisible(true);
    }
}

void Funeraria::showItems(const QString& type, int row)
{
    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    this->showTopBar(type);

    this->is_table_sortable = false;

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
    else if (type == "frame") {
        accessories = this->frame->get();
    }
    else if (type == "pit") {
        accessories = this->pit->get();
    }
    else if (type == "tomb_type") {
        accessories = this->tomb_type->get();
    }
    else if (type == "tomb_format") {
        accessories = this->tomb_format->get();
    }
    else {
        // The type requested is not valid
        return;
    }

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Codice", "Nome", "", "" };

    this->ui.tableWidget->setRowCount(accessories.size());
    this->ui.tableWidget->setColumnCount(headers.size());
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, this->col_width_code);
    this->ui.tableWidget->setColumnWidth(1, this->col_width_acc_name);
    this->ui.tableWidget->setColumnWidth(2, this->col_width_action);
    this->ui.tableWidget->setColumnWidth(3, this->col_width_action);

    int row_number = 1;
    for (int i = 0; i < accessories.size(); i++) {
        QTableWidgetItem* code = new QTableWidgetItem(accessories[i]["code"]);
        // Set the field as not editable
        code->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QTableWidgetItem* name = new QTableWidgetItem(accessories[i]["name"]);
        // Set the field as not editable
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QPushButton* pb_edit = new QPushButton(this->ui.tableWidget);
        pb_edit->setText("Dettagli");

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

    // Set the table scroll to have the current row positioned at the center
    this->ui.tableWidget->scrollToItem(
        this->ui.tableWidget->item(row, 0),
        QAbstractItemView::PositionAtCenter
    );
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

void Funeraria::tombsAlike(
    int client_id, const QString& material, int ep_amount, int pits_amount, 
    bool relief, bool inscription, bool mount, bool provided, bool cross, bool drawing, bool sculpture
)
{
    Tomb* tomb = new Tomb(this->db->db);

    this->showTopBar("tomb");

    QList<QMap<QString, QString>> tombs = tomb->getAlike(
        client_id, material, ep_amount, pits_amount, relief, inscription, mount, provided, cross, drawing, sculpture
    );

    delete tomb;

    this->showClientOrders(tombs);
}

void Funeraria::updateClientsCombobox()
{
    // List of clients' names
    QStringList clients = this->client->getActiveNames();
    // Clear the combobox
    this->cbClient->clear();
    // Add the clients to the combo box
    this->cbClient->addItem(this->client_placeholder);
    this->cbClient->addItems(clients);
}

void Funeraria::updateQuickAccessNames() {
    // List of the clients to be shown inside the quick access bar
    QStringList quick_access_clients = this->client->getQuickAccessNames();

    QFont font;
    font.setPointSize(12);

    this->clearContainer(this->ui.bottomBar);

    // Add the buttons to the bar
    for (const auto& item : std::as_const(quick_access_clients)) {
        QPushButton* btn = new QPushButton();
        btn->setFont(font);
        btn->setText(item);
        btn->setMinimumSize(QSize(0, 30));
        this->ui.bottomBar->addWidget(btn);

        this->connect(btn, &QPushButton::clicked, this, &Funeraria::slotQuickClientOrders);
    }

    // Add an horizontal spacer as last item to compact the buttons to the left
    QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->ui.bottomBar->addSpacerItem(spacer);
}
