#include "orderview.h"
#include "ui_orderview.h"

OrderView::OrderView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderView)
{
    ui->setupUi(this);
    this->setWindowTitle("Zlecenie");
    ui->labelTermsFileName->setText("");
    successful = false;
    priceTotalItem = 0;
    priceTotalService = 0;

    ui->dateTimeEditNotification->setDisabled(true);
    ui->dateTimeEditEndService->setDisabled(true);
    ui->pushButtonPrint->setDisabled(true);
}

OrderView::~OrderView()
{
    delete ui;
}

void OrderView::addOrdr()
{
    action = "Add";
    ui->dateTimeEditRecived->setDateTime(QDateTime::currentDateTime());
    ui->tabWidget->widget(2)->setDisabled(true);
    ui->tabWidget->setCurrentWidget( ui->tabWidget->widget(0)  );
    ui->spinBoxComplaint->setDisabled(true);
    ui->pushButtonComplaint->setDisabled(true);

    QSqlQuery orderID("SELECT orderID FROM `orders` ORDER BY orderID DESC");
    orderID.exec();

    if(orderID.size() > 0)
    {
        orderID.next();
        ui->spinBoxID->setValue( orderID.value(0).toInt() + 1 );
        ui->spinBoxID->setMinimum( orderID.value(0).toInt() + 1 );

    }else
        ui->spinBoxID->setValue( 0 );


}

void OrderView::editOrder(int orderID)
{
    action = "Edit";
    editOrderID = orderID;
    ui->checkBoxComplaint->setDisabled(true);
    ui->spinBoxComplaint->setDisabled(true);
    ui->pushButtonComplaint->setDisabled(true);
    ui->pushButtonComplaint->setHidden(true);
    ui->pushButtonPrint->setDisabled(false);

    QSqlQuery order("SELECT * FROM orders WHERE orderID=?");
    order.addBindValue( orderID );
    order.exec();
    order.next();

    QSqlQuery client("SELECT * FROM clients WHERE peselNIP=?");
    client.addBindValue( order.value(1).toString() );
    client.exec();
    client.next();


    ui->spinBoxID->setValue( orderID );
    ui->spinBoxID->setDisabled(true);
    ui->lineEditPESELNIP->setText( client.value(0).toString() );

    if( client.value(1).toInt() == 0 )
    {
        ui->lineEditNameCompany->setText( client.value(2).toString() +" "+ client.value(3).toString() );
        ui->checkBoxIsCompany->setChecked(false);
    }else
    {
        ui->lineEditNameCompany->setText( client.value(4).toString() );
        ui->checkBoxIsCompany->setChecked(true);
    }

    ui->lineEditAddress->setText( client.value(5).toString() );
    ui->lineEditPostOffice->setText( client.value(6).toString() );
    ui->lineEditEmail->setText( client.value(7).toString() );

    QString landLinePhone = client.value(8).toString().mid(0,3) + " " + client.value(8).toString().mid(3,2) + " " + client.value(8).toString().mid(5,3) + " " + client.value(8).toString().mid(8,2) + " " + client.value(8).toString().mid(10,2);
    ui->lineEditLandlinePhone->setText( landLinePhone );


    QString mobilePhone;
    if(client.value(9).toString().length() == 12)
        mobilePhone = client.value(9).toString().mid(0,3) + " " + client.value(9).toString().mid(3,3) + " " + client.value(9).toString().mid(6,3) + " " + client.value(9).toString().mid(9,3);
    else
        mobilePhone = client.value(9).toString().mid(0,3) + " " + client.value(9).toString().mid(3,3) + " " + client.value(9).toString().mid(6,3) + " " + client.value(9).toString().mid(9,2) + " " + client.value(9).toString().mid(11,2);
    ui->lineEditMobilePhone->setText( mobilePhone );


    QString sDate = order.value(2).toString().split('T').at(0);
    QString sTime = order.value(2).toString().split('T').at(1);
    int year = sDate.split('-').at(0).toInt();
    int month = sDate.split('-').at(1).toInt();
    int day = sDate.split('-').at(2).toInt();
    int hour = sTime.split(':').at(0).toInt();
    int minute = sTime.split(':').at(1).toInt();
    int second = sTime.split(':').at(2).toInt();
    ui->dateTimeEditRecived->setDate( QDate(year, month, day) );
    ui->dateTimeEditRecived->setTime(QTime(hour, minute, second));

    if(order.value(3).toInt())
        ui->checkBoxInvoice->setChecked(true);

    ui->lineEditDevice->setText( order.value(4).toString() );
    ui->lineEditProducer->setText( order.value(5).toString() );
    ui->lineEditModel->setText( order.value(6).toString() );
    ui->lineEditSN->setText( order.value(7).toString() );
    ui->plainTextEditAdditionalEquipment->setPlainText(order.value(8).toString());
    ui->plainTextEditDescription->setPlainText(order.value(9).toString());

    if( !order.value(10).isNull() )
    {
        ui->checkBoxComplaint->setChecked(true);
        ui->spinBoxComplaint->setValue( order.value(10).toInt() );
        ui->spinBoxComplaint->setEnabled(false);
    }else
    {
        ui->checkBoxComplaint->setHidden(true);
        ui->spinBoxComplaint->setHidden(true);
    }


    if(!order.value(11).isNull())
    {
        ui->pushButtonTermsFileSave->setEnabled(true);
        ui->labelTermsFileName->setText( order.value(11).toString() );
    }


    ui->doubleSpinBoxRepairCostService->setValue( order.value(13).toDouble() );
    ui->doubleSpinBoxRepairTotalCost->setValue( order.value(14).toDouble() );

    if(!order.value(15).isNull())
    {
        ui->checkBoxNotification->setChecked(true);
        ui->dateTimeEditNotification->setDateTime( order.value(15).toDateTime()  );

    }else
        ui->checkBoxNotification->setChecked(false);

    if(!order.value(17).isNull())
    {
        ui->checkBoxEndService->setChecked(true);
        ui->dateTimeEditEndService->setDateTime( order.value(17).toDateTime() );

    }else
        ui->checkBoxEndService->setChecked(false);


    ui->lineEditPassword->setText( order.value(18).toString() );
    ui->lineEditWarrantySeal->setText( order.value(19).toString() );

    refreshItemsList();
    refreshServicesList();

    ui->tabWidget->setCurrentIndex(0);
}

