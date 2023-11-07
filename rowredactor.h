#ifndef ROWREDACTOR_H
#define ROWREDACTOR_H

#include <QMainWindow>
#include <QMap>
#include <QLabel>
#include <QLineEdit>
#include <QJsonObject>

namespace Ui {
class RowRedactor;
}

class RowRedactor : public QMainWindow
{
    Q_OBJECT

public:
    explicit RowRedactor(QWidget *parent = nullptr);
    ~RowRedactor();

    void setData(QMap <QString, QString> data);
    void setImage(QByteArray imageData);
    void changeImage(QByteArray imageData);

public slots:
    void showPreviousImage();
    void showNextImage();

private slots:
    void on_cancelPushButton_clicked();

    void on_acceptPushButton_clicked();

private:
    Ui::RowRedactor *ui;

signals:
    void sendNewData(QJsonObject jsonObject);
    void askPreviousImage();
    void askNextImage();
};

#endif // ROWREDACTOR_H
