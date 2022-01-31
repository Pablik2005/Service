#include "clients.h"
#include "ui_clients.h"

Clients::Clients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Clients)
{
    ui->setupUi(this);
    this->setWindowTitle("Kontrahenci");
    ui->tableWidget->setSortingEnabled(true);

    successful = false;
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderItem( 0, new QTableWidgetItem("PESEL / NIP:") );
    ui->tableWidget->setHorizontalHeaderItem( 1, new QTableWidgetItem("Imię Nazwisko / Firma:") );
    ui->tableWidget->setHorizontalHeaderItem( 2, new QTableWidgetItem("Adres") );
    ui->tableWidget->setHorizontalHeaderItem( 3, new QTableWidgetItem("Poczta:") );
    ui->tableWidget->setHorizontalHeaderItem( 4, new QTableWidgetItem("Email:") );
    ui->tableWidget->setHorizontalHeaderItem( 5, new QTableWidgetItem("Tel. Stacjonarny:") );
    ui->tableWidget->setHorizontalHeaderItem( 6, new QTableWidgetItem("Tel. Komórkowy:") );
    ui->tableWidget->setHorizontalHeaderItem( 7, new QTableWidgetItem("Firma:") );
    ui->tableWidget->setHorizontalHeaderItem( 8, new QTableWidgetItem("Rejstacja:") );
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    refresh();

    ui->comboBoxCompany->addItem("");
    ui->comboBoxCompany->addItem("Tak");
    ui->comboBoxCompany->addItem("Nie");
    ui->dateEditRegistrationTo->setDate(QDate::currentDate());
}

Clients::~Clients()
{
    delete ui;
}

void Clients::refresh()
{
    QSqlQuery query("select * from clients");
    query.exec();

    ui->tableWidget->setRowCount(query.size());

    ui->progressBar->setValue(0);
    double progressValue = (100.0 / query.size());
    int progressCounter = 1;

    listName.clear();
    listSurName.clear();
    listPostOffice.clear();
    listAddress.clear();

    int index=0;
    while (query.next())
    {
        //~PESEL - NIP~
        QTableWidgetItem *item0 = new QTableWidgetItem(query.value("peselNIP").toString());
        item0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,0, item0);            

        //~Imię Nazwisko - Firma~
         QTableWidgetItem *item1;
        if(query.value("isCompany").toBool())
            item1 = new QTableWidgetItem(query.value("companyName").toString());
        else
        {
            item1 = new QTableWidgetItem(query.value("name").toString() +" "+ query.value("surName").toString());

            if(!listName.contains( query.value("name").toString() ))
                listName.append( query.value("name").toString() );

            if(!listSurName.contains( query.value("surName").toString() ))
                listSurName.append( query.value("surName").toString() );
        }
        item1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,1, item1);

        //Adres
        QTableWidgetItem *item2 = new QTableWidgetItem(query.value("address").toString());
        item2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,2, item2);

        QString tAddress = prepareAddresToCompleter( query.value("address").toString() );
        if(!listAddress.contains( tAddress ))
            listAddress.append( tAddress );

        //Poczta
        QTableWidgetItem *item3 = new QTableWidgetItem(query.value("postOffice").toString());
        item3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,3, item3);

        if(!listPostOffice.contains( query.value("postOffice").toString() ))
            listPostOffice.append( query.value("postOffice").toString() );

        //Email
        QTableWidgetItem *item4 = new QTableWidgetItem(query.value("email").toString());
        item4->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,4, item4);

        //Tel Stacjonarny
        QString landLinePhone = query.value("landlinePhone").toString().mid(0,3) + " " + query.value("landlinePhone").toString().mid(3,2) + " " + query.value("landlinePhone").toString().mid(5,3) + " " + query.value("landlinePhone").toString().mid(8,2) + " " + query.value("landlinePhone").toString().mid(10,2);
        QTableWidgetItem *item5 = new QTableWidgetItem(landLinePhone);
        item5->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,5, item5);

        //Tel Komórkowy
        QString mobilePhoneQueryValue = query.value("mobilePhone").toString();
        QString mobilePhone;
        if(mobilePhoneQueryValue.length() == 12)
            mobilePhone = mobilePhoneQueryValue.mid(0,3) + " " + mobilePhoneQueryValue.mid(3,3) + " " + mobilePhoneQueryValue.mid(6,3) + " " + mobilePhoneQueryValue.mid(9,3);
        else if(mobilePhoneQueryValue.length() == 13)
            mobilePhone = mobilePhoneQueryValue.mid(0,3) + " " + mobilePhoneQueryValue.mid(3,3) + " " + mobilePhoneQueryValue.mid(6,3) + " " + mobilePhoneQueryValue.mid(9,2) + " " + mobilePhoneQueryValue.mid(11,2);
        QTableWidgetItem *item6 = new QTableWidgetItem(mobilePhone);
        item6->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,6, item6);

        //Firma
        QString isCompany;
        if(query.value("isCompany").toBool())
            isCompany = "Tak";
        else
            isCompany = "Nie";
        QTableWidgetItem *item7 = new QTableWidgetItem(isCompany);
        item7->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,7, item7);

        //Rejstracja
        QTableWidgetItem *item8 = new QTableWidgetItem(query.value("registerDate").toString());
        item8->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->tableWidget->setItem(index,8, item8);

        ui->progressBar->setValue(progressCounter* progressValue );
        progressCounter++;

        index++;
    }

    filter();
    ui->progressBar->setValue(100);
}

