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
    this->connect(this->ui.rbByMonth, &QRadioButton::toggled, this, &ReportUi::slotUpdateRadioState);
    this->connect(this->ui.rbByYear, &QRadioButton::toggled, this, &ReportUi::slotUpdateRadioState);
    this->connect(this->ui.rbOverall, &QRadioButton::toggled, this, &ReportUi::slotUpdateRadioState);
    this->connect(this->ui.rbGraph, &QRadioButton::toggled, this, &ReportUi::slotUpdateRadioState);

    this->tableWidget = new QTableWidget();
    this->chart = new QChart;
    this->chartView = new QChartView(this->chart);
    this->series = new QBarSeries();

    this->client = new Client(this->db);

    // Get the clients' name
    QStringList clients = client->getNames();
    // Add the clients to the combo box
    this->ui.cbClient->addItem("Tutti");
    this->ui.cbClient->addItems(clients);

    // Populate the years combo box, years go back until 2013
    this->ui.cbYear->addItem("Tutti");
    QString this_year = QDate::currentDate().toString("yyyy");
    for (int i = this_year.toInt(); i >= 2013; i--) {
        this->ui.cbYear->addItem(QString::number(i));
    }
}

/********** DESTRUCTOR **********/

ReportUi::~ReportUi()
{
    delete this->tableWidget;
    delete this->chart;
    delete this->chartView;
    delete this->client;
    delete this->series;
}

/********** PROTECTED SLOTS **********/

void ReportUi::slotAddValueLabels()
{
    // Customize the labels' font
    QFont font;
    font.setPixelSize(14);

    for (int i = 0; i < this->series->count(); ++i) {
        QBarSet* set = this->series->barSets().at(i);
        for (int j = 0; j < set->count(); ++j) {
            // Calculate the center of the bar
            qreal barX = this->chart->mapToPosition(QPointF(j, set->at(j)), this->series).x();
            qreal barY = this->chart->mapToPosition(QPointF(j, set->at(j)), this->series).y();

            // Create and position the text item
            QGraphicsSimpleTextItem* label = new QGraphicsSimpleTextItem(QString::number(set->at(j)));
            label->setFont(font);
            
            if (set->at(j) > 5) {
                label->setBrush(QBrush(Qt::white));
                // +25 pushes the lable down inside the bar
                label->setPos(barX - label->boundingRect().width() / 2, barY - label->boundingRect().height() + 25);
            }
            else {
                // Write the value in black outside the bar
                label->setBrush(QBrush(Qt::black));
                label->setPos(barX - label->boundingRect().width() / 2, barY - label->boundingRect().height());
            }

            this->chart->scene()->addItem(label);
        }
    }
}

