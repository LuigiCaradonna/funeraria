#pragma once
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QDate>
#include "ui_NewAccessory.h"

class Flame : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Flame object.
     *
     * @param	const QSqlDatabase&	db	- Reference to the database connection
     */
    Flame(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Flame object.
     */
    ~Flame();

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Gets all the flames
     * 
     * @return  QList<QStringList> - A list containing the flames' id and name
     */
    QList<QStringList> get();

    /*
     * Updates a flame into the database
     * 
     * @param const QString& id     - Id of the flame to update
     * @param const QString& name   - Name of the flame
     *
     * @return  void
     */
    void update(const QString& id, const QString& name);

    /*
     * Removes a flame from the database
     *
     * @param const QString& id - Id of the flame to remove
     *
     * @return  void
     */
    void remove(const QString& id);

protected slots:

    /*
     * Gets the data from the dialog window and stores a new flame into the database
     *
     * @return  void
     */
    void slotAddFlame();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
    Ui::Dialog ui;
    const QString table = "flames";
    const QSqlDatabase& db;
    QWidget* parent;
};

