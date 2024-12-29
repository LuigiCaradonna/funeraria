#pragma once

#include <QDebug>
#include <QFileDialog>
#include "Sculpture.h"
#include "ui_Sculpture.h"
#include "Config.h"
#include "Helpers.h"

class SculptureUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the SculptureUi object.
     *
     * @param	const QSqlDatabase*	db	            - Reference to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	const QString&	    images_folder	- Path to the images folder
     * @param	QWidget*            parent	        - Parent widget
     */
    SculptureUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, const QString& images_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Sculpture object.
     */
    ~SculptureUi();

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

    /*
     * Computes the reduction coefficient to obtain the target width
     *
     * @return  void
     */
    void slotReductionWXY();

    /*
     * Computes the reduction coefficient to obtain the target height
     *
     * @return  void
     */
    void slotReductionHXY();

    /*
     * Computes the reduction coefficient to obtain the target depth
     *
     * @return  void
     */
    void slotReductionZ();

private:
    Ui::SculptureClass ui;
    const QString table = "sculptures";
    QSqlDatabase db;
    QWidget* parent;
    QString code;
    int default_height = 200;
    int default_depth = 11;
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

