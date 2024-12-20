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
     * Gets all the sculptures' data
     *
     * @param   const QString& code - Optional, code or name of the sculputre to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the sculptures' data
     */
    QList<QMap<QString, QString>> get(const QString& code = "");

    /*
     * Gets the given sculpture's datails
     *
     * @param   const int id - Id
     *
     * @return  QMap<QString, QString> - A map containing the sculpture's data
     */
    QMap<QString, QString> getDetailsById(const int id);

    /*
     * Gets the given sculpture's datails
     *
     * @param   const QString& code - Code
     *
     * @return  QMap<QString, QString> - A map containing the sculpture's data
     */
    QMap<QString, QString> getDetailsByCode(const QString& code);

    /*
     * Deletes a sculpture from the database
     *
     * @param   const int id - Id
     *
     * @return  boolean true on success, false on failure
     */
    bool remove(const int id);

    /*
     * Creates a new sculpture entry
     *
     * @param const QString& code   -   Code
     * @param const QString& img    -   Image file name
     * @param const QString& width  -   Width in mm
     * @param const QString& height -   Height in mm
     * @param const QString& depth  -   Depth in mm
     *
     * @return  boolean true if the creation succeeds, false otherwise
     */
    bool store(
        const QString& code,
        const QString& img,
        const QString& width,
        const QString& height,
        const QString& depth
    );

    /*
     * Updates the sculpture's data
     *
     * @param const int id          -   Id
     * @param const QString& code   -   Code
     * @param const QString& img    -   Image file name
     * @param const QString& width  -   Width in mm
     * @param const QString& height -   Height in mm
     * @param const QString& depth  -   Depth in mm
     *
     * @return  boolean true if the update succeeds, false otherwise
     */
    bool update(
        const int id,
        const QString& code,
        const QString& img,
        const QString& width,
        const QString& height,
        const QString& depth
    );

private:
    const QString table = "sculptures";
    QSqlDatabase db;
    QString name;
    // Temporary id for a sculpture to be inserted
    int new_sculpture_temp_id = 9000;
};