void Clients::refreshClient(int fIndex)
{
    QString peselNIP = ui->tableWidget->item(fIndex,0)->text();

    QSqlQuery client("SELECT * FROM `clients` WHERE `peselNIP`=?");
    client.addBindValue( peselNIP );
    client.exec();
    client.next();

    if(client.value("isCompany").toBool())
    {
       ui->tableWidget->item(fIndex, 1)->setText( client.value("companyName").toString()  );
       ui->tableWidget->item(fIndex, 7)->setText( "Tak" );

    }else
    {
        ui->tableWidget->item(fIndex, 1)->setText( client.value("name").toString() +" "+ client.value("surName").toString()  );
        ui->tableWidget->item(fIndex, 7)->setText( "Nie" );
    }

    ui->tableWidget->item(fIndex, 2)->setText(client.value("address").toString());
    ui->tableWidget->item(fIndex, 3)->setText(client.value("postOffice").toString());

    if(!client.value("email").toString().isEmpty())
        ui->tableWidget->item(fIndex, 4)->setText(client.value("email").toString());

    if(!client.value("landlinePhone").toString().isEmpty())
        ui->tableWidget->item(fIndex, 5)->setText(client.value("landlinePhone").toString());

    if(!client.value("mobilePhone").toString().isEmpty())
        ui->tableWidget->item(fIndex, 6)->setText(client.value("mobilePhone").toString());

    ui->tableWidget->item(fIndex, 8)->setText(client.value("registerDate").toString());



}

void Clients::setSelectButtonEnable(bool fBool)
{
    ui->pushButtonSelect->setEnabled(fBool);
}

QString Clients::prepareAddresToCompleter(QString fAddress)
{
    QStringList words = fAddress.split(" ");

    if(words.count() > 1)
    {
        QString lastWord = words.last();

        bool dontHaveDigit = true;
        foreach(QChar c, lastWord)
        {
            if(c.isDigit())
            {
                dontHaveDigit = false;
                break;
            }
        }

        QString tAddress = words.join(" ");


        if(dontHaveDigit)
            return tAddress;
        else
            return tAddress.left( tAddress.length() - (lastWord.length() +1));

    }else
        return fAddress;
}

void Clients::filter()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {

        bool showRow=true;

        //      PESEL/NIP
        if(showRow)
            if(!ui->lineEditPESEL->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,0)->text().contains(ui->lineEditPESEL->text())))
                    showRow=false;
            }

        //      Imie Nazwisko/Firma
        if(showRow)
            if(!ui->lineEditName->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,1)->text().contains(ui->lineEditName->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Address
        if(showRow)
            if(!ui->lineEditAddress->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,2)->text().contains(ui->lineEditAddress->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Poczta
        if(showRow)
            if(!ui->lineEditPostOffice->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,3)->text().contains(ui->lineEditPostOffice->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Email
        if(showRow)
            if(!ui->lineEditEmail->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,4)->text().contains(ui->lineEditEmail->text(), Qt::CaseInsensitive)))
                    showRow=false;
            }

        //      Tel. Stacjonarny
        if(showRow)
            if(!ui->lineEditLandlinePhone->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,5)->text().contains(ui->lineEditLandlinePhone->text())))
                    showRow=false;
            }

        //      Tel. Komórkowy
        if(showRow)
            if(!ui->lineEditMobilePhone->text().isEmpty())
            {
                if(!(ui->tableWidget->item(i,6)->text().contains(ui->lineEditMobilePhone->text())))
                    showRow=false;
            }

        //      Firma
        if(showRow)
            if(!ui->comboBoxCompany->currentText().isEmpty())
            {
                if(ui->comboBoxCompany->currentText() == "Tak")
                {
                    if(!(ui->tableWidget->item(i,7)->text().contains("Tak")))
                        showRow=false;

                }else
                {
                    if(!(ui->tableWidget->item(i,7)->text().contains("Nie")))
                        showRow=false;

                }
            }

        //      Data Rejstracji
        if(showRow)
        {
            QDate tDate;
            tDate.setDate( ui->tableWidget->item(i,8)->text().mid(0,4).toInt(),
                           ui->tableWidget->item(i,8)->text().mid(5,2).toInt(),
                           ui->tableWidget->item(i,8)->text().mid(8,2).toInt()
                         );
            if(!(tDate >= ui->dateEditRegistrationSince->date() && tDate <= ui->dateEditRegistrationTo->date()))
                showRow=false;
        }


        if(showRow)
            ui->tableWidget->setRowHidden(i, false);
        else
            ui->tableWidget->setRowHidden(i, true);
    }
}

