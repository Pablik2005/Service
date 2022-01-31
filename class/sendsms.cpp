#include "sendsms.h"
#include "ui_sendsms.h"

SendSMS::SendSMS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendSMS)
{
    ui->setupUi(this);

    setWindowTitle("SMS");
    nam = new QNetworkAccessManager(this);
    iniFile.setIniFile(QCoreApplication::applicationDirPath() + "/config.ini");
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(close()));
}

SendSMS::~SendSMS()
{
    delete ui;
}

void SendSMS::setMobilePhoneNumber(QString fPhoneNumber)
{
    ui->lineEditSendTo->setText(fPhoneNumber);
}

void SendSMS::setTextMessage(QString fTextMessage)
{
    ui->plainTextEdit->setPlainText(fTextMessage);
}

void SendSMS::on_pushButtonSend_clicked()
{
    QUrlQuery postData;
    postData.addQueryItem("To", ui->lineEditSendTo->text());
    postData.addQueryItem("Message", ui->plainTextEdit->toPlainText());


    QNetworkRequest request(QUrl("http://"+ iniFile.mymobkitAddress +":1688/services/api/messaging/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    nam->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    ui->pushButtonSend->hide();
}
