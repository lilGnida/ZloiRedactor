#ifndef AIRBASESWINDOW_H
#define AIRBASESWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMap>
#include <QJsonObject>
#include "rowredactor.h"

namespace Ui {
class AirbasesWindow;
}

class AirbasesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AirbasesWindow(QWidget *parent = nullptr);
    ~AirbasesWindow();

    QTableWidget* getTableWidget();

private:
    Ui::AirbasesWindow *ui;

    QStringList headers;
    RowRedactor *rowRedactor = nullptr;

public slots:
    void showRowRedactor(QTableWidgetItem *item);
    void getNewAirbaseData(QJsonObject jsonObject);

signals:
    void updateAirbase(QJsonObject jsonObject);
};

#endif // AIRBASESWINDOW_H
