#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include "logindialog.h"
#include "aircraftswindow.h"
#include "airbaseswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void login(QString username, QString pass);
    void refreshTokens();
    void fillTable(QTableWidget *tableWidget, QJsonArray &array);
    void updateAirbase(QJsonObject jsonObject);
    void updateAircraft(QJsonObject jsonObject);
    void loadAircraftPhoto(QString hexCode);

signals:

private slots:

    void on_airplanesPushButton_clicked();

    void on_airbasesPushButton_clicked();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager manager;
    QString accessToken, refreshToken;
    QTimer *refreshTokensTimer = nullptr;

    LoginDialog *loginDialog = nullptr;
    AircraftsWindow *airplanesWindow = nullptr;
    AirbasesWindow *airbasesWindow = nullptr;

    bool loginned = false;
};
#endif // MAINWINDOW_H
