#pragma once

#include <QtWidgets/QMainWindow>
#include <QSignalMapper>
#include <QMenu>
#include "ui_funeraria.h"
#include "DatabaseManager.h"
#include "ClientUi.h"
#include "Client.h"
#include "SettingsUi.h"
#include "Settings.h"
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
     * Shows a context menu when right clicking on the QTableWidget, containis the proper actions according to the current table status
     *
     * @return void
     */
    void slotShowContextMenu(const QPoint& pos);

    /*
     * Iterates over the selected cells, sums their content and shows a dialog containig the result.
     */
    void slotSumSelectedPrices();

    /*
     * Sorts the table's rows according to the clicked column name.
     * 
     * @return void
     */
    void slotHeaderClicked(int logicalIndex);

    /*
     * Saves the database in CSV format
     *
     * @return void
     */
    void slotBackupDbToCSV();

    /*
     * Shows a dialog window containing the settings
     *
     * @return void
     */
    void slotShowSettings();

    /*
     * Updates the selected client's orders list shown applying the filter (deceased name string)
     *
     * @return void
     */
    void slotFilterClientOrders();

    /*
     * Shows the selected client's orders
     *
     * @return void
     */
    void slotClientOrders();

    /*
     * Shows the selected client's orders when using the quick access buttons
     *
     * @param   const QString& client - Name of the client to show
     *
     * @return void
     */
    void slotQuickClientOrders();

    /*
     * Shows a dialog window containing the selected tomb's details
     *
     * @return void
     */
    void slotTombDetails();

    /*
     * Shows the dialog to insert a new tomb
     *
     * @return void
     */
    void slotNewTomb();

    /*
     * Shows the clients' list
     *
     * @return void
     */
    void slotShowClients();

    /*
     * Shows a dialog window containing the selected client's details
     *
     * @return void
     */
    void slotClientDetails();

    /*
     * Shows the dialog to insert a new client
     *
     * @return void
     */
    void slotNewClient();

    /*
     * Shows the accessories' list
     *
     * @param   const QString& type - Name of the accessories type to show
     *
     * @return void
     */
    void slotShowItems(const QString& type);

    /*
     * Shows the dialog to insert a new accessory
     *
     * @param   const QString& type - Name of the accessories type to add
     *
     * @return void
     */
    void slotNewItem(const QString& type);

    /*
     * Persists into the database the change made to a table cell
     *
     * @return void
     */
    void slotUpdateEntry();

    /*
     * Deletes the selected item from the database
     *
     * @return void
     */
    void slotDelete();

    /*
     * Shows the tombs needed which can be engraved
     *
     * @return void
     */
    void slotTombsToEngrave();

    /*
     * Shows the accessories needed by the tombs not yet mounted
     *
     * @return void
     */
    void slotAccessoriesToMount();

    /*
     * Shows the tombs not yet paid
     *
     * @return void
     */
    void slotTombsNotPaid();

private:
    Ui::FunerariaClass ui;

    // Database manager
    DatabaseManager* db;

    // Contains the name of the current table shown
    QString current_table = "";

    // Placeholder for the clients' combo box
    QString client_placeholder = "Tutti";

    QString config_file = "config.cfg";
    QString default_db_path = "./funeraria.db";

    QMenu* context_menu;

    Client* client;
    ClientUi* client_ui;
    SettingsUi* settings_ui;
    TombUi* tombUi;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* material;

    QSignalMapper* show_items_mapper;
    QSignalMapper* new_item_mapper;

    // Table rows' colors
    QColor paid_cell = QColor(255, 255, 255);
    QColor mounted_cell = QColor(255, 255, 255);
    QColor row_bg = QColor(255, 255, 255);
    QColor row_even = QColor(255, 255, 255);
    QColor row_odd = QColor(235, 235, 235);
    QColor tomb_delivered = QColor(200, 230, 200);
    QColor tomb_to_engrave = QColor(180, 225, 235);
    QColor warning_bg = QColor(250, 200, 70);

    // Whether the currently shown table is sortable or not
    bool is_table_sortable = true;
    // Last selected column to sort
    QString last_sorting_column = "";
    // Direction to use to sort the selected table's column (ASC/DESC)
    QString sort_column_direction = "";

    /********** PRIVATE FUNCTIONS **********/

    bool initConfigFile();

    /*
     * Clears the table content and deletes all the created pointers.
     *
     * @return void
     */
    void clearTable();

    /*
     * Fills the table to show the client's orders.
     *
     * @param QList<QMap<QString, QString>> tombs   - List of the client's orders
     *
     * @return void
     */
    void showClientOrders(QList<QMap<QString, QString>> tombs);

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

    /*
     * Closes the main window.
     *
     * @return void
     */
    void closeWindow();

    /*
     * Sets the sorting direction for the selected column.
     *
     * @param const QString& column - column name
     *
     * @return void
     */
    void sortColumnDirection(const QString& column);
};
