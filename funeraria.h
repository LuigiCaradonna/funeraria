#pragma once

#include <QtWidgets/QMainWindow>
#include <QSignalMapper>
#include <QDate>
#include "ui_funeraria.h"
#include "Config.h"
#include "DatabaseManager.h"
#include "Client.h"
#include "Tomb.h"
#include "AccessoryUi.h"
#include "Helpers.h"

class Funeraria : public QMainWindow
{
    Q_OBJECT

public:
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

    // Configuration manager
    Config* config;
    // Database manager
    DatabaseManager* db;

    // Contains the name of the current table shown
    QString current_table = "";

    // Placeholder for the clients' combo box
    QString client_placeholder = "Tutti";

    Client* client;
    Tomb* tomb;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* material;

    QSignalMapper* showItemsMapper;
    QSignalMapper* newItemMapper;

    // Table rows' colors
    QColor paid_cell = QColor(255, 255, 255);
    QColor mounted_cell = QColor(255, 255, 255);
    QColor row_bg = QColor(255, 255, 255);
    QColor row_even = QColor(255, 255, 255);
    QColor row_odd = QColor(235, 235, 235);
    QColor tomb_delivered = QColor(200, 230, 200);
    QColor tomb_to_engrave = QColor(180, 225, 235);
    QColor warning_bg = QColor(250, 200, 70);

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Clears the table content and deletes all the created pointers.
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
};
