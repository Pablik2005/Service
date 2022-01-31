#include "clientview.h"
#include "ui_clientview.h"

ClientView::ClientView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientView)
{
    ui->setupUi(this);
    this->setWindowTitle("Klient");
    successful = false;

    ui->checkBoxIsCompany->setCheckState(Qt::Unchecked);
    ui->lineEditNIP->setDisabled(true);
    ui->lineEditCompanyName->setDisabled(true);

}

ClientView::~ClientView()
{
    delete ui;
}

bool ClientView::getIsCompany()
{
    if(ui->checkBoxIsCompany->isChecked())
        return true;
    else
        return false;
}

QString ClientView::getPESEL()
{
    return ui->lineEditPESEL->text();
}

QString ClientView::getNIP()
{
    return ui->lineEditNIP->text();
}

void ClientView::setEditUserCompany(QString fPK)
{
    editUserCompany = fPK;
    this->action = "Edit";

    QSqlQuery query("SELECT * FROM `clients` WHERE `peselNIP`=?");
    query.addBindValue(fPK);
    query.exec();

    if(query.size() > 0 )
    {
        query.next();
        if(query.value("isCompany").toBool())
        {
            //Firma
            ui->checkBoxIsCompany->setCheckState(Qt::Checked);

            ui->lineEditNIP->setText( query.value("peselNIP").toString() );
            ui->lineEditCompanyName->setText( query.value("companyName").toString() );
            this->setWindowTitle(query.value("companyName").toString());

            ui->lineEditNIP->setEnabled(false);


        }else
        {
            //Osoba Prywatna
            ui->checkBoxIsCompany->setCheckState(Qt::Unchecked);

            ui->lineEditPESEL->setText( query.value("peselNIP").toString() );
            ui->lineEditName->setText( query.value("name").toString() );
            ui->lineEditSurName->setText( query.value("surName").toString() );
            this->setWindowTitle(query.value("name").toString() +" "+ query.value("surName").toString());

            ui->lineEditPESEL->setEnabled(false);
        }

        ui->lineEditAddress->setText( query.value("address").toString() );
        ui->lineEditPostOffice->setText( query.value("postOffice").toString() );
        ui->lineEditEmail->setText( query.value("email").toString() );
        ui->lineEditLandlinePhone->setText( query.value("landlinePhone").toString() );
        ui->lineEditMobilePhone->setText( query.value("mobilePhone").toString() );

    }

    ui->checkBoxIsCompany->hide();

}

void ClientView::setAddressCompleter(QStringList fAddresses)
{
    if(ui->lineEditAddress->completer())
        delete ui->lineEditAddress->completer();

    ui->lineEditAddress->setCompleter( new QCompleter(fAddresses, this) );
}

void ClientView::setPostOfficeCompleter(QStringList fPostoOffices)
{
    if(ui->lineEditPostOffice->completer())
        delete ui->lineEditPostOffice->completer();

    ui->lineEditPostOffice->setCompleter( new QCompleter(fPostoOffices, this) );
}

void ClientView::setNameCompleter(QStringList fNames)
{
    if(ui->lineEditName->completer())
        delete ui->lineEditName->completer();

    ui->lineEditName->setCompleter( new QCompleter(fNames, this) );
}

void ClientView::setSurNameCompleter(QStringList fSurNames)
{
    if(ui->lineEditSurName->completer())
        delete ui->lineEditSurName->completer();

    ui->lineEditSurName->setCompleter( new QCompleter(fSurNames, this) );
}

bool ClientView::checkClientNotHaveOrderOrAgreement(QString fPeselNIP)
{
    QSqlQuery orders("SELECT orderID FROM `orders` WHERE `peselNIP`=?");
    orders.addBindValue( fPeselNIP );
    orders.exec();

    if(orders.size())
        return false;

    QSqlQuery agreements("SELECT agreementNumber FROM `internetAgreement` WHERE `peselNIP`=?");
    agreements.addBindValue( fPeselNIP );
    agreements.exec();

    if(agreements.size())
        return false;

    return true;

}

bool ClientView::checkClientExist(QString fPeselNIP)
{
    QSqlQuery clients("SELECT peselNIP FROM `clients` WHERE `peselNIP`=?");
    clients.addBindValue( fPeselNIP );
    clients.exec();

    if(clients.size())
        return true;

    return false;
}

