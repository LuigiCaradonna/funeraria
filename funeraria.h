#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_funeraria.h"
#include "Config.h"
#include "DatabaseManager.h"
#include "Client.h"
#include "Tomb.h"
#include "Flame.h"
#include "Helpers.h"

class Funeraria : public QMainWindow
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Funeraria object.
     *
     * @param	QWidget*	parent	- Parent widget
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
     * This slot is called when a QListView item is clicked
     *
     * @param   QListWidgetItem*    index
     *
     * @return void
     */
    void slotShowData(QListWidgetItem* index);

    void slotFlames();

    void slotNewFlame();

    void slotUpdateEntry(QTableWidgetItem* item);

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

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Closes the main window.
     *
     * @return void
     */
    void closeWindow();
};
