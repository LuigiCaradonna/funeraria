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
     * Gets the clients' id
     *
     * @param   const QString& name - Client's name
     *
     * @return  int - The clients' id, 0 if not found
     */
    int getId(const QString& name);

    /*
     * Gets all the clients' names
     *
     * @return  QList<QStringList> - A list containing all the clients' names
     */
    QStringList getNames();

    /*
     * Gets all the active clients' names
     *
     * @return  QList<QStringList> - A list containing the clients' names
     */
    QStringList getActiveNames();

    /*
     * Gets all the clients' names to show as buttons into the quick access bar
     *
     * @return  QList<QStringList> - A list containing the clients' names to show as buttons into the quick access bar
     */
    QStringList getQuickAccessNames();

    /*
     * Gets all the clients' data
     *
     * @return  QList<QMap<QString, QString>> - A list containing the clients' data
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets the given client's datails
     *
     * @param   const QString& name - Client's name
     *
     * @return  QMap<QString, QString> - A map containing the client's data
     */
    QMap<QString, QString> getDetails(const QString& name);

    /*
     * Deletes a client from the database
     *
     * @param   const int id - Client's id
     *
     * @return  void
     */
    void remove(const int id);

    /*
     * Creates a new client entry
     *
     * @param const int position        -   Client's ordering position
     * @param const QString& name       -   Client's name
     * @param const QString& emails     -   Client's email(s)
     * @param const QString& address    -   Client's address
     * @param const QString& phones     -   Client's phone number(s)
     * @param const int active          -   Client's active status (0/1)
     * @param const int quick           -   Client's quick access status (0/1)
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
     * @param const int id              -   Client's id
     * @param const int position        -   Client's ordering position
     * @param const QString& name       -   Client's name
     * @param const QString& emails     -   Client's email(s)
     * @param const QString& address    -   Client's address
     * @param const QString& phones     -   Client's phone number(s)
     * @param const int active          -   Client's active status (0/1)
     * @param const int quick           -   Client's quick access status (0/1)
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

    /*
     * Gets the last position in use
     *
     * @return  int - The last position in use
     */
    int getLastPosition();

    /*
     * Given a client's id, name and wanted position, rearranges all the clients' position if necessary
     *
     * @param const int     id          -   Client's id
     * @param const QString& name       -   Client's name
     * @param const int     position    -   Client's ordering position to set. -1 when a client must be deleted
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool rearrangePositions(const int id, const QString& name, const int new_position);

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
     * Shifts down the positions by one
     *
     * @param const int    from            -   Position where to start
     * @param const int    last_position   -   Last position in use
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool shiftPositionsDown(const int from, const int last_position);

    /*
     * Sets the given client's position to a not valid value to allow positions rearranging
     *
     * @param const int    client_id    -   Client's id
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool setInvalidPosition(const int client_id);

private:
    const QString table = "clients";
    QSqlDatabase db;
    QString name;

};
