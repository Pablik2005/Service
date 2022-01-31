#ifndef SENDSMS_H
#define SENDSMS_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QUrlQuery>
#include "inifile.h"


namespace Ui {
class SendSMS;
}

class SendSMS : public QDialog
{
    Q_OBJECT

public:
    explicit SendSMS(QWidget *parent = 0);
    ~SendSMS();

    QNetworkAccessManager   *nam;
    INIFile                 iniFile;

    void setMobilePhoneNumber(QString fPhoneNumber);
    void setTextMessage(QString fTextMessage);

private slots:
    void on_pushButtonSend_clicked();

private:
    Ui::SendSMS *ui;
};

#endif // SENDSMS_H
