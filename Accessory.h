#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include "ui_Accessory.h"

class Accessory : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the accessory object.
     *
     * @param	QSqlDatabase*	db	- Pointer to the database connection
     * @param	const QString&	table	- Database table to use
     * @param	QWidget*        parent	- Parent widget
     */
    Accessory(QSqlDatabase* db, const QString& table, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the accessory object.
     */
    ~Accessory();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the accessories' data
     *
     * @return  QList<QMap<QString, QString>> - A list containing the accessories' data
     */
    QList<QMap<QString, QString>> get();

    /*
     * Gets all the accessories' names
     *
     * @param const QString& name   - The accessory's name
     * 
     * @return  QString - The accessory's code id found, empty string otherwise
     */
    QString getCode(const QString& name);

    /*
     * Gets all the accessories' names
     *
     * @return  QStringList - A list containing the accessories' names
     */
    QStringList getNames();

    /*
     * Removes a accessory from the database
     *
     * @param const QString& code - Accessory's code
     *
     * @return  void
     */
    void remove(const QString& code);

    /*
     * Updates an accessory into the database
     *
     * @param const QString& code   - Accessory's code
     * @param const QString& name   - Accessory's name
     *
     * @return  void
     */
    void update(const QString& code, const QString& name);

protected slots:

    /********** PRIVATE SLOTS **********/

    /*
     * Gets the data from the dialog window and stores a new accessory into the database
     *
     * @return  void
     */
    void slotAddAccessory();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
    Ui::Dialog ui;
    const QString table;
    QSqlDatabase* db;
    QWidget* parent;
};

