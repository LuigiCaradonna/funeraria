#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class Pit : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Pit object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Pit(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Pit object.
     */
    ~Pit();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Stores a new pit
     * 
     * @param const QString& code   - Pit format's code
     * @param const QString& name   - Pit format's name
     * 
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new pit
     *
     * @param const QString& old_code   - Pit format's previous code
     * @param const QString& code   - Pit format's code
     * @param const QString& name   - Pit format's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

    /*
     * Updates a new pit
     *
     * @param const QString& code   - Pit format's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Gets all pits
     *
     * @return QList<QMap<QString, QString>> - A list of all the pits
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all pits' names
     *
     * @return QList<QString> - A list of all the pits' names
     */
    QList<QString> getNames();

    /*
     * Gets a pit's codes given its name
     *
     * @param const QString& code   - Pit format's code
     *
     * @return QString - A pit's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all pits' codes
     *
     * @return QList<QString> - A list of all the pits' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a pit's codes given its name
     *
     * @param const QString& name   - Pit format's name
     *
     * @return QString - A pit's code given its name
     */
    QString getCode(const QString& name);

private:
    const QString table = "pit";
    QSqlDatabase db;
    Settings* settings;
};

