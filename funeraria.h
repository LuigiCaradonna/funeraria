#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_funeraria.h"
#include "dbheader.h"

class funeraria : public QMainWindow
{
    Q_OBJECT

public:
    funeraria(QWidget *parent = nullptr);
    ~funeraria();

private:
    Ui::funerariaClass ui;

    QSqlDatabase database;
};
