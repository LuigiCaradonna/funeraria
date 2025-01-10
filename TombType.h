#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class TombType : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the TombType object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    TombType(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the TombType object.
     */
    ~TombType();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all tomb types
     *
     * @return QList<QMap<QString, QString>> - A list of all the tomb types
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets a tomb type's codes given its name
     *
     * @param const QString& code   - Tomb type's code
     *
     * @return QString - A tomb type's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all tomb types' names
     *
     * @return QList<QString> - A list of all the tomb types' names
     */
    QList<QString> getNames();

    /*
     * Gets a tomb type's codes given its name
     *
     * @param const QString& name   - Tomb type's name
     *
     * @return QString - A tomb type's code given its name
     */
    QString getCode(const QString& name);

    /*
     * Gets all tomb types' codes
     *
     * @return QList<QString> - A list of all the tomb types' codes
     */
    QList<QString> getCodes();

    /*
     * Updates a new tomb type
     *
     * @param const QString& code   - Tomb type's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Stores a new tomb type
     *
     * @param const QString& code   - Tomb type's code
     * @param const QString& name   - Tomb type's name
     *
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new tomb type
     *
     * @param const QString& old_code   - Tomb type's previous code
     * @param const QString& code   - Tomb type's code
     * @param const QString& name   - Tomb type's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

private:
    const QString table = "tomb_type";
    QSqlDatabase db;
    Settings* settings;
};