void OrderView::refreshItemsList()
{

    items.clear();
    ui->listWidgetRepairItems->clear();
    priceTotalItem = 0;
    double tPriceNet = 0;
    double tPriceGross = 0;


    QSqlQuery queryItems("SELECT * FROM `serviceItems` WHERE `orderID`=?");
    queryItems.addBindValue( editOrderID );
    queryItems.exec();

    if(queryItems.size() > 0)
    {
        while (queryItems.next())
        {
            ServiceItem item;
            item.id = queryItems.value(0).toInt();
            item.name = queryItems.value(2).toString();
            item.priceMoxum = queryItems.value(6).toDouble();
            item.priceNet = queryItems.value(7).toDouble();
            item.priceGross = queryItems.value(8).toDouble();

            items.append(item);
        }
    }

    foreach(ServiceItem i, items)
    {
        if(ui->checkBoxInvoice->isChecked())
        {
            ui->listWidgetRepairItems->addItem(i.name +" ("+ QString::number(i.priceMoxum) +" - " + QString::number(i.priceGross) +" zł)" );
            priceTotalItem += i.priceGross;
            tPriceGross += i.priceGross;
        }else
        {
            ui->listWidgetRepairItems->addItem(i.name +" ("+ QString::number(i.priceMoxum) +" - " + QString::number(i.priceNet) +" zł)" );;
            priceTotalItem += i.priceNet;
            tPriceNet += i.priceNet;
        }

    }

     if(ui->checkBoxInvoice->isChecked())
         ui->labelRepairCostService->setText(  "Koszt MOXUM ["+ QString::number(tPriceGross) +" zł] :");
     else
         ui->labelRepairCostService->setText(  "Koszt MOXUM ["+ QString::number(tPriceNet) +" zł] :");

    ui->labelRepairCostTotal->setText( "Koszt Całkowity ["+ QString::number(priceTotalItem + priceTotalService) +" zł] :" );
}

