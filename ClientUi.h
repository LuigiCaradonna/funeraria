#pragma once

#include <QDebug>
#include "Client.h"
#include "ui_Client.h"

class ClientUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Client object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     * @param	QWidget* parent	- Parent widget
     */
    ClientUi(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Client object.
     */
    ~ClientUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the client's name property and updates the content of the QDialog according
     *
     * @param   const QString& name - Client's name
     *
     * @return  void
     */
    void setName(const QString& name);

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Saves the client's data, responds to the dialog window's save button
     *
     * @return  void
     */
    void slotSave();

    /*
     * Updates the client's data, responds to the dialog window's update button
     *
     * @return  void
     */
    void slotUpdate();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
    Ui::ClientClass ui;
    const QString table = "clients";
    QSqlDatabase db;
    QWidget* parent;
    QString name;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Updateds the UI filling the fields with the proper values for the selected client
     *
     * @return  void
     */
    void updateForm();

    /*
     * Checks the data inserted into the form to add/edit a client
     *
     * @return  bool - True if the validation passes, false on failure
     */
    bool checkForm();
};
