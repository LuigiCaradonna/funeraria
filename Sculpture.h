#pragma once

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QMessageBox>

class Sculpture : public QObject
{
	Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Sculpture object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Sculpture(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Sculpture object.
     */
    ~Sculpture();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets the given sculpture's datails
     *
     * @param   const QString& code - Code
     *
     * @return  QMap<QString, QString> - A map containing the sculpture's data
     */
    QMap<QString, QString> getByCode(const QString& code);

    /*
     * Gets the sculpture's code given its name
     *
     * @param   const QString& name - Sculpture's name
     *
     * @return  QString - Sculpture's code
     */
    QString getCode(const QString& name);

    /*
     * Gets all sculptures' codes
     *
     * @return QList<QString> - A list of all the sculptures' codes
     */
    QList<QString> getCodes();

    /*
     * Gets all the sculptures' data
     *
     * @param   const QString& code - Optional, code of the sculpture to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the sculptures' data
     */
    QList<QMap<QString, QString>> getListByCode(const QString& code = "");

    /*
     * Gets all the sculptures' data
     *
     * @param   const QString& code - Optional, name of the sculpture to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the sculptures' data
     */
    QList<QMap<QString, QString>> getListByName(const QString& name = "");

    /*
     * Gets a sculpture's codes given its name
     *
     * @param const QString& code   - Pit type's code
     *
     * @return QString - A sculpture's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all sculptures' names
     *
     * @return QList<QString> - A list of all the sculptures' names
     */
    QList<QString> getNames();

    /*
     * Deletes a sculpture from the database
     *
     * @param   const QString& code - Code
     *
     * @return  boolean true on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Creates a new sculpture entry
     *
     * @param const QString& code   -   Code
     * @param const QString& name   -   Name
     * @param const QString& img    -   Image file name
     * @param const QString& width  -   Width in mm
     * @param const QString& height -   Height in mm
     * @param const QString& depth  -   Depth in mm
     *
     * @return  boolean true if the creation succeeds, false otherwise
     */
    bool store(
        const QString& code,
        const QString& name,
        const QString& img,
        const QString& width,
        const QString& height,
        const QString& depth
    );

    /*
     * Updates the sculpture's data
     *
     * @param const QString& old_code   -   Old code
     * @param const QString& code       -   Code
     * @param const QString& name       -   Name
     * @param const QString& img        -   Image file name
     * @param const QString& width      -   Width in mm
     * @param const QString& height     -   Height in mm
     * @param const QString& depth      -   Depth in mm
     *
     * @return  boolean true if the update succeeds, false otherwise
     */
    bool update(
        const QString& old_code,
        const QString& code,
        const QString& name,
        const QString& img,
        const QString& width,
        const QString& height,
        const QString& depth
    );

private:
    const QString table = "sculpture";
    QSqlDatabase db;
    QString name;
    // Temporary id for a sculpture to be inserted
    int new_sculpture_temp_id = 9000;
};
