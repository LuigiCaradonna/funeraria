#pragma once

#include <QDebug>
#include <QSignalMapper>
#include <QSqlError>
#include <QSqlQuery>
#include <QInputDialog>
#include "ui_Tomb.h"
#include "ClientUi.h"
#include "Client.h"
#include "Accessory.h"
#include "Sculpture.h"
#include "Tomb.h"
#include "TombType.h"
#include "TombFormat.h"
#include "Pit.h"
#include "Cross.h"
#include "Drawing.h"
#include "Helpers.h"

class TombUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the TombUi object.
     *
     * @param	const QSqlDatabase*	db	            - Refrence to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	QWidget*            parent	        - Parent widget
     */
    TombUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the TombUi object.
     */
    ~TombUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the tomb's progressive property and updates the content of the QDialog according
     *
     * @param   int progressive - Progressive number
     *
     * @return  void
     */
    void setProgressive(const int& progressive);

    /*
     * Says whether the tomb's data should be reloaded or not
     * 
     * @return  bool - True if the data should be reloaded, false otherwise
     */
    bool doReload();

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

    /*
     * Deletes a tomb
     *
     * @return  void
     */
    void slotDelete();

    /*
     * Replaces the tomb's progressive number with the one selected among those not in use
     *
     * @return  void
     */
    void slotNotInUseProgressives();

    /*
     * Saves a tomb's data
     *
     * @return  void
     */
    void slotSave();

    /*
     * Sets the current date to the proper input field
     *
     * @param const QString& - Name of the field where to set the current date
     *
     * @return  void
     */
    void slotSetCurrentDate(const QString& field);

    /*
     * Sets the delivered input field with a generic value (Consegnata)
     *
     * @return  void
     */
    void slotSetDelivered();

    /*
     * Sets the engraved input field with a N/N tag (Not Necessary)
     *
     * @return  void
     */
    void slotSetNoEngraving();

    /*
     * Updates the epigraph quantity line edit state
     *
     * @return  void
     */
    void slotUpdateEpNumState();

    /*
     * Updates the notes field according to the other UI form fields
     *
     * @return  void
     */
    void slotUpdateNotes();

    /*
     * Updates the pit combobox enabled state
     *
     * @return  void
     */
    void slotUpdatePitState();

    /*
     * Updates the sculpture hight line edit enabled state
     *
     * @return  void
     */
    void slotUpdateScHightState();

private:
    Ui::TombClass ui;
    const QString table = "tomb";
    const QString btn_create_text = "Crea";
    const QString btn_update_text = "Aggiorna";
    const QString not_engraved = "No";
    const QString icons_folder;
    QSqlDatabase db;
    QWidget* parent;
    int progressive;
    Sculpture* sculpture;
    Accessory* material;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* frame;
    Tomb* tomb;
    TombType* tomb_type;
    TombFormat* tomb_format;
    Pit* pit;
    Cross* cross;
    Drawing* drawing;

    const QString css_folder;

    QSignalMapper* currentDateMapper;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Checks if the dates inserted into the form are congruent
     *
     * @param const QString& order         - Order date
     * @param const QString& proof         - Proof date
     * @param const QString& confirmation  - Confirmation date
     * @param const QString& engraving     - Engraving date
     * @param const QString& delivery      - Delivery date
     *
     * @return  boolean true on success, false otherwise
     */
    bool checkDates(const QString& order, const QString& proof, const QString& confirmation, const QString& engraving, const QString& delivery);

    /*
     * Resets the dialog interface with the default selections.
     *
     * @return void
     */
    void resetForm();

    /*
     * Updates the dialog interface with the proper data.
     * 
     * @return void
     */
    void updateForm();

    /*
     * Validates the form's data.
     *
     * @param const QString& op - store / update
     *
     * @return bool - True if the validation passes, false if it fails
     */
    bool validateForm(const QString& op);
};
