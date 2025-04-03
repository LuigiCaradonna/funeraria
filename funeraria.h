#pragma once

#include <QtWidgets/QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include <QSignalMapper>
#include <QMenu>
#include <QInputDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPixmap>
#include "ui_funeraria.h"
#include "DatabaseManager.h"
#include "Config.h"
#include "ClientUi.h"
#include "Client.h"
#include "Sculpture.h"
#include "SculptureUi.h"
#include "Drawing.h"
#include "DrawingUi.h"
#include "Cross.h"
#include "CrossUi.h"
#include "SettingsUi.h"
#include "Settings.h"
#include "ReportUi.h"
#include "TombsAlikeUi.h"
#include "TombUi.h"
#include "Tomb.h"
#include "AccessoryUi.h"
#include "Accessory.h"
#include "Helpers.h"

class Funeraria : public QMainWindow
{
    Q_OBJECT

public:
    // Says whether the app was correctly initialized or not
    bool up = true;

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Funeraria object.
     *
     * @param	QWidget* parent	- Parent widget
     */
    Funeraria(QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Funeraria object.
     */
    ~Funeraria();

    /********** PUBLIC FUNCTIONS **********/

protected slots:

    /********** SLOTS **********/

    /*
     * Sets the tomb's accessories as mounted
     *
     * @return void
     */
    void slotSetAccessoriesMounted();

    /*
     * Shows the accessories needed by the tombs not yet mounted
     *
     * @return void
     */
    void slotAccessoriesToMount();

    /*
     * Saves the database in CSV format
     *
     * @return void
     */
    void slotBackupDbToCSV();

    /*
     * Shows a dialog window containing the selected client's details
     *
     * @return void
     */
    void slotClientDetails();

    /*
     * Shows the selected client's orders
     *
     * @param bool reload - Whether to reload the client's orders list or not
     * @param int row - Row where to center the scroll view
     *
     * @return void
     */
    void slotClientOrders(bool reload = true, int row = 1);

    /*
     * Shows a dialog window containing the selected cross's details
     *
     * @return void
     */
    void slotCrossDetails();

    /*
     * Deletes the selected item from the database
     *
     * @return void
     */
    void slotDeleteItem();

    /*
     * Shows a dialog window containing the selected images's details
     *
     * @return void
     */
    void slotDrawingDetails();

    /*
     * Edits the selected item
     *
     * @return void
     */
    void slotEditItem();

    /*
     * Updates the selected client's orders list shown applying the filter (deceased name string)
     *
     * @return void
     */
    void slotFilterClientOrders();

    /*
     * Updates the clients' list applying the filter (client's name string)
     *
     * @return void
     */
    void slotFilterClients();

    /*
     * Updates the crosses' orders list shown applying the filter (name string)
     *
     * @return void
     */
    void slotFilterCrosses();

    /*
     * Updates the images' orders list shown applying the filter (name string)
     *
     * @return void
     */
    void slotFilterDrawing();

    /*
     * Updates the sculptures's orders list shown applying the filter (name string)
     *
     * @return void
     */
    void slotFilterSculptures();

    /*
     * Shows the dialog to insert a new client
     *
     * @return void
     */
    void slotNewClient();

    /*
     * Shows the dialog to insert a new cross
     *
     * @return void
     */
    void slotNewCross();

    /*
     * Shows the dialog to insert a new image
     *
     * @return void
     */
    void slotNewDrawing();

    /*
     * Shows the dialog to insert a new accessory
     *
     * @param   const QString& type - Name of the accessories type to add
     *
     * @return void
     */
    void slotNewItem(const QString& type);

    /*
     * Shows the dialog to insert a new sculpture
     *
     * @return void
     */
    void slotNewSculpture();

    /*
     * Shows the dialog to insert a new tomb
     *
     * @return void
     */
    void slotNewTomb();

    /*
     * Iterates over the selected rows and gets informations to save a list containing the data of the tombs
     * to be paid into a pdf file.
     *
     * @return void
     */
    void slotPrintToPayListPdf();

    /*
     * Iterates over the selected rows and gets informations to save a list containing the data of the tombs
     * to be paid into a txt file.
     *
     * @return void
     */
    void slotPrintToPayListTxt();

    /*
     * Shows the selected client's orders when using the quick access buttons
     *
     * @return void
     */
    void slotQuickClientOrders();

    /*
     * Shows the report dialog
     *
     * @return void
     */
    void slotReport();

    /*
     * Shows a dialog window containing the selected sculpture's details
     *
     * @return void
     */
    void slotSculptureDetails();

    /*
     * Searches for tombs similar to the one provided. If an array of tombs is provided, the search is done on the first one.
     *
     * @return void
     */
    void slotSearchAlike();

    /*
     * Shows an order given its progressive number
     *
     * @return void
     */
    void slotSearchByProgressive();

    /*
     * Sets a tomb as confirmed
     *
     * @return void
     */
    void slotSetConfirmedTomb();

    /*
     * Sets a tomb as delivered
     *
     * @return void
     */
    void slotSetDeliveredTomb();

    /*
     * Sets a tomb as Engraved
     *
     * @return void
     */
    void slotSetEngravedTomb();

    /*
     * Sets a tomb as paid
     *
     * @return void
     */
    void slotSetPaidTomb();

    /*
     * Shows the clients' list
     *
     * @param int row - Row where to center the scroll view
     *
     * @return void
     */
    void slotShowClients(int row = 1);

    /*
     * Shows a context menu when right clicking on the QTableWidget, containis the proper actions according to the current table status
     * 
     * @param const QPoint& pos - Mouse pointer's position where the right button was clicked
     *
     * @return void
     */
    void slotShowContextMenu(const QPoint& pos);

    /*
     * Shows the crosses' list
     *
     * @param int row - Row where to center the scroll view
     *
     * @return void
     */
    void slotShowCrosses(int row = 1);

    /*
     * Shows the images' list
     *
     * @param int row - Row where to center the scroll view
     *
     * @return void
     */
    void slotShowDrawing(int row = 1);

    /*
     * Calls the showItem method passing it the required parameters
     *
     * @param   const QString& type - Name of the accessories type to show
     *
     * @return void
     */
    void slotShowItems(const QString& type);

    /*
     * Shows the sculptures' list
     *
     * @param int row - Row where to center the scroll view
     *
     * @return void
     */
    void slotShowSculptures(int row = 1);

    /*
     * Shows a dialog window containing the settings
     *
     * @return void
     */
    void slotShowSettings();

    /*
     * Sorts the table's rows according to the clicked column name.
     *
     * @param int logical_index - Clicked column index
     *
     * @return void
     */
    void slotSortColumn(int logical_index);

    /*
     * Iterates over the selected cells, sums their content and shows a dialog containig the result.
     *
     * @return void
     */
    void slotSumSelectedPrices();

    /*
     * Shows a dialog window containing the selected tomb's details
     *
     * @return void
     */
    void slotTombDetails();

    /*
     * Opens the tomb's folder
     *
     * @return void
     */
    void slotTombFolder();

    /*
     * Shows an input window where to provide a tomb's properties and to look for similar tombs
     *
     * @return void
     */
    void slotTombsAlike();

    /*
     * Shows the tombs which can be engraved
     *
     * @return void
     */
    void slotTombsToEngrave();

    /*
     * Shows the tombs not yet paid
     *
     * @return void
     */
    void slotTombsNotPaid();

    /*
     * Shows a client's order given its progressive number
     *
     * @return void
     */
    void slotTombByProgressive();

private:
    Ui::FunerariaClass ui;

    // Database manager
    DatabaseManager* db;

    // Config manager
    Config* config;

    // Contains the name of the current table shown
    QString current_table = "";

    // Placeholder for the clients' combo box
    QString client_placeholder = "Tutti";

    QMenu* context_menu;

    Client* client;
    ClientUi* client_ui;
    SettingsUi* settings_ui;
    TombUi* tomb_ui;
    TombsAlikeUi* tombsAlike_ui;
    Sculpture* sculpture;
    SculptureUi* sculpture_ui;
    Cross* cross;
    CrossUi* cross_ui;
    Drawing* drawing;
    DrawingUi* drawing_ui;
    ReportUi* report_ui;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* pit;
    Accessory* frame;
    Accessory* material;
    Accessory* tomb_type;
    Accessory* tomb_format;

    QSignalMapper* show_items_mapper;
    QSignalMapper* new_item_mapper;

    // Table rows' colors
    QColor paid_cell = QColor(255, 255, 255);
    QColor mounted_cell = QColor(255, 255, 255);
    QColor row_bg = QColor(255, 255, 255);
    QColor row_odd = QColor(255, 255, 255);
    QColor row_even = QColor(235, 235, 235);
    QColor tomb_delivered_odd = QColor(200, 240, 200);
    QColor tomb_delivered_even = QColor(180, 211, 180);
    QColor tomb_to_engrave = QColor(180, 225, 235);
    QColor warning_bg = QColor(250, 200, 70);
    QColor critical_bg = QColor(255, 140, 140);

    // Whether a table should be reloaded or not
    bool reload_table = true;
    // Whether the currently shown table is sortable or not
    bool is_table_sortable = true;
    // Last selected column to sort
    QString last_sorting_column = "";
    // Direction to use to sort the selected table's column (ASC/DESC)
    QString sort_column_direction = "";

    const QString fonts_folder = "assets/fonts/";
    const QString icons_folder = "assets/icons/";
    const QString images_folder = "assets/img/";
    const QString css_folder = "assets/css/";
    const QString not_engraved = "No";
    const QString name_not_defined = "Non definito";

    const QString sign_yes = "✓";
    const QString sign_no = "X";
    const QString sign_nd = "―";

    // Single table's row height
    const int row_height = 30;

    /* Tables' columns width */

    const int col_width_acc_name = 200;
    const int col_width_action = 90;
    const int col_width_client_name = 200;
    const int col_width_code = 90;
    const int col_width_drawing_name = 300;
    const int col_width_email = 300;
    const int col_width_id = 60;
    const int col_width_ord_accessories = 65;
    const int col_width_ord_btn = 40;
    const int col_width_ord_date = 90;
    const int col_width_ord_type = 220;
    const int col_width_ord_material = 150;
    const int col_width_ord_name = 215;
    const int col_width_ord_notes = 630;
    const int col_width_ord_paid = 55;
    const int col_width_ord_price = 45;
    const int col_width_phone = 250;
    const int col_width_position = 80;
    const int col_width_progressive = 55;
    const int col_width_size = 80;

    /* Top quick access bar elements */

    QPushButton* btnQuickReports;
    QPushButton* btnQuickClients;
    QPushButton* btnQuickSculptures;
    QPushButton* btnQuickCrosses;
    QPushButton* btnQuickDrawing;
    QPushButton* btnQuickToEngrave;
    QPushButton* btnQuickToPay;
    QPushButton* btnQuickToMount;
    QPushButton* btnQuickTombsAlike;
    QPushButton* btnQuickNewTomb;
    QSpacerItem* topQuickAccessSpacer;

    // Tombs top bar elements
    QLabel* lblClient;
    QComboBox* cbClient;
    QLabel* lblYear;
    QComboBox* cbYear;
    QLabel* lblDeceased;
    QLineEdit* leDeceased;
    QCheckBox* chbEngraved;
    QPushButton* btnSearch;
    QSpacerItem* tombSpacer;
    QLabel* lblSearchByProgressive;
    QLineEdit* leSearchByProgressive;
    QPushButton* btnSearchByProgressive;

    // Sculptures top bar elements
    QLabel* lblScName;
    QLineEdit* leScName;
    QSpacerItem* sculptureSpacer;

    // Crosses top bar elements
    QLabel* lblCrName;
    QLineEdit* leCrName;
    QSpacerItem* crossSpacer;

    // Drawing top bar elements
    QLabel* lblImName;
    QLineEdit* leImName;
    QSpacerItem* drawingSpacer;

    // Clients top bar elements
    QLabel* lblClName;
    QLineEdit* leClName;
    QSpacerItem* clientSpacer;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Adds a row to the table that shows the client's orders.
     *
     * @param QMap<QString, QString> &tomb  - Tomb's data
     * @param int row  - Row's number
     *
     * @return void
     */
    void addClientOrdersTableRow(const QMap<QString, QString>& tomb, int row);

    /*
     * Removes all the content from a container.
     *
     * @param QBoxLayout* container   - Pointer to the container to clear
     *
     * @return void
     */
    void clearContainer(QBoxLayout* container);

    /*
     * Clears the table content and all the created pointers.
     *
     * @return void
     */
    void clearTable();

    /*
     * Closes the main window.
     *
     * @return void
     */
    void closeWindow();

    /*
     * Initialize the top bar for the clients view.
     *
     * @return void
     */
    void initClientsTopBar();

    /*
     * Initialize the top bar for the crosses view.
     *
     * @return void
     */
    void initCrossesTopBar();

    /*
     * Initialize the top bar for the drawing images view.
     *
     * @return void
     */
    void initDrawingsTopBar();

    /*
     * Initialize the top bar for the sculptures view.
     *
     * @return void
     */
    void initSculpturesTopBar();

    /*
     * Initialize the top bar for the tombs view.
     *
     * @return void
     */
    void initTombsTopBar();

    /*
     * Initialize the top quick access bar.
     *
     * @return void
     */
    void initTopBarQuickAccess();

    /*
     * Sets up the table to show the accessories to be mounted.
     *
     * @param tombs_count  - Number of tombs to show
     *
     * @return void
     */
    void setupAccessoriesToMountTable(int tombs_count);

    /*
     * Sets up the table to show a client's orders.
     *
     * @param tombs_Count  - Number of tombs to show
     *
     * @return void
     */
    void setupClientOrdersTable(int tombs_count);

    /*
     * Sets up the table to show the tombs to be paid.
     *
     * @param tombs_Count  - Number of tombs to show
     *
     * @return void
     */
    void setupTombsNotPaidTable(int tombs_count);

    /*
     * Sets up the table to show the tombs to be engraved.
     *
     * @param tombs_count  - Number of tombs to show
     *
     * @return void
     */
    void setupTombsToEngraveTable(int tombs_count);

    /*
     * Shows a single client's order.
     *
     * @param const QList<QMap<QString, QString>> &tombs   - List of the client's orders
     *
     * @return void
     */
    void showClientOrder(const QMap<QString, QString>& tomb);

    /*
     * Fills the table to show the client's orders.
     *
     * @param const QList<QMap<QString, QString>> &tombs   - List of the client's orders
     * @param int row - Row where to center the scroll view
     *
     * @return void
     */
    void showClientOrders(const QList<QMap<QString, QString>>& tombs, int row = 1);

    /*
     * Swithces off all the top bars and shows the one required.
     *
     * @return void
     */
    void showTopBar(const QString& bar);

    /*
     * Shows a list of items of the given type.
     *
     * @param const QString &type   - List of the client's orders
     * @param int row               - Row where to center the scroll view
     *
     * @return void
     */
    void showItems(const QString& type, int row = 1);

    /*
     * Sets the sorting direction for the selected column.
     *
     * @param const QString& column - column name
     *
     * @return void
     */
    void sortColumnDirection(const QString& column);

    /*
     * Shows the list of similar tombs given the provided parameters.
     *
     * @param int client_id             - Client's id
     * @param const QString& material   - Material's code
     * @param int ep_amount             - Amount of epigraphs
     * @param int pits_amount           - Amount of pits
     * @param bool relief               - Relief epigraph's presence
     * @param bool inscription          - Inscription's presence
     * @param bool mount                - Whether the tomb was assembled
     * @param bool provided             - Whether the material was provided by the client
     * @param bool cross                - Cross' presence
     * @param bool drawing              - Drawing's presence
     * @param bool sculpture            - Sculpture's presence
     *
     * @return void
     */
    void tombsAlike(
        int client_id,const QString& material, int ep_amount, int pits_amount, 
        bool relief, bool inscription, bool mount, bool provided, bool cross, bool drawing, bool sculpture);

    /*
     * Updates the combobox containing the clients.
     *
     * @return void
     */
    void updateClientsCombobox();

    /*
     * Updates the quick access bar using only the selected clients.
     *
     * @return void
     */
    void updateQuickAccessNames();
};
