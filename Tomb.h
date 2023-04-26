#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include "ui_Tomb.h"

class Tomb : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Tomb object.
     *
     * @param	QSqlDatabase*	db	- Pointer to the database connection
     */
    Tomb(QSqlDatabase* db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Tomb object.
     */
    ~Tomb();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the Tombs
     * 
     * @param int client_id - Client's id
     * @param int year      - Year of the tombs to get, 0 for all the years
     *
     * @return  QList<QStringList> - A list containing the Tombs' id and name
     */
    QList<QStringList> get(int client_id, int year);

    bool update(QString id, QString value);

private:
    Ui::TombClass ui;
    const QString table = "tombs";
    QSqlDatabase* db;
    QWidget* parent;
};
