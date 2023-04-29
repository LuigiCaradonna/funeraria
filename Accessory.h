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
     */
    Accessory(QSqlDatabase* db, const QString& table, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the accessory object.
     */
    ~Accessory();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the accessories
     *
     * @return  QList<QStringList> - A list containing the accessories' id and name
     */
    QList<QStringList> get();

    /*
     * Updates a accessory into the database
     *
     * @param const QString& id     - Id of the accessory to update
     * @param const QString& name   - Name of the accessory
     *
     * @return  void
     */
    void update(const QString& id, const QString& name);

    /*
     * Removes a accessory from the database
     *
     * @param const QString& id - Id of the accessory to remove
     *
     * @return  void
     */
    void remove(const QString& id);

protected slots:

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

