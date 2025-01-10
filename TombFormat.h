#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class TombFormat : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the TombFormat object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    TombFormat(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the TombFormat object.
     */
    ~TombFormat();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all tomb formats
     *
     * @return QList<QMap<QString, QString>> - A list of all the tomb formats
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets a tomb format's codes given its name
     *
     * @param const QString& name   - Tomb format's name
     *
     * @return QString - A tomb format's code given its name
     */
    QString getCode(const QString& name);

    /*
     * Gets all tomb formats' codes
     *
     * @return QList<QString> - A list of all the tomb formats' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a tomb format's codes given its code
     *
     * @param const QString& code   - Tomb format's code
     *
     * @return QString - A tomb format's code given its code
     */
    QString getName(const QString& code);

    /*
     * Gets all tomb formats' names
     *
     * @return QList<QString> - A list of all the tomb formats' names
     */
    QList<QString> getNames();

    /*
     * Updates a new tomb format
     *
     * @param const QString& code   - Tomb format's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Stores a new tomb format
     *
     * @param const QString& code   - Tomb format's code
     * @param const QString& name   - Tomb format's name
     *
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new tomb format
     *
     * @param const QString& old_code   - Tomb format's previous code
     * @param const QString& code   - Tomb format's code
     * @param const QString& name   - Tomb format's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

private:
    const QString table = "tomb_format";
    QSqlDatabase db;
    Settings* settings;
};
