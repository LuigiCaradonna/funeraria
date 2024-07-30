#pragma once
#include <QDebug>
#include <QSignalMapper>
#include <QSqlError>
#include <QSqlQuery>
#include "ui_Tomb.h"
#include "ClientUi.h"
#include "Client.h"
#include "Accessory.h"
#include "Tomb.h"
#include "Helpers.h"

class TombUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the TombUi object.
     *
     * @param	const QSqlDatabase*	db	    - Refrence to the database connection
     * @param	QWidget*            parent	- Parent widget
     */
    TombUi(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the TombUi object.
     */
    ~TombUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Sets the tomb's progressive property and updates the content of the QDialog according
     *
     * @param   int progressive - Tomb's progressive number
     *
     * @return  void
     */
    void setProgressive(const int& progressive);

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Changes the enabled state for the generally disabled fields
     *
     * @return  void
     */
    void slotSwitchEnableState();

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
     * Saves a tomb's data
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
    Ui::TombClass ui;
    const QString table = "tombs";
    const QString btn_create_text = "Crea";
    const QString btn_update_text = "Aggiorna";
    const QString not_engraved = "No";
    QSqlDatabase db;
    QWidget* parent;
    int progressive;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* material;

    QSignalMapper* currentDateMapper;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Updates the dialog interface with the proper data.
     * 
     * @return void
     */
    void updateForm();
};