void ReportUi::slotGenerateReport()
{
    Tomb* tomb = new Tomb(this->db);

    int client_id = this->client->getId(this->ui.cbClient->currentText());
    int year;
    QString title, category;

    if (this->ui.cbYear->currentText() == "Tutti") {
        year = 0;
    }
    else {
        year = this->ui.cbYear->currentText().toInt();
    }

    QString group = "";

    if (this->ui.rbByYear->isChecked()) {
        group = "year";
    }
    else if (this->ui.rbByMonth->isChecked()) {
        group = "month";
    }

    if (this->ui.cbClient->currentText() == "Tutti" && this->ui.cbYear->currentText() == "Tutti") {
        QList<QMap<QString, QString>> report =
            tomb->getReport(client_id, year, this->ui.chbEngraved->isChecked(), group, false);

        if (this->ui.rbGraph->isChecked()) {
            if (group == "year") {
                int total = 0;
                for (int i = 0; i < report.size(); i++) {
                    total += report[i]["amount"].toInt();
                }
                // Totale di ogni cliente da sempre
                // category: nomi, set: quantità
                title = "Andamento ordini annuali. Totale: " + QString::number(total);
                category = "year";
                this->showReportGraph(report, title, category, group);
            }
            else if (group == "month") {
                int total = 0;
                for (int i = 0; i < report.size(); i++) {
                    total += report[i]["amount"].toInt();
                }
                // Totale di ogni cliente da sempre
                // category: nomi, set: quantità
                title = "Andamento ordini mensili. Totale: " + QString::number(total);
                category = "month";
                this->showReportGraph(report, title, category, group);
            }
        }
        else {
            // Totale di ogni cliente da sempre
            this->showReportTable(report, group);
        }
    }

    if (this->ui.cbClient->currentText() != "Tutti" && this->ui.cbYear->currentText() == "Tutti") {
        QList<QMap<QString, QString>> report =
            tomb->getReport(client_id, year, this->ui.chbEngraved->isChecked(), group, false);

        if (group == "year") {
            if (this->ui.rbGraph->isChecked()) {
                int total = 0;
                for (int i = 0; i < report.size(); i++) {
                    total += report[i]["amount"].toInt();
                }
                // Una barra ogni anno col totale delle lapidi richieste dal cliente
                // Nome sopra "nome del cliente"
                // category: anno, set: quantità
                title = this->ui.cbClient->currentText() + " - Totale: " + QString::number(total);
                category = "year";
                this->showReportGraph(report, title, category, group);
            }
            else {
                // Una riga ogni anno col totale delle lapidi richieste dal cliente
                this->showReportTable(report, group);
            }
        }
        else if (group == "month") {
            if (this->ui.rbGraph->isChecked()) {
                int total = 0;
                for (int i = 0; i < report.size(); i++) {
                    total += report[i]["amount"].toInt();
                }
                // Una barra ogni anno col totale delle lapidi richieste dal cliente
                // Nome sopra "nome del cliente"
                // category: anno, set: quantità
                title = this->ui.cbClient->currentText() + " - Totale: " + QString::number(total);
                category = "month";
                this->showReportGraph(report, title, category, group);
            }
            else {
                // Una riga ogni anno col totale delle lapidi richieste dal cliente
                this->showReportTable(report, group);
            }
        }
        else {
            // totale del cliente specificato da sempre
            // basta la tabella, c'è una sola riga contenente il nome del cliente ed il totale delle lapidi ordinate da sempre
            this->showReportTable(report, group);
        }
    }

    if (this->ui.cbClient->currentText() != "Tutti" && this->ui.cbYear->currentText() != "Tutti") {
        QList<QMap<QString, QString>> report =
            tomb->getReport(client_id, year, this->ui.chbEngraved->isChecked(), group, false);

        if (this->ui.rbGraph->isChecked()) {
            int total = 0;
            for (int i = 0; i < report.size(); i++) {
                total += report[i]["amount"].toInt();
            }
            // Una barra ogni anno col totale delle lapidi richieste dal cliente
            // Nome sopra "nome del cliente"
            // category: anno, set: quantità
            title = this->ui.cbClient->currentText() + " - Totale: " + QString::number(total);
            category = "month";
            this->showReportGraph(report, title, category, group);
        }
        else {
            // Tabella con una sola riga contenente l'anno e la quantità di lapidi richieste dal cliente
            this->showReportTable(report, group);
        }
    }

    if (this->ui.cbClient->currentText() == "Tutti" && this->ui.cbYear->currentText() != "Tutti") {
        bool by_client = group == "month" ? false : true;

        QList<QMap<QString, QString>> report =
            tomb->getReport(client_id, year, this->ui.chbEngraved->isChecked(), group, by_client);

        if (this->ui.rbGraph->isChecked()) {
            int total = 0;
            for (int i = 0; i < report.size(); i++) {
                total += report[i]["amount"].toInt();
            }
            // Una barra ogni anno col totale delle lapidi richieste dal cliente
            // Nome sopra "nome del cliente"
            // category: anno, set: quantità
            title = "Ordini mensili per l'anno " + QString::number(year) + " - Totale: " + QString::number(total);
            category = "month";
            this->showReportGraph(report, title, category, group);
        }
        else {
            // Una riga per ogni cliente col totale delle lapidi richieste nell'anno
            this->showReportTable(report, group);
        }
    }
    
    delete tomb;
}

void ReportUi::slotUpdateRadioState()
{
    // All clients and all years
    if (this->ui.cbClient->currentText() == "Tutti" && this->ui.cbYear->currentText() == "Tutti") {
        // Overall is not allowed in this case
        if (this->ui.rbOverall->isChecked()) {
            // Set by year as default
            this->ui.rbByYear->setChecked(true);
        }
    }

    // Specific client and all years
    if (this->ui.cbClient->currentText() != "Tutti" && this->ui.cbYear->currentText() == "Tutti") {
        // If overall is selected
        if (this->ui.rbOverall->isChecked()) {
            this->ui.rbTable->setChecked(true);
        }
    }

    /*
        Specific client selected, specific year selected
        Only table output available, no use for graph
    */
    if (this->ui.cbClient->currentText() != "Tutti" && this->ui.cbYear->currentText() != "Tutti") {
        // By Year is not allowed in this case
        if (this->ui.rbByYear->isChecked()) {
            // Set overall as default
            this->ui.rbOverall->setChecked(true);
        }

        if (this->ui.rbOverall->isChecked()) {
            // Only the table is available
            this->ui.rbTable->setChecked(true);
        }
    }

    /*
        All client selected, specific year selected
        Both the table and graph output are available
    */
    if (this->ui.cbClient->currentText() == "Tutti" && this->ui.cbYear->currentText() != "Tutti") {
        // By Year is not allowed in this case
        if (this->ui.rbByYear->isChecked()) {
            // Set overall as default
            this->ui.rbOverall->setChecked(true);
        }
    }
}

/********** PRIVATE FUNCTIONS **********/

