#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class Settings : public QObject
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Settings object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Settings(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Settings object.
     */
    ~Settings();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the settings from the database
     *
     * @return  QList<QMap<QString, QString>> - A list containing the settings' name and values
     */
    QList<QMap<QString, QString>> get();

    /*
     * Updates a settings into the database
     *
     * @param const QList<QMap<QString, QString>>& settings - Settings name/value to store
     *
     * @return  boolean true on success, false on faliure
     */
    bool store(const QList<QMap<QString, QString>>& settings);

private:
    const QString table = "settings";
    QSqlDatabase db;

};

