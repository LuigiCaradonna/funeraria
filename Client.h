#pragma once

#include <QDialog>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QRegularExpression>
#include <QSqlTableModel>
#include <QMessageBox>
#include "ui_Client.h"

class Client : public QDialog
{
	Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Client object.
     *
     * @param	QSqlDatabase*	db	- Pointer to the database connection
     */
    Client(QSqlDatabase* db, QWidget* parent = nullptr);

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
     * @return  QList<QStringList> - A list containing the clients' names
     */
    QStringList getNames();

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
     * Sets the client's name property and updates the content of the QDialog according
     *
     * @param   const QString& name - Client's name
     *
     * @return  void
     */
    void setName(const QString& name);

    /*
     * Deletes a client from the database
     *
     * @param   const int& id - Client's id
     *
     * @return  void
     */
    void remove(const int& id);

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Saves the client's data, responds to the dialog window's save button
     *
     * @return  void
     */
    void slotSave();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
	Ui::ClientClass ui;
    const QString table = "clients";
    QSqlDatabase* db;
    QWidget* parent;
    QString name;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Creates a new client entry
     * 
     * @param const int& position       -   Client's ordering position
     * @param const QString& name       -   Client's name
     * @param const QString& emails     -   Client's email(s)
     * @param const QString& address    -   Client's address
     * @param const QString& phones     -   Client's phone number(s)
     * @param const int& active         -   Client's active status (0/1)
     *
     * @return  boolean true if the creation succeeds, false otherwise
     */
    bool create(
        const int& position,
        const QString& name,
        const QString& email,
        const QString& address,
        const QString& phone,
        const int& active
    );

    /*
     * Updates the client's data
     *
     * @param const int& id             -   Client's id
     * @param const int& position       -   Client's ordering position
     * @param const QString& name       -   Client's name
     * @param const QString& emails     -   Client's email(s)
     * @param const QString& address    -   Client's address
     * @param const QString& phones     -   Client's phone number(s)
     * @param const int& active         -   Client's active status (0/1)
     *
     * @return  boolean true if the update succeeds, false otherwise
     */
    bool update(
        const int& id,
        const int& position,
        const QString& name,
        const QString& email,
        const QString& address,
        const QString& phone,
        const int& active
    );

    /*
     * Updates the content of the QDialog according to the current client selected
     *
     * @return  void
     */
    void updateForm();

    /*
     * Gets the last position in use
     *
     * @return  int - The last position in use
     */
    int getLastPosition();

    /*
     * Given a client's id, name and wanted position, rearranges all the clients' position if necessary
     *
     * @param const int&     id         -   Client's id
     * @param const QString& name       -   Client's name
     * @param const int&     position   -   Client's ordering position to set. -1 when a client must be deleted
     * 
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool rearrangePositions(const int& id, const QString& name, const int& new_position);

    /*
     * Shifts up the positions by one 
     *
     * @param const int&    from            -   Position where to start
     * @param const int&    last_position   -   Last position in use
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool shiftPositionsUp(const int& from, const int& last_position);

    /*
     * Shifts down the positions by one
     *
     * @param const int&    from            -   Position where to start
     * @param const int&    last_position   -   Last position in use
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool shiftPositionsDown(const int& from, const int& last_position);

    /*
     * Sets the given client's position to a not valid value to allow positions rearranging
     *
     * @param const int&    client_id    -   Client's id
     *
     * @return  boolean true if the operation succeeds, false otherwise
     */
    bool setInvalidPosition(const int& client_id);
};
