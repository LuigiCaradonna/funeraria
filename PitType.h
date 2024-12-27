#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class PitType : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the PitType object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    PitType(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the PitType object.
     */
    ~PitType();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Stores a new pit type
     *
     * @param const QString& code   - Pit type's code
     * @param const QString& name   - Pit type's name
     *
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new pit type
     *
     * @param const QString& old_code   - Pit type's previous code
     * @param const QString& code   - Pit type's code
     * @param const QString& name   - Pit type's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

    /*
     * Updates a new pit type
     *
     * @param const QString& code   - Pit type's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Gets all pit types
     *
     * @return QList<QMap<QString, QString>> - A list of all the pit types
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all pit types' names
     *
     * @return QList<QString> - A list of all the pit types' names
     */
    QList<QString> getNames();

    /*
     * Gets a pit type's codes given its name
     *
     * @param const QString& code   - Pit type's code
     *
     * @return QString - A pit type's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all pit types' codes
     *
     * @return QList<QString> - A list of all the pit types' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a pit type's codes given its name
     *
     * @param const QString& name   - Pit type's name
     *
     * @return QString - A pit type's code given its name
     */
    QString getCode(const QString& name);

private:
    const QString table = "pit_type";
    QSqlDatabase db;
    Settings* settings;
};
