#pragma once
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "ui_Report.h"
#include "Client.h"
#include "Tomb.h"

class ReportUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the TombUi object.
     *
     * @param	const QSqlDatabase*	db	    - Refrence to the database connection
     * @param	QWidget*            parent	- Parent widget
     */
    ReportUi(const QSqlDatabase & db, QWidget * parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the TombUi object.
     */
    ~ReportUi();

    /********** PUBLIC FUNCTIONS **********/

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Retrieves and formats the data for the report
     *
     * @return  void
     */
    void slotGenerateReport();

    /*
     * Sets the radio buttons enabled/disabled according to the user's selections
     *
     * @return  void
     */
    void slotUpdateRadioState();

private:
    Ui::ReportClass ui;
    QSqlDatabase db;
    QWidget* parent;
    QStringList clients;
    Client* client;
    QColor row_bg = QColor(255, 255, 255);
    QColor row_even = QColor(255, 255, 255);
    QColor row_odd = QColor(235, 235, 235);

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Shows the report generating a table
     *
     * @return  void
     */
    void showReport(QList<QMap<QString, QString>> report, bool year_by_year);

    /*
     * Clears the table content and deletes all the created pointers.
     *
     * @return void
     */
    void clearTable();
};

