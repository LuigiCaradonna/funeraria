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
     * @param	QSqlDatabase*	db	    - Pointer to the database connection
     * @param	const QString&	table	- Database table to use
     */
    Accessory(QSqlDatabase* db, const QString& table);

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
     * Gets all the accessories' names
     *
     * @param const QString& name   - The accessory's name
     *
     * @return  QString - The accessory's code id found, empty string otherwise
     */
    QString getCode(const QString& name);

    /*
     * Gets all the accessories' names
     *
     * @param const QString& name   - The accessory's name
     *
     * @return  QString - The accessory's code id
     */
    QString getNameFromId(const QString& code);

    /*
     * Gets all the accessories' names
     *
     * @return  QStringList - A list containing the accessories' names
     */
    QStringList getNames();

    /*
     * Adds an accessory into the database
     *
     * @param const QString& code   - Accessory's code
     * @param const QString& name   - Accessory's name
     *
     * @return  QString - Empty on success, failure cause on failure
     */
    QString store(const QString& code, const QString& name);

    /*
     * Updates an accessory into the database
     *
     * @param const QString& code   - Accessory's code
     * @param const QString& name   - Accessory's name
     *
     * @return  QString - Empty on success, failure cause on failure
     */
    QString update(const QString& code, const QString& name);

    /*
     * Removes a accessory from the database
     *
     * @param const QString& code - Accessory's code
     *
     * @return  QString - Empty on success, failure cause on failure
     */
    QString remove(const QString& code);

private:
    const QString table;
    QSqlDatabase* db;
};

