#pragma once

#include <QDebug>
#include "Helpers.h"
#include "Accessory.h"
#include "ui_Accessory.h"

class AccessoryUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the accessory object.
     *
     * @param	const QSqlDatabase& db	            - Reference to the database connection
     * @param	const QString&	    table	        - Database table to use
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	QWidget*            parent	        - Parent widget
     */
    AccessoryUi(const QSqlDatabase& db, const QString& table, const QString& css_folder, const QString& icons_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the accessory object.
     */
    ~AccessoryUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Updates the form fields
     *
     * @param const QString& code   - Code
     * @param const QString& name   - Name
     *
     * @return  void
     */
    void updateForm(const QString& code = "", const QString& name = "");

protected slots:

    /********** PRIVATE SLOTS **********/

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

    /*
     * Gets the data from the dialog window and edits the accessory information into the database
     *
     * @return  void
     */
    void slotEdit();

    /*
     * Gets the data from the dialog window and stores a new accessory into the database
     *
     * @return  void
     */
    void slotSave();

private:
    Ui::Dialog ui;
    const QString table;
    QSqlDatabase db;
    QWidget* parent;
    QString old_code = "";
    const QString icons_folder;
    const QString css_folder;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Checks the data inserted into the form to add/edit an accessory
     *
     * @return  bool - True if the validation passes, false on failure
     */
    bool checkForm();

    /*
     * Resets the form's fields
     *
     * @return  void
     */
    void resetFields();

    /*
     * Sets the code field
     *
     * @param const QString& code   - Code
     * 
     * @return  void
     */
    void setCode(QString code);

    /*
     * Sets the name field
     *
     * @param const QString& name   - Name
     * 
     * @return  void
     */
    void setName(QString name);
};

