#pragma once
#include <QMainWindow>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>

class Client
{
public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Client object.
     *
     * @param	const QSqlDatabase&	db	- Reference to the database connection
     */
    Client(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Client object.
     */
    ~Client();

    /********** PUBLIC FUNCTIONS **********/

    QStringList getNames();

    QList<QMap<QString, QString>> getDetails();
    QMap<QString, QString> getDetails(const QString& name);

private:
    const QSqlDatabase& db;
    QWidget* parent;
};

