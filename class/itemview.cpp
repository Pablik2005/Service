#include "itemview.h"
#include "ui_itemview.h"

ItemView::ItemView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemView)
{
    ui->setupUi(this);
    this->setWindowTitle("Przedmiot");
    on_checkBoxWarranty_clicked();

    INIFile iniFile( QCoreApplication::applicationDirPath() + "/config.ini");
    netMargin = iniFile.netMargin;
    grossMargin = iniFile.grossMargin;

    action = "Add";
    successful = false;

    ui->pushButtonBillAdd->setEnabled(false);
    ui->pushButtonBillSave->setEnabled(false);
    ui->pushButtonBillDelete->setEnabled(false);
}

ItemView::~ItemView()
{
    delete ui;
}

void ItemView::editItem(int fItemID, int fOrderID)
{
    action = "Edit";
    itemID = fItemID;
    orderID = fOrderID;
    ui->pushButtonBillAdd->setEnabled(true);

    QSqlQuery queryItems("SELECT * FROM `serviceItems` WHERE `orderID`=? AND `ID`=?");
    queryItems.addBindValue( orderID );
    queryItems.addBindValue( itemID );
    queryItems.exec();

    if(queryItems.size() > 0)
    {
        queryItems.next();

        ui->lineEditName->setText( queryItems.value(2).toString() );
        this->setWindowTitle( queryItems.value(2).toString() );

        QString billName = queryItems.value(3).toString();
        if(!billName.isEmpty())
        {
            ui->labelBillFileName->setText( billName);
            ui->pushButtonBillSave->setEnabled(true);
            ui->pushButtonBillDelete->setEnabled(true);
        }else
        {
            ui->labelBillFileName->setText("Brak");
        }

        QString warranty = queryItems.value(5).toString();
        if(warranty == "")
        {
            ui->checkBoxWarranty->setChecked(false);
            ui->dateEditWarrantyTime->setDisabled(true);
        }else
        {
            QStringList date = warranty.split('-');

            int year = date.at(0).toInt();
            int month = date.at(1).toInt();
            int day = date.at(2).toInt();

            ui->checkBoxWarranty->setChecked(true);
            ui->dateEditWarrantyTime->setDisabled(false);
            ui->dateEditWarrantyTime->setDate(QDate(year, month, day));
        }

        ui->doubleSpinBoxPriceMoxum->setValue( queryItems.value(6).toDouble() );
        ui->doubleSpinBoxPriceNet->setValue( queryItems.value(7).toDouble() );
        ui->doubleSpinBoxPriceGross->setValue( queryItems.value(8).toDouble() );

    }
}

void ItemView::on_pushButtonSave_clicked()
{
    successful = true;

    if(action == "Add")
    {
        QSqlQuery addItem("INSERT INTO `serviceItems`(`orderID`, `name`, `warrantyTo`, `priceMoxum`, `priceNet`, `priceGross`) "
                          "VALUES (?,?,?,?,?,?)");

        addItem.addBindValue( orderID );
        addItem.addBindValue( ui->lineEditName->text() );


        if(ui->checkBoxWarranty->isChecked())
            addItem.addBindValue( ui->dateEditWarrantyTime->date().toString("yyyy-MM-dd") );
        else
            addItem.addBindValue("");


        addItem.addBindValue( ui->doubleSpinBoxPriceMoxum->value() );
        addItem.addBindValue( ui->doubleSpinBoxPriceNet->value() );
        addItem.addBindValue( ui->doubleSpinBoxPriceGross->value() );

        addItem.exec();
    }else if(action == "Edit")
    {
        QSqlQuery editItem("UPDATE `serviceItems` SET `name`=?,`warrantyTo`=?,`priceMoxum`=?,`priceNet`=?,`priceGross`=? "
                           "WHERE `ID`=?");

        editItem.addBindValue( ui->lineEditName->text() );

        if(ui->checkBoxWarranty->isChecked())
            editItem.addBindValue( ui->dateEditWarrantyTime->date().toString("yyyy-MM-dd") );
        else
            editItem.addBindValue("");

        editItem.addBindValue( ui->doubleSpinBoxPriceMoxum->value() );
        editItem.addBindValue( ui->doubleSpinBoxPriceNet->value() );
        editItem.addBindValue( ui->doubleSpinBoxPriceGross->value() );
        editItem.addBindValue( itemID );

        editItem.exec();
    }

    close();
}

void ItemView::on_pushButtonCancel_clicked()
{
    successful = false;
    close();
}

void ItemView::on_checkBoxWarranty_clicked()
{
    if(ui->checkBoxWarranty->checkState() == Qt::Unchecked)
        ui->dateEditWarrantyTime->setDisabled(true);
    else
        ui->dateEditWarrantyTime->setDisabled(false);
}

void ItemView::on_doubleSpinBoxPriceMoxum_valueChanged(double arg1)
{
    double xNetMargin = 100 + netMargin;
    double xGrossMargin = 100 + grossMargin;
    ui->doubleSpinBoxPriceNet->setValue( arg1 / 100 * xNetMargin );
    ui->doubleSpinBoxPriceGross->setValue( arg1 / 100 * xGrossMargin );
}

void ItemView::on_pushButtonBillAdd_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this);

    if(!filePath.isEmpty())
    {
        QByteArray byteArray;
        QFileInfo fileInfo(filePath);
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            byteArray = file.readAll();
            file.close();
        }

        QSqlQuery query("UPDATE `serviceItems` SET `billFileName`=?,`billContent`=? WHERE `ID`=?");
        query.addBindValue( fileInfo.fileName() );
        query.addBindValue( byteArray.toBase64() );
        query.addBindValue( itemID );
        query.exec();

        ui->labelBillFileName->setText( fileInfo.fileName() );

        ui->pushButtonBillDelete->setEnabled(true);
        ui->pushButtonBillSave->setEnabled(true);
    }
}

void ItemView::on_pushButtonBillSave_clicked()
{
    QSqlQuery query("SELECT `billFileName`, `billContent` FROM `serviceItems` WHERE `ID`=?");
    query.addBindValue( itemID );
    query.exec();
    query.next();

    QByteArray byteArray = query.value(1).toByteArray();
    QString filePath = QFileDialog::getSaveFileName(this, tr("Zapisz"), QCoreApplication::applicationDirPath() + "/" + query.value(0).toString(), "*." + query.value(0).toString().split('.').at(1));
    QFile file(filePath);
    if(file.open(QIODevice::ReadWrite))
    {
        file.write( QByteArray::fromBase64(byteArray) );
        file.close();
    }

}

void ItemView::on_pushButtonBillDelete_clicked()
{

    QSqlQuery query("UPDATE `serviceItems` SET `billFileName`=?,`billContent`=? WHERE `ID`=?");
    query.addBindValue( QVariant(QVariant::String) );
    query.addBindValue( QVariant(QVariant::ByteArray) );
    query.addBindValue( itemID );
    query.exec();

    ui->labelBillFileName->setText( "" );

    ui->pushButtonBillDelete->setEnabled(false);
    ui->pushButtonBillSave->setEnabled(false);

}