int ClientView::getSubjectedCompanyLastID(QString fNIP)
{

    fNIP = fNIP + "%";
    QSqlQuery clients("SELECT peselNIP FROM `clients` WHERE `peselNIP` LIKE ? ORDER BY peselNIP DESC");
    clients.addBindValue( fNIP );
    clients.exec();

    if(clients.size() == 1)
        return 0;

    if(clients.size() > 1)
    {
        clients.next();
        QString sId = clients.value("peselNIP").toString().split("-").at(1);
        return sId.toInt();
    }

}

bool ClientView::checkPESELisOK(QString fPESEL)
{
    if(fPESEL.length() != 11)
        return false;

    int p0 = fPESEL.mid(0,1).toInt();
    int p1 = fPESEL.mid(1,1).toInt();
    int p2 = fPESEL.mid(2,1).toInt();
    int p3 = fPESEL.mid(3,1).toInt();
    int p4 = fPESEL.mid(4,1).toInt();
    int p5 = fPESEL.mid(5,1).toInt();
    int p6 = fPESEL.mid(6,1).toInt();
    int p7 = fPESEL.mid(7,1).toInt();
    int p8 = fPESEL.mid(8,1).toInt();
    int p9 = fPESEL.mid(9,1).toInt();

    int tIloczyn = (1*p0)+(3*p1)+(7*p2)+(9*p3)+(1*p4)+(3*p5)+(7*p6)+(9*p7)+(1*p8)+(3*p9);
    int tIloczynMod = tIloczyn % 10;
    int checkSum = 10 - tIloczynMod;

    if(checkSum == 10)
        checkSum = 0;

    if(checkSum != fPESEL.mid(10,1).toInt() )
        return false;

    return true;
}

bool ClientView::checkNIPisOK(QString fNIP)
{
    if(fNIP.length() != 10)
        return false;

    int n0 = fNIP.mid(0,1).toInt();
    int n1 = fNIP.mid(1,1).toInt();
    int n2 = fNIP.mid(2,1).toInt();
    int n3 = fNIP.mid(3,1).toInt();
    int n4 = fNIP.mid(4,1).toInt();
    int n5 = fNIP.mid(5,1).toInt();
    int n6 = fNIP.mid(6,1).toInt();
    int n7 = fNIP.mid(7,1).toInt();
    int n8 = fNIP.mid(8,1).toInt();

    int tIloczyn = (6*n0)+(5*n1)+(7*n2)+(2*n3)+(3*n4)+(4*n5)+(5*n6)+(6*n7)+(7*n8);
    int checkSum = tIloczyn % 11;

    if(checkSum != fNIP.mid(9,1).toInt())
        return false;

    return true;
}

