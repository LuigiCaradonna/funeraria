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

    /*
     * Reads the config file and sets the corresponding options.
     *
     * @return void
     */
    void initOptions();


private:
    Ui::FunerariaClass ui;

    QSqlDatabase database;

    QString db_path;
    const QString config_file = "config.cfg";

    // Configuration manager
    Config* config;
};
