#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include "ui_Tomb.h"

class Tomb : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Tomb object.
     *
     * @param	QSqlDatabase*	db	- Pointer to the database connection
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
     * @return  QList<QStringList> - A list containing the Tombs' id and name
     */
    QList<QStringList> get(int client_id, int year, const QString& filter);

    /*
     * Gets the given tomb's datails
     *
     * @param   int - Tomb's progressive number
     *
     * @return  QMap<QString, QString> - A map containing the tomb's data
     */
    QMap<QString, QString> getDetails(int progressive);

    bool update(QString id, QString value);

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

private:
    Ui::TombClass ui;
    const QString table = "tombs";
    QSqlDatabase* db;
    QWidget* parent;
    int progressive;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Updates the content of the QDialog according to the current tomb selected
     *
     * @return  void
     */
    void updateForm();
};
