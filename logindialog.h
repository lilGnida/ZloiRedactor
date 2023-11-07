#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMainWindow>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void setLoginAccess(bool ok);
    bool getLoginAccess();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginDialog *ui;

    bool loginAccessed = false;

signals:
    void login(QString username, QString pass);
    void quit();

    // QWidget interface
protected:
    void hideEvent(QHideEvent *event);
};

#endif // LOGINDIALOG_H
