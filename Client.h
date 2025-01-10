#pragma once

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QRegularExpression>
#include <QMessageBox>

class Client : public QObject
{
	Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Client object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Client(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Client object.
     */
    ~Client();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the clients' data
     *
     * @param   const QString& name - Optional, client's name to get
     *
     * @return  QList<QMap<QString, QString>> - A list containing the clients' data
     */
    QList<QMap<QString, QString>> get(const QString& name = "");

    /*
     * Gets all the active clients
     *
     * @return  QList<QMap<QString, QString>> - A list containing the active clients
     */
    QList<QMap<QString, QString>> getActive();

    /*
     * Gets all the active clients' names
     *
     * @return  QQStringList - A list containing the Active clients' names
     */
    QStringList getActiveNames();

    /*
     * Gets the given client's datails
     *
     * @param   int id - Id
     *
     * @return  QMap<QString, QString> - A map containing the client's data
     */
    QMap<QString, QString> getDetailsById(int id);

    /*
     * Gets the given client's datails
     *
     * @param   const QString& name - Name
     *
     * @return  QMap<QString, QString> - A map containing the client's data
     */
    QMap<QString, QString> getDetailsByName(const QString& name);

    /*
     * Gets the clients' id
     *
     * @param   const QString& name - Name
     *
     * @return  int - The clients' id, 0 if not found, also meaning "all"
     */
    int getId(const QString& name);

    /*
     * Gets the last position in use
     *
     * @return  int - The last position in use
     */
    int getLastPosition();

    /*
     * Gets the client's name
     *
     * @param   const int - Id
     *
     * @return  QString - The clients' id, 0 if not found, also meaning "all"
     */
    QString getName(const int id);

    /*
     * Gets all the clients' names
     *
     * @return  QStringList - A list containing all the clients' names
     */
    QStringList getNames();

    /*
     * Gets all the clients' names to show as buttons into the quick access bar
     *
     * @return  QStringList - A list containing the clients' names to show as buttons into the quick access bar
     */
    QStringList getQuickAccessNames();

    /*
     * Given a client's id, name and wanted position, rearranges all the clients' position if necessary
     *
     * @param const int     id          -   Id
     * @param const QString& name       -   Name
     * @param const int     position    -   Ordering position to set. -1 when a client must be deleted
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool rearrangePositions(const int id, const QString& name, const int new_position);

    /*
     * Deletes a client from the database
     *
     * @param   const int id - Id
     *
     * @return  boolean true on success, false on failure
     */
    bool remove(const int id);

    /*
     * Sets the given client's position to a not valid value to allow positions rearranging
     *
     * @param const int    client_id    -   Id
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool setInvalidPosition(const int client_id);

    /*
     * Shifts down the positions by one
     *
     * @param const int    from            -   Position where to start
     * @param const int    last_position   -   Last position in use
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool shiftPositionsDown(const int from, const int last_position);

    /*
     * Shifts up the positions by one
     *
     * @param const int    from            -   Position where to start
     * @param const int    last_position   -   Last position in use
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool shiftPositionsUp(const int from, const int last_position);

    /*
     * Creates a new client entry
     *
     * @param const int position        -   Ordering position
     * @param const QString& name       -   Name
     * @param const QString& emails     -   Eemail(s)
     * @param const QString& address    -   Address
     * @param const QString& phones     -   Phone number(s)
     * @param const int active          -   Active status (0/1)
     * @param const int quick           -   Quick access status (0/1)
     *
     * @return  boolean true if the creation succeeds, false otherwise
     */
    bool store(
        const int position,
        const QString& name,
        const QString& email,
        const QString& address,
        const QString& phone,
        const int active,
        const int quick
    );

    /*
     * Updates the client's data
     *
     * @param const int id              -   Id
     * @param const int position        -   Ordering position
     * @param const QString& name       -   Name
     * @param const QString& emails     -   Eemail(s)
     * @param const QString& address    -   Address
     * @param const QString& phones     -   Phone number(s)
     * @param const int active          -   Active status (0/1)
     * @param const int quick           -   Quick access status (0/1)
     *
     * @return  boolean true if the update succeeds, false otherwise
     */
    bool update(
        const int id,
        const int position,
        const QString& name,
        const QString& email,
        const QString& address,
        const QString& phone,
        const int active,
        const int quick
    );

private:
    const QString table = "client";
    QSqlDatabase db;
    QString name;
    // Temporary id for a client to be inserted
    int new_client_temp_id = 2000;
};
