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
#include "Helpers.h"

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
     * Updates a tomb into the database
     *
     * @param const int& old_progressive        - Tomb's original progressive number
     * @param const int& progressive            - Tomb's new progressive number
     * @param const int& client_id              - Client's id
     * @param const QString& name               - Deceased's name
     * @param const QString& additional_names   - Additional names
     * @param const double& price               - Price
     * @param const bool& paid                  - Paid or not
     * @param const QString& material_code      - Material's code
     * @param const QString& vase_code          - Vase's code
     * @param const QString& lamp_code          - Lamp's code
     * @param const QString& flame_code         - Flame's code
     * @param const QString& notes              - Notes
     * @param const bool& accessories_mounted   - Accessories mounted or not
     * @param const QString& ordered_at         - Order date
     * @param const QString& proofed_at         - Proof date
     * @param const QString& confirmed_at       - Confirmation date
     * @param const QString& engraved_at        - Engraving date
     * @param const QString& delivered_at       - Delivery date
     *
     * @return  boolean true on success, false otherwise
     */
    bool update(
        const int& old_progressive,
        const int& progressive,
        const int& client_id,
        const QString& name,
        const QString& additional_names,
        const double& price,
        const bool& paid,
        const QString& material_code,
        const QString& vase_code,
        const QString& lamp_code,
        const QString& flame_code,
        const QString& notes,
        const bool& accessories_mounted,
        const QString& ordered_at,
        const QString& proofed_at,
        const QString& confirmed_at,
        const QString& engraved_at,
        const QString& delivered_at
    );

    /*
     * Deletes a tomb from the database
     *
     * @param   int progressive - Tomb's progressive number
     *
     * @return  void
     */
    void remove(int progressive);

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
