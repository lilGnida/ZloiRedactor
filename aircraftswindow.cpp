#include "aircraftswindow.h"
#include "QtNetwork/qnetworkreply.h"
#include "ui_aircraftswindow.h"

AircraftsWindow::AircraftsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AircraftsWindow)
{
    ui->setupUi(this);

    headers << "reg" << "hexcode" << "type" << "icao_type" << "long_type" << "country" << "air_squadron" << "air_group" <<
    "air_wing" << "air_forse" << "air_command" << "id_airbase" << "air_squadron_alt" << "air_group_alt" << "air_wing_alt" <<
    "air_forse_alt" << "air_command_alt" << "id_airbase_alt" << "version" << "update_date";

    ui->tableWidget->setColumnCount(headers.count());

    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(headers.at(i)));
    }

    connect(ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &AircraftsWindow::showRowRedactor);

    rowRedactor = new RowRedactor(this);

    connect(rowRedactor, &RowRedactor::sendNewData, this, &AircraftsWindow::getNewAircraftData);
    connect(rowRedactor, &RowRedactor::askPreviousImage, this, &AircraftsWindow::showPreviousImage);
    connect(rowRedactor, &RowRedactor::askNextImage, this, &AircraftsWindow::showNextImage);
}

AircraftsWindow::~AircraftsWindow()
{
    delete ui;
}

QTableWidget *AircraftsWindow::getTableWidget()
{
    return ui->tableWidget;
}

void AircraftsWindow::setAircraftPhoto(QByteArray imageData)
{
    rowRedactor->setImage(imageData);
}

void AircraftsWindow::showRowRedactor(QTableWidgetItem *item)
{
    int row = item->row();
    QMap <QString, QString> data;

    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        data.insert(headers.at(i), ui->tableWidget->item(row, i)->data(Qt::DisplayRole).toString());
    }

    emit getAircraftPhoto(data.value("hexcode"));

    rowRedactor->setData(data);

    imageList.clear();
    rowRedactor->show();
}

void AircraftsWindow::addAircraftImage(QByteArray imageData)
{
    if (imageList.isEmpty()) {
        setAircraftPhoto(imageData);
    }

    imageList << imageData;
}

void AircraftsWindow::getNewAircraftData(QJsonObject jsonObject)
{
    emit updateAircraft(jsonObject);
}

void AircraftsWindow::showPreviousImage()
{
    if (imageList.count() > 1) {
        if (currentImage == 0) {
            currentImage = imageList.count() - 1;
            rowRedactor->changeImage(imageList.at(currentImage));
        } else {
            rowRedactor->changeImage(imageList.at(--currentImage));
        }
    }
}

void AircraftsWindow::showNextImage()
{
    if (imageList.count() > 1) {
        if (currentImage == imageList.count() - 1) {
            currentImage = 0;
            rowRedactor->changeImage(imageList.at(currentImage));
        } else {
            rowRedactor->changeImage(imageList.at(++currentImage));
        }
    }
}

