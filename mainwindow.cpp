#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loginDialog = new LoginDialog(this);

    connect(loginDialog, &LoginDialog::login, this, &MainWindow::login);
    connect(loginDialog, &LoginDialog::quit, this, &MainWindow::close);

    airplanesWindow = new AircraftsWindow(this);
    airbasesWindow = new AirbasesWindow(this);

    connect(airplanesWindow, &AircraftsWindow::updateAircraft, this, &MainWindow::updateAircraft);
    connect(airplanesWindow, &AircraftsWindow::getAircraftPhoto, this, &MainWindow::loadAircraftPhoto);
    connect(airbasesWindow, &AirbasesWindow::updateAirbase, this, &MainWindow::updateAirbase);

    refreshTokensTimer = new QTimer(this);

    connect (refreshTokensTimer, &QTimer::timeout, this, &MainWindow::refreshTokens);

    loginDialog->show();
    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(QString username, QString pass)
{
    QUrl url("https://auth.zloi.space/realms/bots/protocol/openid-connect/token");

    QUrlQuery query;
    query.addQueryItem("grant_type", "password");
    query.addQueryItem("client_id", "bots");
    query.addQueryItem("username", username);
    query.addQueryItem("password", pass);

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = manager.post(request, query.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray resp = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonObject object = doc.object();
            this->accessToken = object.value("access_token").toString();
            this->refreshToken = object.value("refresh_token").toString();
            int expires = object.value("expires_in").toInt();
            this->refreshTokensTimer->setInterval((expires - 100) * 1900);
            this->refreshTokensTimer->start();

            loginned = true;

            loginDialog->setLoginAccess(loginned);
            loginDialog->hide();
        } else {
            loginned = false;

            loginDialog->setLoginAccess(loginned);
        }

        reply->deleteLater();
    });
}

void MainWindow::refreshTokens()
{
    QUrl url("https://auth.zloi.space/realms/bots/protocol/openid-connect/token");

    QUrlQuery query;
    query.addQueryItem("grant_type", "refresh_token");
    query.addQueryItem("client_id", "bots");
    query.addQueryItem("refresh_token", this->refreshToken);

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = manager.post(request, query.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray resp = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonObject object = doc.object();
            this->accessToken = object.value("access_token").toString();
            this->refreshToken = object.value("refresh_token").toString();

            loginned = true;
        } else {
            loginned = false;
        }

        loginDialog->setLoginAccess(loginned);
        reply->deleteLater();
    });
}

void MainWindow::fillTable(QTableWidget *tableWidget, QJsonArray &array)
{
    for (int i = 0; i < array.count(); i++) {
        tableWidget->insertRow(i);

        QJsonObject obj = array.at(i).toObject();

        for (int j = 0; j < tableWidget->columnCount(); j++) {
            QString key = tableWidget->horizontalHeaderItem(j)->text();
            QVariant value = obj.value(key).toVariant();
            QTableWidgetItem *item = new QTableWidgetItem(value.toString());
            tableWidget->setItem(i, j, item);
        }
    }
}

void MainWindow::updateAirbase(QJsonObject jsonObject)
{
    QUrl url("https://armybots.ru/api/flights-bot/airbases/:id");

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString header = "Bearer " + accessToken;
    request.setRawHeader(QByteArray("Authorization"), header.toUtf8());

    QNetworkReply *reply = manager.sendCustomRequest(request, "PATCH", QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << reply;
        } else {
            qDebug() << reply->error();
        }

        reply->deleteLater();
    });

    on_airbasesPushButton_clicked();
}

void MainWindow::updateAircraft(QJsonObject jsonObject)
{
    QUrl url("https://armybots.ru/api/flights-bot/aircrafts/:id");

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString header = "Bearer " + accessToken;
    request.setRawHeader(QByteArray("Authorization"), header.toUtf8());

    QNetworkReply *reply = manager.sendCustomRequest(request, "PATCH", QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << reply;
        } else {
            qDebug() << reply->error();
        }

        reply->deleteLater();
    });

    on_airbasesPushButton_clicked();
}

void MainWindow::loadAircraftPhoto(QString hexCode)
{
    QUrl url("https://armybots.ru/api/flights-bot/flight-photos?hexcode=" + hexCode);

    QNetworkRequest request(url);

    QString header = "Bearer " + accessToken;
    request.setRawHeader(QByteArray("Authorization"), header.toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray resp = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonArray array = doc.array();

            for (int i = 0; i < array.count(); i++) {
                QJsonObject obj = array.at(i).toObject();
                QUrl photoUrl(obj.value("source_link").toString());
                QNetworkRequest photoRequest(photoUrl);
                QNetworkReply *photoReply = manager.get(photoRequest);

                connect(photoReply, &QNetworkReply::finished, [=]() {
                    if (photoReply->error() == QNetworkReply::NoError) {
                        QByteArray data = photoReply->readAll();
                        airplanesWindow->addAircraftImage(data);
                    } else {
                        qDebug() << photoReply->error();
                    }
                });
            }
        } else {
            qDebug() << reply->error();
        }

        reply->deleteLater();
    });
}

void MainWindow::on_airplanesPushButton_clicked()
{
    QUrl url("https://armybots.ru/api/flights-bot/aircrafts/");

    QNetworkRequest request(url);

    QString header = "Bearer " + accessToken;
    request.setRawHeader(QByteArray("Authorization"), header.toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray resp = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonArray array = doc.array();

            fillTable(airplanesWindow->getTableWidget(), array);
            airplanesWindow->show();
        } else {
            qDebug() << reply->error();
        }

        reply->deleteLater();
    });
}


void MainWindow::on_airbasesPushButton_clicked()
{
    QUrl url("https://armybots.ru/api/flights-bot/airbases/");

    QNetworkRequest request(url);

    QString header = "Bearer " + accessToken;
    request.setRawHeader(QByteArray("Authorization"), header.toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray resp = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonArray array = doc.array();

            fillTable(airbasesWindow->getTableWidget(), array);
            airbasesWindow->show();
        } else {
            qDebug() << reply->error();
        }

        reply->deleteLater();
    });
}

