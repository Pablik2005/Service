#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setSortingEnabled(true);
    this->setMinimumWidth(1200);
    this->setWindowIcon(QIcon(":/img/moxum.ico"));
    this->setWindowTitle("Serwis");


    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("UTF-8"));
    iniFile.setIniFile(QCoreApplication::applicationDirPath() + "/config.ini");

    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setHorizontalHeaderItem( 0, new QTableWidgetItem("ID:") );
    ui->tableWidget->setHorizontalHeaderItem( 1, new QTableWidgetItem("PESEL / NIP:") );
    ui->tableWidget->setHorizontalHeaderItem( 2, new QTableWidgetItem("Imie Nazwisko / Firma:") );
    ui->tableWidget->setHorizontalHeaderItem( 3, new QTableWidgetItem("Przyjęto:") );
    ui->tableWidget->setHorizontalHeaderItem( 4, new QTableWidgetItem("Urządzenie:") );
    ui->tableWidget->setHorizontalHeaderItem( 5, new QTableWidgetItem("Producent:") );
    ui->tableWidget->setHorizontalHeaderItem( 6, new QTableWidgetItem("Model:") );
    ui->tableWidget->setHorizontalHeaderItem( 7, new QTableWidgetItem("Koszt [zł]:") );
    ui->tableWidget->setHorizontalHeaderItem( 8, new QTableWidgetItem("Powiadomiono:") );
    ui->tableWidget->setHorizontalHeaderItem( 9, new QTableWidgetItem("Wydano:") );
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->comboBoxNotified->addItem("");
    ui->comboBoxNotified->addItem("Tak");
    ui->comboBoxNotified->addItem("Nie");


    ui->comboBoxEnd->addItem("");
    ui->comboBoxEnd->addItem("Tak");
    ui->comboBoxEnd->addItem("Nie");
    ui->comboBoxEnd->setCurrentIndex(2);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    QSqlQuery query("SELECT orderID, peselNIP, receivedDate, deviceType, deviceProducer, deviceModel, repairCostTotal, notificationDate, endServiceDate FROM orders");
    query.exec();

    ui->tableWidget->setRowCount(query.size());

    int index=0;
    double progressValue = (100.0 / query.size());
    int progressCounter = 1;
    while (query.next())
    {
        //ID
        QTableWidgetItem *orderID = new QTableWidgetItem(query.value("orderID").toString());
        orderID->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,0, orderID);

        QString tPeselNIP = query.value("peselNIP").toString();
        QSqlQuery queryNameCompany("SELECT `isCompany`, `name`, `surName`, `companyName` FROM `clients` WHERE `peselNIP`=?");
        queryNameCompany.addBindValue( tPeselNIP );
        queryNameCompany.exec();
        queryNameCompany.next();

        //PESEL
        if( queryNameCompany.value("isCompany").toBool())
            tPeselNIP = tPeselNIP.left(10);
        else
            tPeselNIP = tPeselNIP.left(11);

        QTableWidgetItem *peselNIP = new QTableWidgetItem(tPeselNIP);
        peselNIP->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,1, peselNIP);

        //Imię i Nazwisko
        QString sNameCompany;
        if( queryNameCompany.value("isCompany").toBool())
            sNameCompany = queryNameCompany.value("companyName").toString();
        else
            sNameCompany = queryNameCompany.value("name").toString() + " " + queryNameCompany.value("surName").toString();

        QTableWidgetItem *nameCompany = new QTableWidgetItem(sNameCompany);
        nameCompany->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,2, nameCompany);

        //Data Przyjęcia
        QTableWidgetItem *recivedDate = new QTableWidgetItem(query.value("receivedDate").toString().replace("T", " "));
        recivedDate->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,3, recivedDate);

        //Urządzenie
        QTableWidgetItem *device = new QTableWidgetItem(query.value("deviceType").toString());
        device->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,4, device);

        //Producent
        QTableWidgetItem *producer = new QTableWidgetItem(query.value("deviceProducer").toString());
        producer->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,5, producer);

        //Model
        QTableWidgetItem *model = new QTableWidgetItem(query.value("deviceModel").toString());
        model->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,6, model);

        //Koszt Całkowity
        QTableWidgetItem *price = new QTableWidgetItem(query.value("repairCostTotal").toString());
        price->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,7, price);

        //Powiadomiono
        QTableWidgetItem *notificationDate = new QTableWidgetItem(query.value("notificationDate").toString().split('T').at(0));
        notificationDate->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,8, notificationDate);

        //Wydano
        QTableWidgetItem *endService = new QTableWidgetItem(query.value("endServiceDate").toString().split('T').at(0));
        endService->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,9, endService);

        ui->progressBar->setValue(progressCounter* progressValue );
        progressCounter++;

        index++;

    }
    filter();
    ui->progressBar->setValue(100);
}

