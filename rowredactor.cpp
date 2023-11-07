#include "rowredactor.h"
#include "ui_rowredactor.h"

RowRedactor::RowRedactor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RowRedactor)
{
    ui->setupUi(this);

    ui->acceptPushButton->setEnabled(false);

    ui->cancelPushButton->setShortcut(Qt::Key_Escape);
}

RowRedactor::~RowRedactor()
{
    delete ui;
}

void RowRedactor::setData(QMap<QString, QString> data)
{
    for (int i = 0; i < this->centralWidget()->children().count(); i++) {
        QObject *obj = this->centralWidget()->children().at(i);
        if (obj->objectName().contains("label") || obj->objectName().contains("lineEdit") || obj->objectName().contains("Image")) {
            obj->deleteLater();
        }
    }

    for (int i = 1; i < data.count() + 1; i++) {
        QString key = data.keys().at(i - 1);
        QString value = data.value(key);

        QLabel *label = new QLabel(this);
        label->setObjectName("label" + QString::number(i));
        label->setText(key);

        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setObjectName("lineEdit" + QString::number(i));
        lineEdit->setText(value);

        connect(lineEdit, &QLineEdit::textChanged, [=]() {
            ui->acceptPushButton->setEnabled(true);
        });

        ui->gridLayout->addWidget(label, i, 0, 1, 2);
        ui->gridLayout->addWidget(lineEdit, i, 2, 1, 2);
    }
}

void RowRedactor::setImage(QByteArray imageData)
{
    QLabel *label = new QLabel(this);
    label->setObjectName("imageLabel");
    QPixmap pixmap;
    pixmap.loadFromData(imageData);
    ui->gridLayout->addWidget(label, 0, 1, 1, 2, Qt::AlignCenter);
    label->setPixmap(pixmap.scaledToWidth(this->width() - 60));

    QPushButton *rightButton = new QPushButton(QIcon(":/Icons/forward.end.png"), "", this);
    QPushButton *leftButton = new QPushButton(QIcon(":/Icons/backward.end.png"), "", this);
    rightButton->setObjectName("nextImageButton");
    leftButton->setObjectName("previousImageButton");
    rightButton->setFixedSize(25, 25);
    leftButton->setFixedSize(25, 25);

    connect(rightButton, &QPushButton::clicked, this, &RowRedactor::showNextImage);
    connect(leftButton, &QPushButton::clicked, this, &RowRedactor::showPreviousImage);

    ui->gridLayout->addWidget(leftButton, 0, 0, Qt::AlignVCenter);
    ui->gridLayout->addWidget(rightButton, 0, 3, Qt::AlignVCenter);
}

void RowRedactor::changeImage(QByteArray imageData)
{
    QPixmap pixmap;
    pixmap.loadFromData(imageData);
    QLabel *label = dynamic_cast<QLabel*>(ui->gridLayout->itemAtPosition(0, 1)->widget());
    label->setPixmap(pixmap.scaledToWidth(this->width() - 60));
}

void RowRedactor::showPreviousImage()
{
    emit askPreviousImage();
}

void RowRedactor::showNextImage()
{
    emit askNextImage();
}

void RowRedactor::on_cancelPushButton_clicked()
{
    this->close();
}

void RowRedactor::on_acceptPushButton_clicked()
{
    QJsonObject obj;

    for (int i = 0; i < ui->gridLayout->rowCount(); i++) {
        QLabel *label = dynamic_cast <QLabel*> (ui->gridLayout->itemAtPosition(i, 0)->widget());
        QLineEdit *lineEdit = dynamic_cast <QLineEdit*> (ui->gridLayout->itemAtPosition(i, 1)->widget());
        QString key = label->text();
        QString value = lineEdit->text();
        obj.insert(key, value);
    }

    emit sendNewData(obj);

    this->close();
}

