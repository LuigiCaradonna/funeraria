#pragma once

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QDialog>
#include <QTimer>
#include <QTableWidget>
#include "ui_Report.h"
#include "Client.h"
#include "Tomb.h"
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QCategoryAxis>
#include <QGraphicsSimpleTextItem>


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
     * Adds the value labels to each bar of the chart
     *
     * @return  void
     */
    void slotAddValueLabels();

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
    QTableWidget* tableWidget;
    QChart* chart;
    QChartView* chartView;
    QBarSeries* series;
    Client* client;
    QColor row_bg = QColor(255, 255, 255);
    QColor row_even = QColor(255, 255, 255);
    QColor row_odd = QColor(235, 235, 235);
    const QString icons_folder = "assets\\icons\\";

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Generates the report as a table
     *
     * @param	QList<QMap<QString, QString>> report	- Report's data to show
     * @param	const QString& group                    - Group policy, default none
     * 
     * @return  void
     */
    void showReportTable(QList<QMap<QString, QString>> report, const QString& group = "");

    /*
     * Generates the report as a bar chart
     *
     * @param	QList<QMap<QString, QString>> report	- Report's data to show
     * @param	const QString& title	                - Graph title
     * @param	const QString& category                 - Graph category
     * @param	const QString& group	                - Group policy, default none
     * 
     * @return  void
     */
    void showReportGraph(
        QList<QMap<QString, QString>> report, 
        const QString& title, 
        const QString& category,
        const QString& group = ""
    );

    /*
     * Resets the layout containing the report
     *
     * @return  void
     */
    void clearLayout();

    /*
     * Clears the table content and deletes all the created pointers.
     *
     * @return void
     */
    void clearTable();
};