void OrderView::refreshServicesList()
{

    services.clear();
    ui->listWidgetRepairServices->clear();
    priceTotalService = 0;

    QSqlQuery queryServices("SELECT * FROM `serviceServices` WHERE `orderID`=?");
    queryServices.addBindValue( editOrderID );
    queryServices.exec();

    if(queryServices.size() > 0)
    {
        while (queryServices.next())
        {
            ServiceItem service;
            service.id = queryServices.value(0).toInt();
            service.name = queryServices.value(2).toString();
            service.priceNet = queryServices.value(3).toDouble();
            service.priceGross = queryServices.value(4).toDouble();

            services.append(service);
        }
    }

    foreach(ServiceItem i, services)
    {
        if(ui->checkBoxInvoice->isChecked())
        {
            ui->listWidgetRepairServices->addItem(i.name +" ( " + QString::number(i.priceGross) +" zł)" );
            priceTotalService += i.priceGross;
        }else
        {
            ui->listWidgetRepairServices->addItem(i.name +" ( " + QString::number(i.priceNet) +" zł)" );;
            priceTotalService += i.priceNet;
        }

    }

    ui->labelRepairCostTotal->setText( "Koszt Całkowity ["+ QString::number(priceTotalItem + priceTotalService) +" zł] :" );

}

int OrderView::getOrderID()
{
    return ui->spinBoxID->value();
}

void OrderView::on_pushButtonSearchClient_clicked()
{
    Clients clients(this);
    clients.setSelectButtonEnable(true);
    clients.exec();

    if(clients.successful)
    {
        ui->lineEditPESELNIP->setText( clients.selectedPESELNIP );
        ui->lineEditNameCompany->setText( clients.selectedNameCompany );
        ui->lineEditAddress->setText( clients.selectedAddress );
        ui->lineEditPostOffice->setText( clients.selectedPostOffice );
        ui->lineEditEmail->setText( clients.selectedEmail );
        ui->lineEditLandlinePhone->setText( clients.selectedLandlinePhone );
        ui->lineEditMobilePhone->setText( clients.selectedMobilePhone );

        if( clients.selectedIsCompany )
            ui->checkBoxIsCompany->setChecked(true);
        else
            ui->checkBoxIsCompany->setChecked(false);
    }
}

void OrderView::on_pushButtonSave_clicked()
{
    successful = true;
    QSqlQuery query;

    if(action == "Add")
    {
        if(!ui->lineEditPESELNIP->text().isEmpty())
        {
            query.prepare("INSERT INTO `orders`(`orderID`, `peselNIP`, `receivedDate`, `invoice`, `deviceType`, `deviceProducer`, `deviceModel`, `deviceSN`, `additionalEquipment`, `description`, `complaint`, `password`, `warrantySeal`) "
                          "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");

            query.addBindValue( ui->spinBoxID->value() );
            query.addBindValue( ui->lineEditPESELNIP->text() );
            query.addBindValue( ui->dateTimeEditRecived->dateTime().toString("yyyy-MM-dd hh:mm:ss") );

            bool invoice;
            if(Qt::Unchecked == ui->checkBoxInvoice->checkState())
                invoice = false;
            else if(Qt::Checked == ui->checkBoxInvoice->checkState())
                invoice = true;
            query.addBindValue( invoice );

            query.addBindValue( ui->lineEditDevice->text() );
            query.addBindValue( ui->lineEditProducer->text() );
            query.addBindValue( ui->lineEditModel->text() );
            query.addBindValue( ui->lineEditSN->text() );
            query.addBindValue( ui->plainTextEditAdditionalEquipment->toPlainText() );
            query.addBindValue( ui->plainTextEditDescription->toPlainText() );

            if(ui->checkBoxComplaint->isChecked())
                query.addBindValue( ui->spinBoxComplaint->value() );
            else
                query.addBindValue( QVariant(QVariant::Int) );


            query.addBindValue( ui->lineEditPassword->text() );
            query.addBindValue( ui->lineEditWarrantySeal->text() );

            query.exec();

            close();
        }else
            QMessageBox::information(this, "Error", "Brak powiązanego klienta");


    }else if(action == "Edit")
    {
        //,`notificationDate`=?,`notificationBy`=?,`endServiceDate`=?

        query.prepare("UPDATE `orders` SET `peselNIP`=?,`receivedDate`=?,`invoice`=?,`deviceType`=?,`deviceProducer`=?,`deviceModel`=?,`deviceSN`=?,`additionalEquipment`=?,`description`=?,`repairCostService`=?,`repairCostTotal`=? ,`notificationDate`=?,`endServiceDate`=?, `password`=?, `warrantySeal`=? "
                      "WHERE `orderID`=?");

        query.addBindValue( ui->lineEditPESELNIP->text() );
        query.addBindValue( ui->dateTimeEditRecived->dateTime().toString("yyyy-MM-dd hh:mm:ss") );

        int invoice;
        if(ui->checkBoxInvoice->isChecked())
            invoice = 1;
        else
            invoice = 0;
        query.addBindValue( invoice );

        query.addBindValue( ui->lineEditDevice->text() );
        query.addBindValue( ui->lineEditProducer->text() );
        query.addBindValue( ui->lineEditModel->text() );
        query.addBindValue( ui->lineEditSN->text() );
        query.addBindValue( ui->plainTextEditAdditionalEquipment->toPlainText() );
        query.addBindValue( ui->plainTextEditDescription->toPlainText() );
        query.addBindValue( ui->doubleSpinBoxRepairCostService->value() );
        query.addBindValue( ui->doubleSpinBoxRepairTotalCost->value() );

        if(ui->checkBoxNotification->isChecked())
            query.addBindValue( ui->dateTimeEditNotification->dateTime().toString("yyyy-MM-dd hh:mm:ss") );
        else
            query.addBindValue( QVariant(QVariant::DateTime));

        if(ui->checkBoxEndService->isChecked())
            query.addBindValue( ui->dateTimeEditEndService->dateTime().toString("yyyy-MM-dd hh:mm:ss") );
        else
            query.addBindValue( QVariant(QVariant::DateTime));

        query.addBindValue( ui->lineEditPassword->text() );
        query.addBindValue( ui->lineEditWarrantySeal->text() );

        query.addBindValue( editOrderID );
        query.exec();

        close();

    }
}

