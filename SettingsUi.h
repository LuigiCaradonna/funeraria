#pragma once
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include "Settings.h"
#include "ui_Settings.h"

class SettingsUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the SettingsUi object.
     *
     * @param	const QSqlDatabase*	db	    - Refrence to the database connection
     * @param	QWidget*            parent	- Parent widget
     */
    SettingsUi(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the SettingsUi object.
     */
    ~SettingsUi();


protected slots:
    /********** PROTECTED SLOTS **********/

    /*
     * Saves a settings
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
    Ui::SettingsClass ui;
    QSqlDatabase db;
    QWidget* parent;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Updates the dialog interface with the proper data.
     *
     * @return void
     */
    void updateForm();
};

