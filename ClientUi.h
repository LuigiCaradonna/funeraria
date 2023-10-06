#pragma once

#include <QDialog>
#include <QMap>
#include <QSqlDatabase>
#include <QDebug>
#include <QRegularExpression>
#include <QDate>
#include <QMessageBox>
#include "Client.h"
#include "ui_Client.h"

class ClientUi : public QDialog
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the Client object.
     *
     * @param	const QSqlDatabase*	db	- Reference to the database connection
     */
    ClientUi(const QSqlDatabase& db, QWidget* parent = nullptr);

    /********** DESTRUCTOR **********/

    /*
     * Destructs the Client object.
     */
    ~ClientUi();

protected slots:

    /********** PROTECTED SLOTS **********/

    /*
     * Saves the client's data, responds to the dialog window's save button
     *
     * @return  void
     */
    void slotSave();

    /*
     * Closes the dialog window
     *
     * @return  void
     */
    void slotCloseDialog();

private:
    Ui::ClientClass ui;
    const QString table = "clients";
    QSqlDatabase db;
    QWidget* parent;
    QString name;

    /********** PRIVATE FUNCTIONS **********/

    void updateForm();
};
