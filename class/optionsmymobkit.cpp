#include "optionsmymobkit.h"
#include "ui_optionsmymobkit.h"

OptionsMyMobKit::OptionsMyMobKit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsMyMobKit)
{
    ui->setupUi(this);
    successful = false;
}

OptionsMyMobKit::~OptionsMyMobKit()
{
    delete ui;
}

void OptionsMyMobKit::setOptions(QString fAddress)
{
    ui->lineEditAdress->setText( fAddress );
}

QString OptionsMyMobKit::getAddress()
{
    return ui->lineEditAdress->text();
}

void OptionsMyMobKit::on_pushButtonSave_clicked()
{
    successful = true;
}

void OptionsMyMobKit::on_pushButtonCancel_clicked()
{
    close();
}
