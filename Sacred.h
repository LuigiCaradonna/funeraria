#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class Sacred : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Sacred object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Sacred(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Sacred object.
     */
    ~Sacred();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Stores a new sacred image
     *
     * @param const QString& code   - Sacred image's code
     * @param const QString& name   - Sacred image's name
     *
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new sacred image
     *
     * @param const QString& old_code   - Sacred image's previous code
     * @param const QString& code   - Sacred image's code
     * @param const QString& name   - Sacred image's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

    /*
     * Updates a new sacred image
     *
     * @param const QString& code   - Sacred image's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Gets all sacred images
     *
     * @return QList<QMap<QString, QString>> - A list of all the sacred images
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all sacred images' names
     *
     * @return QList<QString> - A list of all the sacred images' names
     */
    QList<QString> getNames();

    /*
     * Gets a sacred image's codes given its name
     *
     * @param const QString& code   - Sacred image's code
     *
     * @return QString - A sacred image's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all sacred images' codes
     *
     * @return QList<QString> - A list of all the sacred images' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a sacred image's codes given its name
     *
     * @param const QString& name   - Sacred image's name
     *
     * @return QString - A sacred image's code given its name
     */
    QString getCode(const QString& name);

private:
    const QString table = "sacred";
    QSqlDatabase db;
    Settings* settings;
};
