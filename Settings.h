#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QDate>
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
     * @param const QMap<QString, QString>& setting - Setting name/value to store
     *
     * @return  boolean true on success, false on faliure
     */
    bool store(const QMap<QString, QString>& setting);

    /*
     * Gets the backup interval
     *
     * @return  int - The backup interval on success, -1 on failure
     */
    const int getBackupInterval();

    /*
     * Gets the number of backups to keep
     *
     * @return  int - The backups to keep on success, -1 on failure
     */
    const int getBackupsToKeep();

private:
    const QString table = "settings";
    QSqlDatabase db;
};

