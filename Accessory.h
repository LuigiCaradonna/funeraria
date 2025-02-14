#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

class Accessory : QObject
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the accessory object.
     *
     * @param	const QSqlDatabase&	db  - Reference to the database connection
     * @param	const QString&	table	- Database table to use
     */
    Accessory(const QSqlDatabase& db, const QString& table);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the accessory object.
     */
    ~Accessory();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the accessories' data
     *
     * @return  QList<QMap<QString, QString>> - A list containing the accessories' data
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets the accessorie's code given its name
     *
     * @param const QString& name   - Name
     *
     * @return  QString - The accessory's code if found, empty string otherwise
     */
    QString getCode(const QString& name);

    /*
     * Gets the accessory name given its code
     *
     * @param const QString& code   - Code
     *
     * @return  QString - The accessory's code id
     */
    QString getNameFromCode(const QString& code);

    /*
     * Gets all the accessories' names
     *
     * @return  QStringList - A list containing the accessories' names
     */
    QStringList getNames();

    /*
     * Removes a accessory from the database
     *
     * @param const QString& code - Code
     *
     * @return  Boolean true on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Adds an accessory into the database
     *
     * @param const QString& code   - Code
     * @param const QString& name   - Name
     *
     * @return  Boolean true on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     * Updates an accessory into the database
     *
     * @param const QString& old_code   - Previous code
     * @param const QString& code       - Code
     * @param const QString& name       - Name
     *
     * @return  Boolean true on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

private:
    const QString table;
    QSqlDatabase db;
};

