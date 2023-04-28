#pragma once

#include <QtWidgets/QMainWindow>
#include <QSignalMapper>
#include <QDate>
#include "ui_funeraria.h"
#include "Config.h"
#include "DatabaseManager.h"
#include "Client.h"
#include "Tomb.h"
#include "Accessory.h"

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
    Funeraria(QWidget *parent = nullptr);

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

private:
    Ui::FunerariaClass ui;

    // Configuration manager
    Config* config;
    // Database manager
    DatabaseManager* db;

    // Contains the name of the current table shown
    QString current_table = "";

    // Placeholder for the clients' combo box
    QString client_placeholder = "Cliente";
    
    Client* client;
    Tomb* tomb;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* material;

    QSignalMapper* showItemsMapper;
    QSignalMapper* newItemMapper;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Closes the main window.
     *
     * @return void
     */
    void closeWindow();
};
