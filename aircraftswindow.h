#ifndef AIRCRAFTSWINDOW_H
#define AIRCRAFTSWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "rowredactor.h"

namespace Ui {
class AircraftsWindow;
}

class AircraftsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AircraftsWindow(QWidget *parent = nullptr);
    ~AircraftsWindow();

    QTableWidget* getTableWidget();
    void addAircraftImage(QByteArray imageData);
    void setAircraftPhoto(QByteArray imageData);

public slots:
    void showRowRedactor(QTableWidgetItem *item);
    void getNewAircraftData(QJsonObject jsonObject);
    void showPreviousImage();
    void showNextImage();

private:
    Ui::AircraftsWindow *ui;

    QStringList headers;
    RowRedactor *rowRedactor = nullptr;
    QList<QByteArray> imageList;
    int currentImage = 0;

signals:
    void updateAircraft(QJsonObject jsonObject);
    void getAircraftPhoto(QString hexCode);
};

#endif // AIRCRAFTSWINDOW_H