void MainWindow::refreshOrder(int fIndex)
{
    QSqlQuery order("SELECT orderID, peselNIP, receivedDate, deviceType, deviceProducer, deviceModel, repairCostTotal, notificationDate, endServiceDate FROM orders WHERE orderID=?");
    order.addBindValue( ui->tableWidget->item(fIndex, 0)->text() );
    order.exec();
    order.next();

    QString tPeselNIP = order.value("peselNIP").toString();
    QSqlQuery client("SELECT `isCompany`, `name`, `surName`, `companyName` FROM `clients` WHERE `peselNIP`=?");
    client.addBindValue( tPeselNIP );
    client.exec();
    client.next();

    //Pesel /NIP
    ui->tableWidget->item(fIndex, 1)->setText( order.value("peselNIP").toString() );


    //Imię i Nazwisko
    QString sNameCompany;
    if( client.value("isCompany").toBool())
        sNameCompany = client.value("companyName").toString();
    else
        sNameCompany = client.value("name").toString() + " " + client.value("surName").toString();
    ui->tableWidget->item(fIndex, 2)->setText( sNameCompany );

    //Data Przyjęcia
    ui->tableWidget->item(fIndex, 3)->setText( order.value("receivedDate").toString().replace("T", " ") );

    //Urządzenie
    ui->tableWidget->item(fIndex, 4)->setText( order.value("deviceType").toString() );

    //Producent
    ui->tableWidget->item(fIndex, 5)->setText( order.value("deviceProducer").toString() );

    //Model
    ui->tableWidget->item(fIndex, 6)->setText( order.value("deviceModel").toString() );

    //Koszt Całkowity
    ui->tableWidget->item(fIndex, 7)->setText( order.value("repairCostTotal").toString() );

    //Powiadomiono
    ui->tableWidget->item(fIndex, 8)->setText( order.value("notificationDate").toString().split('T').at(0) );

    //Wydano
    ui->tableWidget->item(fIndex, 9)->setText( order.value("endServiceDate").toString().split('T').at(0) );

    filter();
}

void MainWindow::filter()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {

        bool showRow=true;

        //      ID
        if(showRow)
            if(!ui->lineEditID->text().isEmpty())
            {
                if(ui->tableWidget->item(i,0)->text()!= ui->lineEditID->text() )
                    showRow=false;
            }

        //      PESEL/NIP
        if(showRow)
            if(!ui->lineEditPESELNIP->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,1)->text().contains(ui->lineEditPESELNIP->text())))
                    showRow=false;
            }

        //      Imie Nazwisko/Firma
        if(showRow)
            if(!ui->lineEditNemCompany->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,2)->text().contains(ui->lineEditNemCompany->text(),  Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Urządzenie
        if(showRow)
            if(!ui->lineEditDevice->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,4)->text().contains(ui->lineEditDevice->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Producent
        if(showRow)
            if(!ui->lineEditProducer->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,5)->text().contains(ui->lineEditProducer->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Model
        if(showRow)
            if(!ui->lineEditModel->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,6)->text().contains(ui->lineEditModel->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Powiadomiono
        if(showRow)
            if(!ui->comboBoxNotified->currentText().isEmpty())
            {
                if(ui->comboBoxNotified->currentText() == "Tak")
                {
                    if(ui->tableWidget->item(i,8)->text().isEmpty())
                        showRow=false;

                }else
                {
                    if(!(ui->tableWidget->item(i,8)->text().isEmpty()))
                        showRow=false;

                }
            }

        //      Wydano
        if(showRow)
            if(!ui->comboBoxEnd->currentText().isEmpty())
            {
                if(ui->comboBoxEnd->currentText() == "Tak")
                {
                    if(ui->tableWidget->item(i,9)->text().isEmpty())
                        showRow=false;

                }else
                {
                    if(!(ui->tableWidget->item(i,9)->text().isEmpty()))
                        showRow=false;

                }
            }

        if(showRow)
            ui->tableWidget->setRowHidden(i, false);
        else
            ui->tableWidget->setRowHidden(i, true);
    }

}

void MainWindow::on_pushButtonClients_clicked()
{
    Clients clients(this);
    clients.exec();
}

void MainWindow::on_pushButtonAddOrder_clicked()
{
    OrderView orderView(this);
    orderView.addOrdr();
    orderView.exec();

    if(orderView.successful)
    {
        int tRowCout = ui->tableWidget->rowCount();
        tRowCout++;
        ui->tableWidget->setRowCount(tRowCout);

        for(int i=0; i<10; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            ui->tableWidget->setItem(tRowCout-1,i, item);
        }

        ui->tableWidget->item(tRowCout-1, 0)->setText( QString::number(orderView.getOrderID()) );

        refreshOrder(tRowCout-1);
    }

}

void MainWindow::on_pushButtonEditOrder_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if(select->selectedRows().count())
    {
        int index = select->selectedRows().at(0).row();

        OrderView orderView(this);
        orderView.editOrder( ui->tableWidget->item(index, 0)->text().toInt() );
        orderView.exec();

        if(orderView.successful)
            refreshOrder(index);
    }
}

void MainWindow::on_pushButtonDelOrder_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if(select->selectedRows().count())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Usuń", "Czy napewno chcesz usunąć zlecenie? ", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            int index = select->selectedRows().at(0).row();

            QSqlQuery query;
            query.prepare("DELETE FROM `serviceItems` WHERE orderID = ?");
            query.addBindValue( ui->tableWidget->item(index, 0)->text().toInt() );
            query.exec();

            query.prepare("DELETE FROM `serviceServices` WHERE orderID = ?");
            query.addBindValue( ui->tableWidget->item(index, 0)->text().toInt() );
            query.exec();

            query.prepare("DELETE FROM `orders` WHERE orderID = ?");
            query.addBindValue( ui->tableWidget->item(index, 0)->text().toInt() );
            query.exec();

            ui->tableWidget->removeRow( index );
        }
    }
}