void OrderView::on_pushButtonCancel_clicked()
{
    successful = false;
    close();
}

void OrderView::on_pushButtonPrint_clicked()
{

    INIFile iniFile( QCoreApplication::applicationDirPath() + "/config.ini");

    QUrl url("http://" + iniFile.mysqlAddres + "/order.php?id=" + QString::number(editOrderID));
    QDesktopServices::openUrl(url);
}

void OrderView::on_pushButtonAddItem_clicked()
{
    ItemView itemView(this);
    itemView.orderID = editOrderID;
    itemView.exec();

    if(itemView.successful)
        refreshItemsList();

}

void OrderView::on_pushButtonRemoveItem_clicked()
{
    int index = ui->listWidgetRepairItems->currentIndex().row();
    if(index >= 0)
    {
        QSqlQuery queryItem("DELETE FROM `serviceItems` WHERE `ID`=?");
        queryItem.addBindValue( items.at(index).id );
        queryItem.exec();

        refreshItemsList();
    }

}

void OrderView::on_pushButtonAddService_clicked()
{
    ServiceView serviceView(this);
    serviceView.orderID = editOrderID;
    serviceView.exec();

    if(serviceView.successful)
        refreshServicesList();
}

void OrderView::on_pushButtonRemoveService_clicked()
{
    int index = ui->listWidgetRepairServices->currentIndex().row();
    if(index >= 0)
    {
        QSqlQuery queryService("DELETE FROM `serviceServices` WHERE `ID`=?");
        queryService.addBindValue( services.at(index).id );
        queryService.exec();

        refreshServicesList();
    }

}

void OrderView::on_listWidgetRepairItems_doubleClicked(const QModelIndex &index)
{
    ItemView itemView(this);
    itemView.orderID = editOrderID;
    itemView.editItem(items.at(index.row()).id , editOrderID);
    itemView.exec();

    if(itemView.successful)
        refreshItemsList();
}

void OrderView::on_checkBoxInvoice_stateChanged(int arg1)
{
    refreshItemsList();
    refreshServicesList();
}

