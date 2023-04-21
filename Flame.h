#pragma once
#include <QMainWindow>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>

class Flame
{
public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Flame object.
     *
     * @param	const QSqlDatabase&	db	- Reference to the database connection
     */
    Flame(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Flame object.
     */
    ~Flame();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the flames
     * 
     * @return  QList<QStringList> - A list containing the flames' id and name
     */
    QList<QStringList> get();

    bool update(QString id, QString value);

private:
    const QString table = "flames";
    const QSqlDatabase& db;
    QWidget* parent;
};

