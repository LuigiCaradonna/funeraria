#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_funeraria.h"
#include "config.h"
#include "dbheader.h"

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

    /********** INIT FUNCTIONS **********/


private:
    Ui::FunerariaClass ui;

    QSqlDatabase db;

    // Configuration manager
    Config* config;
};
