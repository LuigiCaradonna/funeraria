#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class Cross : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Cross object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Cross(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Cross object.
     */
    ~Cross();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Stores a new cross
     *
     * @param const QString& code   - Cross's code
     * @param const QString& name   - Cross's name
     *
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new cross
     *
     * @param const QString& old_code   - Cross's previous code
     * @param const QString& code   - Cross's code
     * @param const QString& name   - Cross's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

    /*
     * Updates a new cross
     *
     * @param const QString& code   - Cross's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Gets all crosses
     *
     * @return QList<QMap<QString, QString>> - A list of all the crosss
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all crosses' names
     *
     * @return QList<QString> - A list of all the crosss' names
     */
    QList<QString> getNames();

    /*
     * Gets a cross's codes given its name
     *
     * @param const QString& code   - Cross's code
     *
     * @return QString - A cross's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all crosses' codes
     *
     * @return QList<QString> - A list of all the crosss' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a cross's codes given its name
     *
     * @param const QString& name   - Cross's name
     *
     * @return QString - A cross's code given its name
     */
    QString getCode(const QString& name);

private:
    const QString table = "cross";
    QSqlDatabase db;
    Settings* settings;
};
