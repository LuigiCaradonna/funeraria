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
     * @param	const QSqlDatabase&	db	- Reference to the database connection
     */
    Client(const QSqlDatabase& db, QWidget* parent = nullptr);

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
     * @return  int - The clients' id
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
    const QSqlDatabase& db;
    QWidget* parent;
    QString name;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Creates a new client entry
     * 
     * @param const QString& position   -   Client's ordering position
     * @param const QString& name       -   Client's name
     * @param const QString& emails     -   Client's email(s)
     * @param const QString& address    -   Client's address
     * @param const QString& phones     -   Client's phone number(s)
     * @param const QString& active     -   Client's active status (0/1)
     *
     * @return  void
     */
    void create(
        const QString& position,
        const QString& name,
        const QString& email,
        const QString& address,
        const QString& phone,
        const QString& active
    );

    /*
     * Updates the client's data
     *
     * @param const QString& id         -   Client's id
     * @param const QString& position   -   Client's ordering position
     * @param const QString& name       -   Client's name
     * @param const QString& emails     -   Client's email(s)
     * @param const QString& address    -   Client's address
     * @param const QString& phones     -   Client's phone number(s)
     * @param const QString& active     -   Client's active status (0/1)
     *
     * @return  void
     */
    void update(
        const QString& id,
        const QString& position,
        const QString& name,
        const QString& email,
        const QString& address,
        const QString& phone,
        const QString& active
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
};
