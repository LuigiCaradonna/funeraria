#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include <QSignalMapper>
#include "ui_Tomb.h"
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
     * Constructs the Tomb object.
     *
     * @param	QSqlDatabase*	db	- Pointer to the database connection
     * @param	QWidget*        parent	- Parent widget
     */
    TombUi(QSqlDatabase* db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Tomb object.
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
    const QString btnCreateText = "Crea";
    const QString btnUpdateText = "Aggiorna";
    QSqlDatabase* db;
    QWidget* parent;
    int progressive;
    Client* client;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* material;

    QSignalMapper* currentDateMapper;

    /********** PRIVATE FUNCTIONS **********/

    void updateForm();
};
