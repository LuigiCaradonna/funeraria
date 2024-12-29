#pragma once

#include <QDebug>
#include "Helpers.h"
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
     * @param	const QSqlDatabase*	db	            - Reference to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	QWidget*            parent	        - Parent widget
     */
    ClientUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Client object.
     */
    ~ClientUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the client's id property and updates the content of the QDialog according
     *
     * @param   int id - Id
     *
     * @return  void
     */
    void setId(int id);

    /*
     * Sets the client's name property and updates the content of the QDialog according
     *
     * @param   const QString& name - Name
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
    const QString table = "client";
    QSqlDatabase db;
    QWidget* parent;
    QString name;
    int id;
    const QString icons_folder;
    const QString css_folder;

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
