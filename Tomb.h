#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "Settings.h"
#include "Helpers.h"
#include "Config.h"

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
     * Gets all the tombs matching the given parameters
     * 
     * @param const int client_id       - Client's id
     * @param const int year            - Year of the tombs to get, 0 for all the years
     * @param bool engraved             - True to get only the tobs that required to be engraved, false for all the tombs
     * @param QString name              - Deceased name or partial name to use to refine the result
     * @param QString sort_by           - Column to sort
     * @param QString sort_direction    - Sort direction
     *
     * @return  QList<QMap<QString, QString>> - A list containing the Tombs' details
     */
    QList<QMap<QString, QString>> getList(
        const int client_id, 
        const int year, 
        bool engraved = false, 
        QString name = "",
        QString sort_by = "progressive",
        QString sort_direction = "ASC"
    );

    /*
     * Gets all the tombs matching the given parameters and formats the report
     *
     * @param const int client_id       - Client's id
     * @param const int year            - Year of the tombs to get, 0 for all the years
     * @param bool engraved             - True to get only the tombs that required to be engraved, false for all the tombs
     * @param QString group             - How to group the result: by year, month or none
     * @param bool by_client            - True to get the tombs count separately by client
     *
     * @return  QList<QMap<QString, QString>> - A list containing the Tombs' report
     */
    QList<QMap<QString, QString>> getReport(
        const int client_id,
        const int year,
        bool engraved = false,
        QString group = "",
        bool by_client = false
    );

    /*
     * Gets all the tombs in the archive giving the sum of them sort by year
     *
     * @return  QList<QMap<QString, QString>> - A list containing the general trend info
     */
    QList<QMap<QString, QString>> getGeneralTrend();

    /*
     * Gets the tombs matching the given progressive number
     *
     * @param const int progressive     - Progressive number
     *
     * @return  QMap<QString, QString>  - The Tomb's details
     */
    QMap<QString, QString> getByProgressive(const int progressive);

    /*
     * Gets the given tomb's datails joined to the related tables
     *
     * @param   int - Progressive number
     *
     * @return  QMap<QString, QString> - A map containing the tomb's  datails joined to the related tables
     */
    QMap<QString, QString> getDetails(const int progressive);

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
     * @param const int progressive             - Tomb's progressive number
     * @param const int client_id               - Client's id
     * @param const QString& name               - Deceased's name
     * @param const QString& additional_names   - Additional names
     * @param const bool engraved               - Whether the tomb must be engraved or only the material was requested
     * @param const double& price               - Price
     * @param const bool paid                   - Paid or not
     * @param const QString& material_code      - Material's code
     * @param const QString& vase_code          - Vase's code
     * @param const QString& lamp_code          - Lamp's code
     * @param const QString& flame_code         - Flame's code
     * @param const QString& notes              - Notes
     * @param const bool accessories_mounted    - Accessories mounted or not
     * @param const QString& ordered_at         - Order date
     * @param const QString& proofed_at         - Proof date
     * @param const QString& confirmed_at       - Confirmation date
     * @param const QString& engraved_at        - Engraving date
     * @param const QString& delivered_at       - Delivery date
     *
     * @return  boolean true on success, false otherwise
     */
    bool store(
        const int progressive,
        const int client_id,
        const QString& name,
        const QString& additional_names,
        const bool engraved,
        const double& price,
        const bool paid,
        const QString& material_code,
        const QString& vase_code,
        const QString& lamp_code,
        const QString& flame_code,
        const QString& notes,
        const bool accessories_mounted,
        const QString& ordered_at,
        const QString& proofed_at,
        const QString& confirmed_at,
        const QString& engraved_at,
        const QString& delivered_at
    );

    /*
     * Updates a tomb into the database
     *
     * @param const int old_progressive         - Tomb's original progressive number
     * @param const int progressive             - Tomb's new progressive number
     * @param const int client_id               - Client's id
     * @param const QString& name               - Deceased's name
     * @param const QString& additional_names   - Additional names
     * @param const bool engraved               - Whether the tomb must be engraved or only the material was requested
     * @param const double& price               - Price
     * @param const bool paid                   - Paid or not
     * @param const QString& material_code      - Material's code
     * @param const QString& vase_code          - Vase's code
     * @param const QString& lamp_code          - Lamp's code
     * @param const QString& flame_code         - Flame's code
     * @param const QString& notes              - Notes
     * @param const bool accessories_mounted    - Accessories mounted or not
     * @param const QString& ordered_at         - Order date
     * @param const QString& proofed_at         - Proof date
     * @param const QString& confirmed_at       - Confirmation date
     * @param const QString& engraved_at        - Engraving date
     * @param const QString& delivered_at       - Delivery date
     *
     * @return  boolean true on success, false otherwise
     */
    bool update(
        const int old_progressive,
        const int progressive,
        const int client_id,
        const QString& name,
        const QString& additional_names,
        const bool engraved,
        const double& price,
        const bool paid,
        const QString& material_code,
        const QString& vase_code,
        const QString& lamp_code,
        const QString& flame_code,
        const QString& notes,
        const bool accessories_mounted,
        const QString& ordered_at,
        const QString& proofed_at,
        const QString& confirmed_at,
        const QString& engraved_at,
        const QString& delivered_at
    );

    /*
     * Deletes a tomb from the database
     *
     * @param   const int progressive - Progressive number
     *
     * @return  void
     */
    void remove(const int progressive);

    /*
     * Sets a tomb as confirmed
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setConfirmed(const int progressive);

    /*
     * Sets a tomb as engraved
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setEngraved(const int progressive);

    /*
     * Sets a tomb as delivered
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setDelivered(const int progressive);

    /*
     * Sets a tomb as paid
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setPaid(const int progressive);

    /*
     * Sets a tomb's accessories as mounted
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setAccessoriesMounted(const int progressive);

    /*
     * Gets the last progressive number in use
     *
     * @return  int - The last progressive number in use
     */
    int getLastProgresive();

    /*
     * Gets the last progressive number in use
     *
     * @return  QList<int> - The list of progressive numbers not in use
     */
    QList<int> getNotInUseProgressives();

    /*
     * Checks if the given progressive number is in use
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the progressive number in use or if the query fails, false otherwise
     */
    bool isProgressiveInUse(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been confirmed
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been confirmed, false otherwise
     */
    bool isConfirmed(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been engraved
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been engraved, false otherwise
     */
    bool isEngraved(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been delivered
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been delivered, false otherwise
     */
    bool isDelivered(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been paied
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been paied, false otherwise
     */
    bool isPaied(const int progressive);

    /*
     * Given the progressive number of the tomb, calculates its range and then the folder where it belongs.
     * E.G.: progressive 2140, it belongs to 2100-2199
     *
     * @param   const int - Progressive number of the tomb
     *
     * @return  QString - The folder where the tomb belongs.
     */
    QString getGroupingFolder(const int progressive);

    /*
     * Provides the full path to the folder of a given tomb
     *
     * @param   const int       - Progressive number of the tomb
     * @param   const QString&  - Deceased's name
     *
     * @return  QString - The full path to the tomb's folder.
     */
    QString getFolderPath(const int progressive, const QString& name);

private:
    const QString table = "tombs";
    QSqlDatabase db;
    Settings* settings;

    const QString not_engraved = "No";
};
