#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setLoginAccess(bool ok)
{
    loginAccessed = ok;
}

bool LoginDialog::getLoginAccess()
{
    return loginAccessed;
}

void LoginDialog::on_pushButton_clicked()
{
    QString username = ui->loginLineEdit->text(),
            pass = ui->passwordLineEdit->text();

    emit login(username, pass);
}

void LoginDialog::hideEvent(QHideEvent *event)
{
    if (!loginAccessed) {
        emit quit();
    }

    QWidget::hideEvent(event);
}

