#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class PitFormat : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the PitFormat object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    PitFormat(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the PitFormat object.
     */
    ~PitFormat();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Stores a new pit format
     * 
     * @param const QString& code   - Pit format's code
     * @param const QString& name   - Pit format's name
     * 
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new pit format
     *
     * @param const QString& old_code   - Pit format's previous code
     * @param const QString& code   - Pit format's code
     * @param const QString& name   - Pit format's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

    /*
     * Updates a new pit format
     *
     * @param const QString& code   - Pit format's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Gets all pit formats
     *
     * @return QList<QMap<QString, QString>> - A list of all the pit formats
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all pit formats' names
     *
     * @return QList<QString> - A list of all the pit formats' names
     */
    QList<QString> getNames();

    /*
     * Gets a pit format's codes given its name
     *
     * @param const QString& code   - Pit format's code
     *
     * @return QString - A pit format's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all pit formats' codes
     *
     * @return QList<QString> - A list of all the pit formats' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a pit format's codes given its name
     *
     * @param const QString& name   - Pit format's name
     *
     * @return QString - A pit format's code given its name
     */
    QString getCode(const QString& name);

private:
    const QString table = "pit_format";
    QSqlDatabase db;
    Settings* settings;
};

