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
     * Gets the accessories needed by the tombs not yet mounted
     *
     * @return  QList<QMap<QString, QString>>   a list of the accessories' name, type and quantity
     */
    QList<QMap<QString, QString>> accessoriesToMount();

    /*
     * Gets all the tombs matching the given parameters
     *
     * @param const int client_id       - Client's id
     * @param const QString& material   - Material's code
     * @param int ep_amount             - Amount of epigraphs
     * @param int pits_amount           - Amount of pits
     * @param bool relief               - Relief epigraph's presence
     * @param bool inscription          - Inscription's presence
     * @param bool mount                - Whether the tomb was assembled
     * @param bool provided             - Whether the material was provided by the client
     * @param bool cross                - Cross' presence
     * @param bool drawing              - Drawing's presence
     * @param bool sculpture            - Sculpture's presence
     *
     * @return  QList<QMap<QString, QString>> - A list containing the Tombs' details
     */
    QList<QMap<QString, QString>> getAlike(
        const int client_id,
        const QString& material,
        const int ep_amount,
        const int pits_amount,
        bool relief,
        bool inscription,
        bool mount,
        bool provided,
        bool cross,
        bool drawing,
        bool sculpture
    );

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
     * Gets all the tombs in the archive giving the sum of them sort by year
     *
     * @return  QList<QMap<QString, QString>> - A list containing the general trend info
     */
    QList<QMap<QString, QString>> getGeneralTrend();

    /*
     * Provides the full path to the folder of a given tomb
     *
     * @param   const int       - Progressive number of the tomb
     * @param   const QString&  - Deceased's name
     *
     * @return  QString - The full path to the tomb's folder.
     */
    QString getFolderPath(const int progressive, const QString& name);

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
     * Gets the last progressive number in use
     *
     * @return  int - The last progressive number in use
     */
    int getLastProgresive();

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
     * Gets the last progressive number in use
     *
     * @return  QList<int> - The list of progressive numbers not in use
     */
    QStringList getNotInUseProgressives();

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
     * Checks if the tomb with the given progressive number has been confirmed
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been confirmed, false otherwise
     */
    bool isConfirmed(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been delivered
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been delivered, false otherwise
     */
    bool isDelivered(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been engraved
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been engraved, false otherwise
     */
    bool isEngraved(const int progressive);

    /*
     * Checks if the tomb with the given progressive number has been paied
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the tomb has been paid, false otherwise
     */
    bool isPaid(const int progressive);

    /*
     * Checks if the given progressive number is in use
     *
     * @param   const int progressive - Tomb's progressive number
     *
     * @return  boolean true if the progressive number in use or if the query fails, false otherwise
     */
    bool isProgressiveInUse(const int progressive);

    /*
     * Gets the list of the tombs which can be engraved
     *
     * @return  QList<QMap<QString, QString>>   the list of the tombs which can be engraved
     */
    QList<QMap<QString, QString>> tombsToEngrave();

    /*
     * Gets the list of the tombs not yet paid
     *
     * @return  QList<QMap<QString, QString>>   the list of the tombs not yet paid
     */
    QList<QMap<QString, QString>> tombsToPay();

    /*
     * Deletes a tomb from the database
     *
     * @param   const int progressive - Progressive number
     *
     * @return  void
     */
    void remove(const int progressive);

    /*
     * Sets a tomb's accessories as mounted
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setAccessoriesMounted(const int progressive);

    /*
     * Sets a tomb as confirmed
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setConfirmed(const int progressive);

    /*
     * Sets a tomb as delivered
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setDelivered(const int progressive);

    /*
     * Sets a tomb as engraved
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setEngraved(const int progressive);

    /*
     * Sets a tomb as paid
     *
     * @param   const int progressive - Progressive number
     *
     * @return  bool - True on success, false on failure
     */
    bool setPaid(const int progressive);

    /*
     * Stores a tomb into the database
     *
     * @param const int progressive             - Tomb's progressive number
     * @param const int client_id               - Client's id
     * @param const QString& name               - Deceased's name
     * @param const QString& engraved_names     - Additional names
     * @param const int ep_amount               - Amount of epigraphs engraved
     * @param const int engraved                - 0 not engraved, 1 engraved, 2 bronze
     * @param const double& price               - Price
     * @param const double& deposit             - Deposit
     * @param const bool paid                   - Paid or not
     * @param const QString& material_code      - Material's code
     * @param const QString& tomb_type          - Tomb type
     * @param const QString& tomb_format        - Tomb format
     * @param const QString& vase_code          - Vase's code
     * @param const QString& lamp_code          - Lamp's code
     * @param const QString& flame_code         - Flame's code
     * @param const bool mounted                - Whether the tomb have to be mounted or not
     * @param const bool mat_provided           - Whether the material was provided or not
     * @param const bool ep_relief              - Whether the tomb have a relief engraved epigraph or not
     * @param const bool inscription            - Whether the tomb have an inscription or not
     * @param const QString& sculpture_code     - Sculpture's code
     * @param const& float sculpture_height     - The sculpture height, 0 if the tomb has no sculpture
     * @param const QString& pit_format_one     - Pit one format
     * @param const QString& pit_type_one       - Pit one type
     * @param const QString& pit_format_two     - Pit two format
     * @param const QString& pit_type_two       - Pit two type
     * @param const QString& pit_format_three   - Pit three format
     * @param const QString& pit_type_three     - Pit three type
     * @param const QString& pit_format_four    - Pit four format
     * @param const QString& pit_type_four      - Pit four type
     * @param const QString& pit_format_five    - Pit five format
     * @param const QString& pit_type_five      - Pit five type
     * @param const QString& pit_format_six     - Pit six format
     * @param const QString& pit_type_six       - Pit six type
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
        const QString& engraved_names,
        const int ep_amount,
        const int engraved,
        const double& price,
        const double& deposit,
        const bool paid,
        const QString& material_code,
        const QString& tomb_type,
        const QString& tomb_format,
        const QString& vase_code,
        const QString& lamp_code,
        const QString& flame_code,
        const QString& cross_code,
        const QString& sacred_code,
        const QString& sculpture_code,
        const float& sculpture_height,
        const bool mounted,
        const bool mat_provided,
        const bool ep_relief,
        const bool inscription,
        const QString& pit_format_one,
        const QString& pit_type_one,
        const QString& pit_format_two,
        const QString& pit_type_two,
        const QString& pit_format_three,
        const QString& pit_type_three,
        const QString& pit_format_four,
        const QString& pit_type_four,
        const QString& pit_format_five,
        const QString& pit_type_five,
        const QString& pit_format_six,
        const QString& pit_type_six,
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
     * @param const QString& engraved_names     - Additional names
     * @param const int ep_amount               - Amount of epigraphs engraved
     * @param const int engraved                - 0 not engraved, 1 engraved, 2 bronze
     * @param const double& price               - Price
     * @param const double& deposit             - Deposit
     * @param const bool paid                   - Paid or not
     * @param const QString& material_code      - Material's code
     * @param const QString& tomb_type          - Tomb type
     * @param const QString& tomb_format        - Tomb format
     * @param const QString& vase_code          - Vase's code
     * @param const QString& lamp_code          - Lamp's code
     * @param const QString& flame_code         - Flame's code
     * @param const bool mounted                - Whether the tomb have to be mounted or not
     * @param const bool mat_provided           - Whether the material was provided or not
     * @param const bool ep_relief              - Whether the tomb have a relief engraved epigraph or not
     * @param const bool inscription            - Whether the tomb have an inscription or not
     * @param const QString& sculpture_code     - Sculpture's code
     * @param const float& sculpture_height     - The sculpture height, 0 if the tomb has no sculpture
     * @param const QString& pit_format_one     - Pit one format
     * @param const QString& pit_type_one       - Pit one type
     * @param const QString& pit_format_two     - Pit two format
     * @param const QString& pit_type_two       - Pit two type
     * @param const QString& pit_format_three   - Pit three format
     * @param const QString& pit_type_three     - Pit three type
     * @param const QString& pit_format_four    - Pit four format
     * @param const QString& pit_type_four      - Pit four type
     * @param const QString& pit_format_five    - Pit five format
     * @param const QString& pit_type_five      - Pit five type
     * @param const QString& pit_format_six     - Pit six format
     * @param const QString& pit_type_six       - Pit six type
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
        const QString& engraved_names,
        const int ep_amount,
        const int engraved,
        const double& price,
        const double& deposit,
        const bool paid,
        const QString& material_code,
        const QString& tomb_type,
        const QString& tomb_format,
        const QString& vase_code,
        const QString& lamp_code,
        const QString& flame_code,
        const QString& cross_code,
        const QString& sacred_code,
        const QString& sculpture_code,
        const float& sculpture_height,
        const bool mounted,
        const bool mat_provided,
        const bool ep_relief,
        const bool inscription,
        const QString& pit_format_one,
        const QString& pit_type_one,
        const QString& pit_format_two,
        const QString& pit_type_two,
        const QString& pit_format_three,
        const QString& pit_type_three,
        const QString& pit_format_four,
        const QString& pit_type_four,
        const QString& pit_format_five,
        const QString& pit_type_five,
        const QString& pit_format_six,
        const QString& pit_type_six,
        const QString& notes,
        const bool accessories_mounted,
        const QString& ordered_at,
        const QString& proofed_at,
        const QString& confirmed_at,
        const QString& engraved_at,
        const QString& delivered_at
    );

private:
    const QString table = "tomb";
    QSqlDatabase db;
    Settings* settings;

    const QString not_engraved = "No";

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Gives the state of the accessories mounted on the tomb
     *
     * @param const QString& vase       - Vase's code
     * @param const QString& lamp       - Lamp's code
     * @param const QString& flame      - Flame's code
     * @param const QString& mounted    - Whether the accessories are mounted or not
     * 
     * @returnn QString - The state of the accessories mounted. 1=yes, 0=no, -=no accessories required
     */
    QString accessoriesMountedState(const QString& vase, const QString& lamp, const QString& flame, const QString& mounted);
};
