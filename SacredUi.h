#pragma once

#include <QDebug>
#include <QFileDialog>
#include "Sacred.h"
#include "ui_Sacred.h"
#include "Config.h"
#include "Helpers.h"

class SacredUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the SacredUi object.
     *
     * @param	const QSqlDatabase*	db	            - Reference to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	const QString&	    images_folder	- Path to the images folder
     * @param	QWidget*            parent	        - Parent widget
     */
    SacredUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, const QString& images_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the SacredUi object.
     */
    ~SacredUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the sculpture's code property and updates the content of the QDialog according
     *
     * @param   const QString& code - Code
     *
     * @return  void
     */
    void setCode(const QString& code);

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Sets the selected sculpture's image path into the corresponding form field
     *
     * @return  void
     */
    void slotSelectImage();

    /*
     * Changes the enabled state for the generally disabled fields
     *
     * @return  void
     */
    void slotSwitchEnableState();

    /*
     * Saves the sculpture's data, responds to the dialog window's save button
     *
     * @return  void
     */
    void slotSave();

    /*
     * Updates the sculpture's data, responds to the dialog window's update button
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
    Ui::SacredClass ui;
    const QString table = "sacred";
    QSqlDatabase db;
    QWidget* parent;
    QString code;
    const QString icons_folder;
    const QString images_folder;
    const QString css_folder;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Updateds the UI filling the fields with the proper values for the selected sculpture
     *
     * @return  void
     */
    void updateForm();

    /*
     * Checks the data inserted into the form to add/edit a sculpture
     *
     * @return  bool - True if the validation passes, false on failure
     */
    bool checkForm();
};