void Clients::on_pushButtonAddClient_clicked()
{
    ClientView clientView(this);
    clientView.action = "Add";
    clientView.setNameCompleter( listName );
    clientView.setSurNameCompleter( listSurName );
    clientView.setPostOfficeCompleter( listPostOffice );
    clientView.setAddressCompleter( listAddress );
    clientView.exec();

    if(clientView.successful)
    {
        int tRowCount = ui->tableWidget->rowCount();
        tRowCount++;
        ui->tableWidget->setRowCount(tRowCount);

        for(int i=0; i<9; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->tableWidget->setItem(tRowCount-1,i, item);
        }

        if(clientView.getIsCompany())
            ui->tableWidget->item(tRowCount-1, 0)->setText( clientView.getNIP() );
        else
            ui->tableWidget->item(tRowCount-1, 0)->setText( clientView.getPESEL() );

        refreshClient(tRowCount-1);
    }
}

void Clients::on_pushButtonDelClient_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if(select->selectedRows().count() == 1)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Usuń", "Czy napewno chcesz usunąć clienta? ", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            int index = select->selectedRows().at(0).row();
            QString peselNIP = ui->tableWidget->item(index, 0)->text();

            QSqlQuery query;
            query.prepare("SELECT `orderID` FROM `orders` WHERE `peselClient`=?");
            query.addBindValue( peselNIP );
            query.exec();
            if(query.size() > 0)
            {
                while(query.next())
                {
                    QSqlQuery items("DELETE FROM `serviceItems` WHERE `orderID`=?");
                    items.addBindValue( query.value(0).toString() );
                    items.exec();

                    QSqlQuery services("DELETE FROM `serviceServices` WHERE `orderID`=?");
                    services.addBindValue( query.value(0).toString() );
                    services.exec();
                }
            }

            query.prepare("DELETE FROM `orders` WHERE peselClient = ?");
            query.addBindValue( peselNIP );
            query.exec();

            query.prepare("DELETE FROM `clients` WHERE peselNIP = ?");
            query.addBindValue( peselNIP );
            query.exec();

            ui->tableWidget->removeRow(index);
        }
    }
}

void Clients::on_pushButtonEditClient_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    if(select->selectedRows().count())
    {
        int index = select->selectedRows().at(0).row();

        ClientView clientView(this);
        clientView.setEditUserCompany( ui->tableWidget->item(index, 0)->text() );
        clientView.setNameCompleter( listName );
        clientView.setSurNameCompleter( listSurName );
        clientView.setPostOfficeCompleter( listPostOffice );
        clientView.setAddressCompleter( listAddress );
        clientView.exec();

        if(clientView.successful)
        {
            refreshClient(index);
        }
    }
}

void Clients::on_pushButtonSelect_clicked()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();
    if(select->selectedRows().count())
    {
        int index = select->selectedRows().at(0).row();

        selectedPESELNIP = ui->tableWidget->item(index, 0)->text();
        selectedNameCompany = ui->tableWidget->item(index, 1)->text();
        selectedAddress = ui->tableWidget->item(index, 2)->text();
        selectedPostOffice = ui->tableWidget->item(index, 3)->text();
        selectedEmail = ui->tableWidget->item(index, 4)->text();
        selectedLandlinePhone = ui->tableWidget->item(index, 5)->text();
        selectedMobilePhone = ui->tableWidget->item(index, 6)->text();

        if(ui->tableWidget->item(index, 7)->text() == "Tak")
            selectedIsCompany = true;
        else
            selectedIsCompany = false;

        successful = true;
        close();
    }
}



void Clients::on_lineEditPESEL_textChanged(){ filter();}
void Clients::on_lineEditName_textChanged(){ filter();}
void Clients::on_lineEditAddress_textChanged(){ filter();}
void Clients::on_lineEditPostOffice_textChanged(){ filter();}
void Clients::on_lineEditEmail_textChanged(){ filter();}
void Clients::on_lineEditLandlinePhone_textChanged(){ filter();}
void Clients::on_lineEditMobilePhone_textChanged(){ filter();}
void Clients::on_comboBoxCompany_currentIndexChanged(){ filter();}
void Clients::on_dateEditRegistrationSince_userDateChanged(){ filter();}
void Clients::on_dateEditRegistrationTo_userDateChanged(){ filter();}
