#pragma once

#include <QtWidgets/QMainWindow>
#include <QSignalMapper>
#include "ui_funeraria.h"
#include "Config.h"
#include "DatabaseManager.h"
#include "Client.h"
#include "Tomb.h"
#include "Flame.h"

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
     * Shows the selected client's orders
     *
     * @param   QListWidgetItem* index
     *
     * @return void
     */
    void slotClientOrders(QListWidgetItem* index);

    void slotClientDetails();

    /*
     * Shows the clients' list
     *
     * @return void
     */
    void slotShowClients();

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
    
    Client* client;
    Tomb* tomb;
    // Vase* vase;
    // Lamp* lamp;
    Flame* flame;

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
