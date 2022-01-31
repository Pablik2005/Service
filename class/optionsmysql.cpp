#include "optionsmysql.h"
#include "ui_optionsmysql.h"

OptionsMySQL::OptionsMySQL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsMySQL)
{
    ui->setupUi(this);
    this->setWindowTitle("MySQL");


    setWindowTitle("MyMobKit");
    successful = false;
}

OptionsMySQL::~OptionsMySQL()
{
    delete ui;
}

void OptionsMySQL::setOptions(QString fAddress, QString fLogin, QString fPassword, QString fDB)
{
    ui->lineEditHost->setText( fAddress);
    ui->lineEditUser->setText( fLogin );
    ui->lineEditPassword->setText( fPassword );
    ui->lineEditDB->setText( fDB );

}

QString OptionsMySQL::getAddress()
{
    return ui->lineEditHost->text();
}

QString OptionsMySQL::getLogin()
{
    return ui->lineEditUser->text();
}

QString OptionsMySQL::getPassword()
{
    return ui->lineEditPassword->text();
}

QString OptionsMySQL::getDB()
{
    return ui->lineEditDB->text();
}


void OptionsMySQL::on_pushButtonSave_clicked()
{
    successful = true;
    close();
}

void OptionsMySQL::on_pushButtonCancel_clicked()
{
    close();
}
