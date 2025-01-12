#pragma once

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>
#include "ui_TombsAlike.h"
#include "Helpers.h"
#include "Client.h"
#include "Accessory.h"

class TombsAlikeUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the TombsAlikeUi object.
     *
     * @param	const QSqlDatabase*	db	            - Refrence to the database connection
     * @param	const QString&	    css_folder	    - Path to the css folder
     * @param	const QString&	    icons_folder	- Path to the icons folder
     * @param	QWidget*            parent	        - Parent widget
     */
    TombsAlikeUi(const QSqlDatabase& db, const QString& css_folder, const QString& icons_folder, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the TombsAlikeUi object.
     */
    ~TombsAlikeUi();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets the client field value
     *
     * @return  QString client field value
     */
    QString getClient();

    /*
     * Gets the cross' checkbox state value
     *
     * @return  bool cross' checkbox state value
     */
    bool getCross();

    /*
     * Gets the drawing's checkbox state value
     *
     * @return  bool drawing's checkbox state value
     */
    bool getDrawing();

    /*
     * Gets the inscription's checkbox state value
     *
     * @return  bool inscription's checkbox state value
     */
    bool getInscription();

    /*
     * Gets the epigraphs amount field value
     *
     * @return  int epigraphs amount field value
     */
    int getEpAmount();

    /*
     * Gets the material field value
     *
     * @return  QString material field value
     */
    QString getMaterial();

    /*
     * Gets the mounted checkbox state value
     *
     * @return  bool mounted checkbox state value
     */
    bool getMount();

    /*
     * Gets the pits amount field value
     *
     * @return  int pits amount field value
     */
    int getPitsAmount();

    /*
     * Gets the material provided checkbox state value
     *
     * @return  bool material provided checkbox state value
     */
    bool getProvided();

    /*
     * Gets the epigraph relief's checkbox state value
     *
     * @return  bool epigraph relief's checkbox state value
     */
    bool getRelief();

    /*
     * Gets the sculpture's checkbox state value
     *
     * @return  bool sculpture's checkbox state value
     */
    bool getSculpture();

    /*
     * Resets the form to the default selections
     *
     * @return  void
     */
    void reset();

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

    /*
     * Validates the form
     *
     * @return  void
     */
    void slotValidateForm();

private:
    Ui::TombsAlikeClass ui;
    QSqlDatabase db;
    QWidget* parent;
    const QString css_folder;
    const QString icons_folder;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Initializes the forms' data
     *
     * @return  void
     */
    void init();
};