void ClientView::on_pushButtonSave_clicked()
{
    QSqlQuery query;

    if(action=="Add")
    {
        query.prepare("INSERT INTO `clients`(`peselNIP`, `isCompany`, `name`, `surName`, `companyName`, `address`, `postOffice`, `email`, `landlinePhone`, `mobilePhone`, `registerDate`) "
                      "VALUES (?,?,?,?,?,?,?,?,?,?,CURDATE())");

        if(ui->checkBoxIsCompany->checkState() == Qt::Unchecked)
        {
            if(checkPESELisOK( ui->lineEditPESEL->text() ))
            {
                if(!checkClientExist(ui->lineEditPESEL->text()))
                {
                    query.addBindValue( ui->lineEditPESEL->text() );
                    query.addBindValue( false );
                    query.addBindValue( ui->lineEditName->text() );
                    query.addBindValue( ui->lineEditSurName->text() );
                    query.addBindValue( "" );
                    query.addBindValue( ui->lineEditAddress->text() );
                    query.addBindValue( ui->lineEditPostOffice->text() );
                    query.addBindValue( ui->lineEditEmail->text() );

                    if(ui->lineEditLandlinePhone->text() == "+48")
                        query.addBindValue( "" );
                    else
                        query.addBindValue( ui->lineEditLandlinePhone->text() );


                    if(ui->lineEditMobilePhone->text() == "+48")
                        query.addBindValue( "" );
                    else
                        query.addBindValue( ui->lineEditMobilePhone->text() );

                    query.exec();
                    successful = true;
                    close();
                }else
                    QMessageBox::information(this, "Error", "Taki client już istnieje");
            }else
                QMessageBox::information(this, "Error", "Błędny nr PESEL");
        }else
        {
            if(checkNIPisOK(ui->lineEditNIP->text()))
            {
                if(checkClientExist( ui->lineEditNIP->text() ))
                {
                    int lastId = getSubjectedCompanyLastID( ui->lineEditNIP->text() );
                    lastId++;

                    query.addBindValue( ui->lineEditNIP->text() + "-" + QString::number( lastId ) );
                }else
                    query.addBindValue( ui->lineEditNIP->text() );

                query.addBindValue( true );
                query.addBindValue( "" );
                query.addBindValue( "" );
                query.addBindValue( ui->lineEditCompanyName->text() );
                query.addBindValue( ui->lineEditAddress->text() );
                query.addBindValue( ui->lineEditPostOffice->text() );
                query.addBindValue( ui->lineEditEmail->text() );

                if(ui->lineEditLandlinePhone->text() == "+48")
                    query.addBindValue( "" );
                else
                    query.addBindValue( ui->lineEditLandlinePhone->text() );


                if(ui->lineEditMobilePhone->text() == "+48")
                    query.addBindValue( "" );
                else
                    query.addBindValue( ui->lineEditMobilePhone->text() );


                query.exec();
                successful = true;
                close();
            }else
                QMessageBox::information(this, "Error", "Błędny nr NIP");
        }
    }else if(action=="Edit")
    {
        bool changedPeselNIP=false;

        query.prepare("UPDATE `clients` SET `peselNIP`=?,`isCompany`=?,`name`=?,`surName`=?,`companyName`=?,`address`=?,`postOffice`=?,`email`=?,`landlinePhone`=?,`mobilePhone`=? WHERE `peselNIP`=?");


        if(ui->checkBoxIsCompany->checkState() == Qt::Unchecked)
        {
            query.addBindValue( ui->lineEditPESEL->text() );
            query.addBindValue( false );
            query.addBindValue( ui->lineEditName->text() );
            query.addBindValue( ui->lineEditSurName->text() );
            query.addBindValue( "" );

            if(editUserCompany != ui->lineEditPESEL->text())
                changedPeselNIP = true;

        }else
        {
            query.addBindValue( ui->lineEditNIP->text() );
            query.addBindValue( true );
            query.addBindValue( "" );
            query.addBindValue( "" );
            query.addBindValue( ui->lineEditCompanyName->text() );

            if(editUserCompany != ui->lineEditNIP->text())
                changedPeselNIP = true;
        }

        query.addBindValue( ui->lineEditAddress->text() );
        query.addBindValue( ui->lineEditPostOffice->text() );
        query.addBindValue( ui->lineEditEmail->text() );
        query.addBindValue( ui->lineEditLandlinePhone->text() );
        query.addBindValue( ui->lineEditMobilePhone->text() );
        query.addBindValue( editUserCompany );
        query.exec();

        if(changedPeselNIP)
        {
            QMessageBox::information(this,"","");

            //Edit PESEL/NIP in internetAgreement
            query.prepare("UPDATE `internetAgreement` SET `peselNIP`=? WHERE `peselNIP`=?");
            if(ui->checkBoxIsCompany->checkState() == Qt::Unchecked)
                query.addBindValue( ui->lineEditPESEL->text() );
            else
                query.addBindValue( ui->lineEditNIP->text() );
            query.addBindValue( editUserCompany );
            query.exec();

            //Edit PESEL/NIP in orders
            query.prepare("UPDATE `orders` SET `peselNIP`=? WHERE `peselNIP`=?");
            if(ui->checkBoxIsCompany->checkState() == Qt::Unchecked)
                query.addBindValue( ui->lineEditPESEL->text() );
            else
                query.addBindValue( ui->lineEditNIP->text() );
            query.addBindValue( editUserCompany );
            query.exec();

        }
        successful = true;
        close();
    }

}

void ClientView::on_pushButtonCancel_clicked()
{
    successful = false;
    this->close();
}

void ClientView::on_checkBoxIsCompany_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->lineEditNIP->setEnabled(true);
        ui->lineEditCompanyName->setEnabled(true);

        ui->lineEditPESEL->setDisabled(true);
        ui->lineEditPESEL->setText("");
        ui->lineEditName->setDisabled(true);
        ui->lineEditName->setText("");
        ui->lineEditSurName->setDisabled(true);
        ui->lineEditSurName->setText("");

    }else if(arg1 == Qt::Unchecked)
    {
        ui->lineEditNIP->setEnabled(false);
        ui->lineEditNIP->setText("");
        ui->lineEditCompanyName->setEnabled(false);
        ui->lineEditCompanyName->setText("");

        ui->lineEditPESEL->setDisabled(false);
        ui->lineEditName->setDisabled(false);
        ui->lineEditSurName->setDisabled(false);
    }

}
