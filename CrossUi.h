#pragma once

#include <QDebug>
#include <QFileDialog>
#include "Cross.h"
#include "ui_Cross.h"
#include "Config.h"
#include "Helpers.h"

class CrossUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the CrossUi object.
     *
     * @param	const QSqlDatabase*	db	            - Reference to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	const QString&	    images_folder	- Path to the images folder
     * @param	QWidget*            parent	        - Parent widget
     */
    CrossUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, const QString& images_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the CrossUi object.
     */
    ~CrossUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the cross's code property and updates the content of the QDialog according
     *
     * @param   const QString& code - Code
     *
     * @return  void
     */
    void setCode(const QString& code);

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

    /*
     * Saves the cross's data, responds to the dialog window's save button
     *
     * @return  void
     */
    void slotSave();

    /*
     * Sets the selected cross's image path into the corresponding form field
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
     * Updates the cross's data, responds to the dialog window's update button
     *
     * @return  void
     */
    void slotUpdate();

private:
    Ui::CrossClass ui;
    const QString table = "cross";
    QSqlDatabase db;
    QWidget* parent;
    QString code;
    const QString icons_folder;
    const QString images_folder;
    const QString css_folder;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Checks the data inserted into the form to add/edit a cross
     *
     * @return  bool - True if the validation passes, false on failure
     */
    bool checkForm();

    /*
     * Updateds the UI filling the fields with the proper values for the selected cross
     *
     * @return  void
     */
    void updateForm();
};

