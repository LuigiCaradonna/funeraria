#pragma once

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"
#include "ui_Settings.h"

class SettingsUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the SettingsUi object.
     *
     * @param	const QSqlDatabase*	db	            - Refrence to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	QWidget*            parent	        - Parent widget
     */
    SettingsUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the SettingsUi object.
     */
    ~SettingsUi();


protected slots:
    /********** PROTECTED SLOTS **********/

    /*
     * Asks the user to provide a new path for the archive's file
     *
     * @return  void
     */
    void slotChangeArchivePath();

    /*
     * Asks the user to provide a new path for the crosses' file
     *
     * @return  void
     */
    void slotChangeCrossesPath();

    /*
     * Asks the user to provide a new path for the DB's file
     *
     * @return  void
     */
    void slotChangeDbPath();

    /*
     * Asks the user to provide a new path for the drawing images' file
     *
     * @return  void
     */
    void slotChangeDrawingPath();

    /*
     * Asks the user to provide a new path for the sculptures' file
     *
     * @return  void
     */
    void slotChangeSculpturesPath();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

    /*
     * Saves a settings
     *
     * @return  void
     */
    void slotSave();

private:
    Ui::SettingsClass ui;
    QSqlDatabase db;
    QWidget* parent;
    const QString icons_folder;
    const QString css_folder;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Updates the settings according to the data provided into the dialog box.
     * 
     * @param const QMap<QString, QString>& setting - Setting name/value to store
     *
     * @return bool true on success, false on failure
     */
    const bool store(const QMap<QString, QString>& setting);

    /*
     * Updates the dialog interface with the proper data.
     *
     * @return void
     */
    void updateForm();
};

