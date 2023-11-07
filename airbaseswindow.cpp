#include "airbaseswindow.h"
#include "ui_airbaseswindow.h"

AirbasesWindow::AirbasesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AirbasesWindow)
{
    ui->setupUi(this);

    headers << "id" << "name" << "lat" << "lon" << "country" << "type" << "icao" << "elevation" << "continent";

    ui->tableWidget->setColumnCount(headers.count());

    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(headers.at(i)));
    }

    connect(ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &AirbasesWindow::showRowRedactor);

    rowRedactor = new RowRedactor(this);

    connect(rowRedactor, &RowRedactor::sendNewData, this, &AirbasesWindow::getNewAirbaseData);
}

AirbasesWindow::~AirbasesWindow()
{
    delete ui;
}

QTableWidget *AirbasesWindow::getTableWidget()
{
    return ui->tableWidget;
}

void AirbasesWindow::showRowRedactor(QTableWidgetItem *item)
{
    int row = item->row();
    QMap <QString, QString> data;

    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        data.insert(headers.at(i), ui->tableWidget->item(row, i)->data(Qt::DisplayRole).toString());
    }

    rowRedactor->setData(data);

    rowRedactor->show();
}

void AirbasesWindow::getNewAirbaseData(QJsonObject jsonObject)
{
    emit updateAirbase(jsonObject);
}
