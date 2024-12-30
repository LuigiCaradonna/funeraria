#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

class Frame : public QObject
{
    Q_OBJECT

public:

    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Frame object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Frame(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Frame object.
     */
    ~Frame();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Stores a new frame
     *
     * @param const QString& code   - Pit type's code
     * @param const QString& name   - Pit type's name
     *
     * @return bool - True on success, false on failure
     */
    bool store(const QString& code, const QString& name);

    /*
     *  Updates a new frame
     *
     * @param const QString& old_code   - Pit type's previous code
     * @param const QString& code   - Pit type's code
     * @param const QString& name   - Pit type's name
     *
     * @return bool - True on success, false on failure
     */
    bool update(const QString& old_code, const QString& code, const QString& name);

    /*
     * Updates a new frame
     *
     * @param const QString& code   - Pit type's code
     *
     * @return bool - True on success, false on failure
     */
    bool remove(const QString& code);

    /*
     * Gets all frames
     *
     * @return QList<QMap<QString, QString>> - A list of all the frames
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all frames' names
     *
     * @return QList<QString> - A list of all the frames' names
     */
    QList<QString> getNames();

    /*
     * Gets a frame's codes given its name
     *
     * @param const QString& code   - Pit type's code
     *
     * @return QString - A frame's code given its name
     */
    QString getName(const QString& code);

    /*
     * Gets all frames' codes
     *
     * @return QList<QString> - A list of all the frames' codes
     */
    QList<QString> getCodes();

    /*
     * Gets a frame's codes given its name
     *
     * @param const QString& name   - Pit type's name
     *
     * @return QString - A frame's code given its name
     */
    QString getCode(const QString& name);

private:
    const QString table = "frame";
    QSqlDatabase db;
    Settings* settings;
};
