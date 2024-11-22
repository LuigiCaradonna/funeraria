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
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     * @param	QWidget* parent	- Parent widget
     */
    SculptureUi(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Sculpture object.
     */
    ~SculptureUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the sculpture's id property and updates the content of the QDialog according
     *
     * @param   int id - Sculpture's id
     *
     * @return  void
     */
    void setId(int id);

    /*
     * Sets the sculpture's code property and updates the content of the QDialog according
     *
     * @param   const QString& code - Sculpture's code
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
    Ui::SculptureClass ui;
    const QString table = "sculptures";
    QSqlDatabase db;
    QWidget* parent;
    QString code;
    int id;
    int default_height = 200;
    int default_depth = 11;

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