void MainWindow::on_pushButtonFiltr_clicked()
{
    if(ui->widgetFilter->isHidden())

        ui->widgetFilter->show();
    else
        ui->widgetFilter->hide();
}

void MainWindow::on_actionMySQL_triggered()
{
    OptionsMySQL optionsMySQL(this);
    optionsMySQL.setOptions(iniFile.mysqlAddres, iniFile.mysqlLogin, iniFile.mysqlPassword, iniFile.mysqlDB);
    optionsMySQL.exec();

    if(optionsMySQL.successful)
    {
        iniFile.setMySQL(optionsMySQL.getAddress(),
                         optionsMySQL.getLogin(),
                         optionsMySQL.getPassword(),
                         optionsMySQL.getDB());
    }
}

void MainWindow::on_pushButtonLogin_clicked()
{

    if (!db.isOpen())
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName( iniFile.mysqlAddres );
        db.setDatabaseName( iniFile.mysqlDB );
        db.setUserName( iniFile.mysqlLogin );
        db.setPassword( iniFile.mysqlPassword );

        if (db.open())
        {
            ui->pushButtonLogin->setText("Wyloguj");
            refresh();
            ui->pushButtonClients->setEnabled(true);
            ui->pushButtonAddOrder->setEnabled(true);
            ui->pushButtonEditOrder->setEnabled(true);
            ui->pushButtonDelOrder->setEnabled(true);
            ui->actionGeneruj->setEnabled(true);
        }else
            QMessageBox::critical(0, QObject::tr("Database Error"), db.lastError().text());

    }else
    {
        db.close();
        ui->pushButtonClients->setEnabled(false);
        ui->pushButtonAddOrder->setEnabled(false);
        ui->pushButtonEditOrder->setEnabled(false);
        ui->pushButtonDelOrder->setEnabled(false);
        ui->actionGeneruj->setEnabled(false);
        ui->pushButtonLogin->setText("Zaloguj");
        ui->tableWidget->setRowCount(0);
        ui->progressBar->setValue(0);
    }
}

void MainWindow::on_lineEditID_textChanged(){ filter(); }
void MainWindow::on_lineEditPESELNIP_textChanged(){ filter(); }
void MainWindow::on_lineEditNemCompany_textChanged(){ filter(); }
void MainWindow::on_lineEditDevice_textChanged(){ filter(); }
void MainWindow::on_lineEditProducer_textChanged(){ filter(); }
void MainWindow::on_lineEditModel_textChanged(){ filter(); }
void MainWindow::on_comboBoxNotified_currentIndexChanged(){ filter(); }
void MainWindow::on_comboBoxEnd_currentIndexChanged(){ filter(); }

void MainWindow::on_actionGeneruj_triggered()
{
    ReportGenerator reportGenerator(this);
    reportGenerator.exec();
}

void MainWindow::on_actionMyMobKit_SMS_triggered()
{
    OptionsMyMobKit optionsMyMobKit(this);
    optionsMyMobKit.setOptions(iniFile.mymobkitAddress);
    optionsMyMobKit.exec();

    if(optionsMyMobKit.successful)
    {
        iniFile.setMyMobKit( optionsMyMobKit.getAddress() );
    }
}
