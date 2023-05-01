#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include "ui_Tomb.h"
#include "Client.h"
#include "Accessory.h"

class Tomb : public QDialog
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
    Tomb(QSqlDatabase* db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Tomb object.
     */
    ~Tomb();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the Tombs
     * 
     * @param int client_id             - Client's id
     * @param int year                  - Year of the tombs to get, 0 for all the years
     * @param const QString& filter     - Deceased name to use to refine the result
     *
     * @return  QList<QMap<QString, QString>> - A list containing the Tombs' id and name
     */
    QList<QMap<QString, QString>> getList(int client_id, int year, const QString& filter);

    /*
     * Gets the given tomb's datails joined to the related tables
     *
     * @param   int - Tomb's progressive number
     *
     * @return  QMap<QString, QString> - A map containing the tomb's  datails joined to the related tables
     */
    QMap<QString, QString> getDetails(int progressive);

    /*
     * Sets the tomb's progressive property and updates the content of the QDialog according
     *
     * @param   int progressive - Tomb's progressive number
     *
     * @return  void
     */
    void setProgressive(int progressive);

    /*
     * Deletes a tomb from the database
     *
     * @param   const int& progressive - Tomb's progressive number
     *
     * @return  void
     */
    void remove(const int& progressive);

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Changes the enabled state for the generally disabled fields
     *
     * @return  void
     */
    void slotSwitchEnableState();

    /*
     * Updates a tomb's data
     *
     * @param   int - Tomb's progressive number
     *
     * @return  boolean true on success, false otherwise
     */
    bool slotUpdate(int progressive);

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
    Ui::TombClass ui;
    const QString table = "tombs";
    QSqlDatabase* db;
    QWidget* parent;
    int progressive;
    Client* client;
    Accessory* vase;
    Accessory* lamp;
    Accessory* flame;
    Accessory* material;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Gets the last progressive number in use
     *
     * @return  int - The last progressive number in use
     */
    int getLastProgresive();

    /*
     * Updates the content of the dialog according to the current tomb selected
     *
     * @return  void
     */
    void updateForm();
};
