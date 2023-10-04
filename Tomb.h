#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include "Helpers.h"

class Tomb : public QObject
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Tomb object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    Tomb(const QSqlDatabase& db);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Tomb object.
     */
    ~Tomb();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the Tombs
     * 
     * @param const int& client_id      - Client's id
     * @param const int& year           - Year of the tombs to get, 0 for all the years
     * @param const QString& filter     - Deceased name to use to refine the result
     *
     * @return  QList<QMap<QString, QString>> - A list containing the Tombs' details
     */
    QList<QMap<QString, QString>> getList(const int& client_id, const int& year, const QString& filter);

    /*
     * Gets the given tomb's datails joined to the related tables
     *
     * @param   int - Tomb's progressive number
     *
     * @return  QMap<QString, QString> - A map containing the tomb's  datails joined to the related tables
     */
    QMap<QString, QString> getDetails(const int& progressive);

    /*
     * Gets the list of the tombs which can be engraved
     *
     * @return  QList<QMap<QString, QString>>   the list of the tombs which can be engraved
     */
    QList<QMap<QString, QString>> tombsToEngrave();

    /*
     * Gets the accessories needed by the tombs not yet mounted
     *
     * @return  QList<QMap<QString, QString>>   a list of the accessories' name, type and quantity
     */
    QList<QMap<QString, QString>> accessoriesToMount();

    /*
     * Gets the list of the tombs not yet paid
     *
     * @return  QList<QMap<QString, QString>>   the list of the tombs not yet paid
     */
    QList<QMap<QString, QString>> tombsToPay();

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
     * Updates a tomb into the database
     *
     * @param const int& progressive            - Tomb's progressive number
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
    bool store(
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
     * @param   const int& progressive - Tomb's progressive number
     *
     * @return  void
     */
    void remove(const int& progressive);

    /*
     * Gets the last progressive number in use
     *
     * @return  int - The last progressive number in use
     */
    int getLastProgresive();

    /*
     * Checks if the given progressive number is in use
     *
     * @return  boolean true if the progressive number in use or if the query fails, false otherwise
     */
    bool isProgressiveInUse(const int& progressive);

private:
    const QString table = "tombs";
    QSqlDatabase db;

};
