#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_funeraria.h"
#include "Config.h"
#include "DatabaseManager.h"
#include "Client.h"
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
    /*
     * This slot is called when a QListView item is clicked
     *
     * @param   QModelIndex    index
     *
     * @return void
     */
    void slotShowData(QModelIndex index);

private:
    Ui::FunerariaClass ui;

    // Configuration manager
    Config* config;
    // Database manager
    DatabaseManager* db;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Closes the main window.
     *
     * @return void
     */
    void closeWindow();
};
