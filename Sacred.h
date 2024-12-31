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
     * Gets all the images' data
     *
     * @param   const QString& code - Optional, code of the image to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the images' data
     */
    QList<QMap<QString, QString>> getListByCode(const QString& code = "");

    /*
     * Gets all the images' data
     *
     * @param   const QString& code - Optional, name of the image to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the images' data
     */
    QList<QMap<QString, QString>> getListByName(const QString& name = "");

    /*
     * Gets the given image's datails
     *
     * @param   const QString& code - Code
     *
     * @return  QMap<QString, QString> - A map containing the image's data
     */
    QMap<QString, QString> getByCode(const QString& code);

    /*
     * Gets all images' names
     *
     * @return QList<QString> - A list of all the images' names
     */
    QList<QString> getNames();

    /*
     * Gets an image's codes given its name
     *
     * @param const QString& code   - Pit type's code
     *
     * @return QString - A image's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all images' codes
     *
     * @return QList<QString> - A list of all the images' codes
     */
    QList<QString> getCodes();

    /*
     * Gets the image's code given its name
     *
     * @param   const QString& name - Image's name
     *
     * @return  QString - Image's code
     */
    QString getCode(const QString& name);

    /*
     * Creates a new image entry
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
     * Updates the image's data
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

    /*
     * Deletes an image from the database
     *
     * @param   const QString& code - Code
     *
     * @return  boolean true on success, false on failure
     */
    bool remove(const QString& code);

private:
    const QString table = "sacred";
    QSqlDatabase db;
    QString name;
    // Temporary id for an image to be inserted
    int new_sacred_temp_id = 9000;
};