void ReportUi::showReportTable(QList<QMap<QString, QString>> report, const QString& group)
{
    this->clearLayout();

    // Block the signals while building the table
    const QSignalBlocker blocker(this->tableWidget);

    // Reset the table's content
    this->clearTable();

    QStringList headers{ "Cliente", "Quantità" };

    if (group == "year") {
        headers.append("Anno");
    }
    else if (group == "month") {
        headers.append("Mese");
    }

    // +1 to add the last row with the total
    this->tableWidget->setRowCount(report.size() + 1);
    this->tableWidget->setColumnCount(headers.size());
    this->tableWidget->setHorizontalHeaderLabels(headers);

    this->tableWidget->setColumnWidth(0, 200);   // Cliente
    this->tableWidget->setColumnWidth(1, 60);    // Quantità

    int rows = report.size();
    int total = 0;

    for (int i = 0; i < rows; i++) {

        if (i % 2 == 0) {
            this->row_bg = this->row_even;
        }
        else {
            this->row_bg = this->row_odd;
        }

        QTableWidgetItem* name = new QTableWidgetItem(this->client->getName(report[i]["client_id"].toInt()));
        name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QTableWidgetItem* amount = new QTableWidgetItem(report[i]["amount"]);
        amount->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        name->setBackground(QBrush(this->row_bg));
        amount->setBackground(QBrush(this->row_bg));

        this->tableWidget->setItem(i, 0, name);
        this->tableWidget->setItem(i, 1, amount);

        if (group == "year") {
            QTableWidgetItem* year = new QTableWidgetItem(report[i]["year"]);
            year->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            year->setBackground(QBrush(this->row_bg));

            this->tableWidget->setItem(i, 2, year);
        }
        else if (group == "month") {
            QTableWidgetItem* month = new QTableWidgetItem(report[i]["month"]);
            month->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            month->setBackground(QBrush(this->row_bg));

            this->tableWidget->setItem(i, 2, month);
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

    this->tableWidget->setItem(rows, 0, name);
    this->tableWidget->setItem(rows, 1, amount);

    this->ui.ReportContainer->addWidget(this->tableWidget);
}

void ReportUi::showReportGraph(
    QList<QMap<QString, QString>> report, 
    const QString& title, 
    const QString& category,
    const QString& group
)
{
    QBarSet* set = new QBarSet("Lapidi");
    QCategoryAxis* axisX = new QCategoryAxis();
    QStringList categories;
    QString cat = group;

    this->clearLayout();

    int max = 0;
    for (int i = 0; i < report.size(); i++) {
        if (category == "client_id") {
            cat = this->client->getName(report[i]["client_id"].toInt());
        }

        if (group == "year") {
            // Years prior to 2013 have missing data
            if (report[i]["year"].toInt() < 2013) continue;
        }

        // The inserted set is the last of the list, assign a value to it
        *set << report[i]["amount"].toInt();

        // Holds the category titles, the year
        if (cat == "year") {
            categories << report[i]["year"];
        }
        else if (cat == "month") {
            categories << report[i]["month"];
        }
        else {
            categories << cat;
        }
    }

    // Add the set to the series
    this->series->append(set);

    // Create chart, add data, hide legend, and add axis
    this->chart->addSeries(this->series);

    // Customize the title font
    QFont font;
    font.setPixelSize(18);
    this->chart->setTitleFont(font);
    this->chart->setTitleBrush(QBrush(Qt::black));
    this->chart->setTitle(title);

    // Adds categories to the axes
    QBarCategoryAxis* axis = new QBarCategoryAxis();
    axis->append(categories);
    this->chart->createDefaultAxes();

    this->chart->setAxisX(axis, this->series);
    // Used to display the chart
    this->chartView->setRenderHint(QPainter::Antialiasing);
    this->ui.ReportContainer->addWidget(this->chartView);

    // Add value labels on top of bars, using a delay to wait the graph to be ready before to add the lables
    QTimer::singleShot(100, this, SLOT(slotAddValueLabels()));
}

void ReportUi::clearLayout()
{
    QLayoutItem* child;
    while ((child = this->ui.ReportContainer->takeAt(0)) != 0) {
        this->ui.ReportContainer->removeItem(child);
        delete child;
    }

    delete this->tableWidget;
    delete this->chart;
    delete this->chartView;
    this->series = nullptr;

    this->tableWidget = new QTableWidget();
    this->chart = new QChart;
    this->chartView = new QChartView(this->chart);
    this->series = new QBarSeries();
}

void ReportUi::clearTable()
{
    int row_number = 1;
    for (int i = 0; i < this->tableWidget->rowCount(); i++) {
        this->tableWidget->setRowHeight(i, 30);
        for (int j = 0; j < this->tableWidget->columnCount(); j++) {
            // Usually the last column holds a button
            if (j == this->tableWidget->columnCount() - 1) {
                QPushButton* pbutton = qobject_cast<QPushButton*>(this->tableWidget->cellWidget(i, j));
                if (pbutton) {
                    pbutton->disconnect(); // Disconnect any connections
                    pbutton->setParent(nullptr); // Reparent the button
                    delete pbutton;
                    pbutton = nullptr;
                }
                else {
                    QTableWidgetItem* item = this->tableWidget->item(i, j);
                    delete item;
                    item = nullptr;
                }
            }
            else {
                QTableWidgetItem* item = this->tableWidget->item(i, j);
                delete item;
                item = nullptr;
            }
        }
    }

    this->tableWidget->clear();
}