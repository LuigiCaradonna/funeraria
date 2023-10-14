#pragma once
#include <QDebug>
#include "Accessory.h"
#include "ui_Accessory.h"

class AccessoryUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the accessory object.
     *
     * @param	const QSqlDatabase& db	    - Reference to the database connection
     * @param	const QString&	    table	- Database table to use
     * @param	QWidget*            parent	- Parent widget
     */
    AccessoryUi(const QSqlDatabase& db, const QString& table, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the accessory object.
     */
    ~AccessoryUi();

protected slots:

    /********** PRIVATE SLOTS **********/

    /*
     * Gets the data from the dialog window and stores a new accessory into the database
     *
     * @return  void
     */
    void slotAddAccessoryUi();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
    Ui::Dialog ui;
    const QString table;
    QSqlDatabase db;
    QWidget* parent;
};

