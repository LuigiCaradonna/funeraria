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
     * Gets the given cross' datails
     *
     * @param   const QString& code - Code
     *
     * @return  QMap<QString, QString> - A map containing the cross's data
     */
    QMap<QString, QString> getByCode(const QString& code);

    /*
     * Gets the cross's code given its name
     *
     * @param   const QString& name - Cross's name
     *
     * @return  QString - Cross's code
     */
    QString getCode(const QString& name);

    /*
     * Gets all crosses' codes
     *
     * @return QList<QString> - A list of all the crosses' codes
     */
    QList<QString> getCodes();

    /*
     * Gets all the crosses' data
     *
     * @param   const QString& code - Optional, code of the sculputre to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the crosses' data
     */
    QList<QMap<QString, QString>> getListByCode(const QString& code = "");

    /*
     * Gets all the crosses' data
     *
     * @param   const QString& code - Optional, name of the sculputre to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the crosses' data
     */
    QList<QMap<QString, QString>> getListByName(const QString& name = "");

    /*
     * Gets a cross' codes given its name
     *
     * @param const QString& code   - Cross' code
     *
     * @return QString - A cross' code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all crosses' names
     *
     * @return QList<QString> - A list of all the crosses' names
     */
    QList<QString> getNames();

    /*
     * Deletes a cross from the database
     *
     * @param   const QString& code - Code
     *
     * @return  boolean true on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Creates a new cross entry
     *
     * @param const QString& code   -   Code
     * @param const QString& name   -   Name
     * @param const QString& img    -   Image file name
     * @param const QString& width  -   Width in mm
     * @param const QString& height -   Height in mm
     *
     * @return  boolean true if the creation succeeds, false otherwise
     */
    bool store(
        const QString& code,
        const QString& name,
        const QString& img,
        const QString& width,
        const QString& height
    );

    /*
     * Updates the cross's data
     *
     * @param const QString& old_code   -   Old code
     * @param const QString& code       -   Code
     * @param const QString& name       -   Name
     * @param const QString& img        -   Image file name
     * @param const QString& width      -   Width in mm
     * @param const QString& height     -   Height in mm
     *
     * @return  boolean true if the update succeeds, false otherwise
     */
    bool update(
        const QString& old_code,
        const QString& code,
        const QString& name,
        const QString& img,
        const QString& width,
        const QString& height
    );

private:
    const QString table = "cross";
    QSqlDatabase db;
    QString name;
    // Temporary id for a cross to be inserted
    int new_cross_temp_id = 9000;
};