void OrderView::on_checkBoxComplaint_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->spinBoxComplaint->setEnabled(true);
        ui->pushButtonComplaint->setEnabled(true);
    }else
    {
        ui->spinBoxComplaint->setDisabled(true);
        ui->pushButtonComplaint->setDisabled(true);
    }
}

void OrderView::on_pushButtonComplaint_clicked()
{

    QSqlQuery order("SELECT * FROM orders WHERE orderID=?");
    order.addBindValue( ui->spinBoxComplaint->value() );
    order.exec();
    order.next();

    QSqlQuery client("SELECT * FROM clients WHERE peselNIP=?");
    client.addBindValue( order.value(1).toString() );
    client.exec();
    client.next();

    ui->lineEditPESELNIP->setText( client.value(0).toString() );

    if( client.value(1).toInt() == 0 )
    {
        ui->lineEditNameCompany->setText( client.value(2).toString() +" "+ client.value(3).toString() );
        ui->checkBoxIsCompany->setChecked(false);
    }else
    {
        ui->lineEditNameCompany->setText( client.value(4).toString() );
        ui->checkBoxIsCompany->setChecked(true);
    }

    ui->lineEditAddress->setText( client.value(5).toString() );
    ui->lineEditPostOffice->setText( client.value(6).toString() );
    ui->lineEditEmail->setText( client.value(7).toString() );
    ui->lineEditLandlinePhone->setText( client.value(8).toString() );
    ui->lineEditMobilePhone->setText( client.value(9).toString() );

    if(order.value(3).toInt())
        ui->checkBoxInvoice->setChecked(true);

    ui->lineEditDevice->setText( order.value(4).toString() );
    ui->lineEditProducer->setText( order.value(5).toString() );
    ui->lineEditModel->setText( order.value(6).toString() );
    ui->lineEditSN->setText( order.value(7).toString() );
    ui->lineEditPassword->setText( order.value(18).toString() );


}

void OrderView::on_listWidgetRepairServices_doubleClicked(const QModelIndex &index)
{
    ServiceView serviceView(this);
    serviceView.orderID = editOrderID;
    serviceView.editService(services.at(index.row()).id , editOrderID);
    serviceView.exec();

    if(serviceView.successful)
        refreshServicesList();
}

void OrderView::on_checkBoxNotification_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->dateTimeEditNotification->setEnabled(true);
        ui->dateTimeEditNotification->setDateTime(QDateTime::currentDateTime());
    }else
        ui->dateTimeEditNotification->setEnabled(false);
}

void OrderView::on_checkBoxEndService_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->dateTimeEditEndService->setEnabled(true);
        ui->dateTimeEditEndService->setDateTime(QDateTime::currentDateTime());
    }else
        ui->dateTimeEditEndService->setEnabled(false);
}

void OrderView::on_pushButtonAddTermsService_clicked()
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

        QSqlQuery query("UPDATE `orders` SET `termsServiceFileName`=?,`termsServiceContent`=? WHERE `orderID`=?");
        query.addBindValue( fileInfo.fileName() );
        query.addBindValue( byteArray.toBase64() );
        query.addBindValue( editOrderID );
        query.exec();

        ui->labelTermsFileName->setText( fileInfo.fileName() );
    }
}

void OrderView::on_pushButtonTermsFileSave_clicked()
{
    QSqlQuery query("SELECT `termsServiceFileName`, `termsServiceContent` FROM `orders` WHERE `orderID`=?");
    query.addBindValue( editOrderID );
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



void OrderView::on_pushButton_clicked()
{
    if(ui->lineEditMobilePhone->text().leftRef(3) == "+48")
    {
        SendSMS sendSMS(this);

        QString tPhoneNumber = ui->lineEditMobilePhone->text().remove(0,3);
        tPhoneNumber = tPhoneNumber.remove(" ");

        sendSMS.setMobilePhoneNumber(tPhoneNumber);
        sendSMS.setTextMessage("Witamy tu MOXUM.\n"
                               "Chcieli byśmy poinformować Państwa, że urządzenie zostawione u nas do naprawy jest gotowe do odbioru.\n"
                               "W razie jakichś pytań proszę się kontaktować na nasz nr sklepowy 377-40-88.");

        sendSMS.exec();
    }else
    {
        QMessageBox::information(this,"Error","Nr telefonu nie jest z polski");
    }
}
