#include "serviceview.h"
#include "ui_serviceview.h"

ServiceView::ServiceView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceView)
{
    ui->setupUi(this);
    this->setWindowTitle("Usługa");

    INIFile iniFile( QCoreApplication::applicationDirPath() + "/config.ini");
    netMargin = iniFile.netMargin;
    grossMargin = iniFile.grossMargin;

    action = "Add";
    successful = false;
}

ServiceView::~ServiceView()
{
    delete ui;
}

void ServiceView::editService(int fServiceID, int fOrderID)
{
    action = "Edit";
    serviceID = fServiceID;
    orderID = fOrderID;
    QSqlQuery queryService("SELECT * FROM `serviceServices` WHERE `orderID`=? AND `ID`=?");
    queryService.addBindValue( orderID );
    queryService.addBindValue( serviceID );
    queryService.exec();

    if(queryService.size() > 0)
    {
        queryService.next();

        ui->lineEditName->setText( queryService.value(2).toString() );
        this->setWindowTitle( queryService.value(2).toString() );
        ui->doubleSpinBoxPriceNet->setValue( queryService.value(3).toDouble() );
        ui->doubleSpinBoxPriceGross->setValue( queryService.value(4).toDouble() );
    }
}

void ServiceView::on_pushButtonSave_clicked()
{

    successful = true;

    if(action == "Add")
    {
        QSqlQuery addService("INSERT INTO `serviceServices`(`orderID`, `name`, `priceNet`, `priceGross`) "
                             "VALUES (?,?,?,?)");

        addService.addBindValue( orderID );
        addService.addBindValue( ui->lineEditName->text() );
        addService.addBindValue( ui->doubleSpinBoxPriceNet->value() );
        addService.addBindValue( ui->doubleSpinBoxPriceGross->value() );

        addService.exec();

    }else if(action == "Edit")
    {
        QSqlQuery editItem("UPDATE `serviceServices` SET `name`=?,`priceNet`=?,`priceGross`=? "
                           "WHERE `ID`=?");

        editItem.addBindValue( ui->lineEditName->text() );
        editItem.addBindValue( ui->doubleSpinBoxPriceNet->value() );
        editItem.addBindValue( ui->doubleSpinBoxPriceGross->value() );
        editItem.addBindValue( serviceID );
        editItem.exec();
    }

    close();

}

void ServiceView::on_pushButtonCancel_clicked()
{
    successful = false;
    close();
}

void ServiceView::on_doubleSpinBoxPriceNet_valueChanged(double arg1)
{
    double xGrossMargin = 100 + grossMargin;
    ui->doubleSpinBoxPriceGross->setValue( arg1 / 100 * xGrossMargin );

}
