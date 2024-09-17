#include "ReportUi.h"

/********** CONSTRUCTOR **********/

ReportUi::ReportUi(const QSqlDatabase& db, QWidget* parent)
    : db(db), parent(parent)
{
    this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("funeraria.png"));
    this->setWindowTitle("Report");

    this->connect(this->ui.btnReport, &QPushButton::clicked, this, &ReportUi::slotGenerateReport);
    this->connect(this->ui.cbClient, &QComboBox::currentTextChanged, this, &ReportUi::slotUpdateRadioState);
    this->connect(this->ui.cbYear, &QComboBox::currentTextChanged, this, &ReportUi::slotUpdateRadioState);

    this->client = new Client(this->db);

    // Get the clients' name
    QStringList clients = client->getNames();
    // Add the clients to the combo box
    this->ui.cbClient->addItem("Tutti");
    this->ui.cbClient->addItems(clients);

    // Populate the years combo box, years go back until 2020
    this->ui.cbYear->addItem("Tutti");
    QString this_year = QDate::currentDate().toString("yyyy");
    for (int i = this_year.toInt(); i >= 2020; i--) {
        this->ui.cbYear->addItem(QString::number(i));
    }

    // Initially, the radio buttons are disabled because the client is set to "Tutti"
    this->ui.rbGroupYes->setEnabled(false);
    this->ui.rbGroupNo->setEnabled(false);
}

/********** DESTRUCTOR **********/

ReportUi::~ReportUi()
{
    delete this->client;
}

/********** PROTECTED SLOTS **********/

void ReportUi::slotGenerateReport()
{
    Tomb* tomb = new Tomb(this->db);

    int client_id = this->client->getId(this->ui.cbClient->currentText());
    int year;

    if (this->ui.cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->ui.cbYear->currentText().toInt();
    }

    bool year_by_year = this->ui.rbGroupYes->isEnabled() && this->ui.rbGroupYes->isChecked();

    QList<QMap<QString, QString>> report = tomb->getReport(client_id, year, this->ui.chbEngraved->isChecked(), year_by_year);

    this->showReport(report, year_by_year);

    delete tomb;
}

void ReportUi::slotUpdateRadioState()
{
    // Radio buttons to group or split the tombs count for each year for the given client
    // True if a specific client is selected and all the years are selected
    bool enabled = this->ui.cbClient->currentText() != "Tutti" && this->ui.cbYear->currentText() == "Tutti";

    this->ui.rbGroupYes->setEnabled(enabled);
    this->ui.rbGroupNo->setEnabled(enabled);
}

/********** PRIVATE FUNCTIONS **********/

void ReportUi::showReport(QList<QMap<QString, QString>> report, bool year_by_year)
{
    Client* client = new Client(this->db);

    // Block the signals while building the table
    const QSignalBlocker blocker(this->ui.tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{"Cliente", "Quantità"};

    if (year_by_year) {
        headers.append("Anno");
    }

    // +1 to add the last row with the total
    this->ui.tableWidget->setRowCount(report.size() + 1);
    this->ui.tableWidget->setColumnCount(headers.size());
    // this->ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // this->ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui.tableWidget->setHorizontalHeaderLabels(headers);

    this->ui.tableWidget->setColumnWidth(0, 200);   // Cliente
    this->ui.tableWidget->setColumnWidth(1, 60);    // Quantità

    int rows = report.size();
    int total = 0;

    for (int i = 0; i < rows; i++) {

        if (i % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        QTableWidgetItem* name = new QTableWidgetItem(client->getName(report[i]["client_id"].toInt()));
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QTableWidgetItem* amount = new QTableWidgetItem(report[i]["amount"]);
        amount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        name->setBackground(QBrush(this->row_bg));
        amount->setBackground(QBrush(this->row_bg));

        this->ui.tableWidget->setItem(i, 0, name);
        this->ui.tableWidget->setItem(i, 1, amount);

        if (year_by_year) {
            QTableWidgetItem* year = new QTableWidgetItem(report[i]["year"]);
            year->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            year->setBackground(QBrush(this->row_bg));

            this->ui.tableWidget->setItem(i, 2, year);
        }

        total += report[i]["amount"].toInt();
    }

    QTableWidgetItem* name = new QTableWidgetItem("Totale");
    name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* amount = new QTableWidgetItem(QString::number(total));
    amount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    this->row_bg == this->row_even ? this->row_odd : this->row_even;

    name->setBackground(QBrush(this->row_bg));
    amount->setBackground(QBrush(this->row_bg));

    this->ui.tableWidget->setItem(rows, 0, name);
    this->ui.tableWidget->setItem(rows, 1, amount);

    delete client;
}

void ReportUi::clearTable()
{
    int row_number = 1;
    for (int i = 0; i < this->ui.tableWidget->rowCount(); i++) {
        this->ui.tableWidget->setRowHeight(i, 30);
        for (int j = 0; j < this->ui.tableWidget->columnCount(); j++) {
            // Usually the last column holds a button
            if (j == this->ui.tableWidget->columnCount() - 1) {
                QPushButton* pbutton = qobject_cast<QPushButton*>(this->ui.tableWidget->cellWidget(i, j));
                if (pbutton) {
                    pbutton->disconnect(); // Disconnect any connections
                    pbutton->setParent(nullptr); // Reparent the button
                    delete pbutton;
                    pbutton = nullptr;
                }
                else {
                    QTableWidgetItem* item = this->ui.tableWidget->item(i, j);
                    delete item;
                    item = nullptr;
                }
            }
            else {
                QTableWidgetItem* item = this->ui.tableWidget->item(i, j);
                delete item;
                item = nullptr;
            }
        }
    }

    this->ui.tableWidget->clear();
}