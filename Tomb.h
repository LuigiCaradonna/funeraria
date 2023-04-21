#pragma once
#include <QMainWindow>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>

class Tomb
{
public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Tomb object.
     *
     * @param	const QSqlDatabase&	db	- Reference to the database connection
     */
    Tomb(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Tomb object.
     */
    ~Tomb();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the Tombs
     *
     * @return  QList<QStringList> - A list containing the Tombs' id and name
     */
    QList<QStringList> get(int client_id);

    bool update(QString id, QString value);

private:
    const QString table = "tombs";
    const QSqlDatabase& db;
    QWidget* parent;
};

