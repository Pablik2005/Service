#include "reportgenerator.h"
#include "ui_reportgenerator.h"

ReportGenerator::ReportGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportGenerator)
{
    ui->setupUi(this);

    this->setWindowTitle("Raport");
    ui->dateEditSice->setDate( QDate::currentDate() );
    ui->dateEditFor->setDate( QDate::currentDate() );
}

ReportGenerator::~ReportGenerator()
{
    delete ui;
}

void ReportGenerator::on_pushButtonClose_clicked()
{
    close();
}

void ReportGenerator::on_pushButtonGenerate_clicked()
{
    if(ui->dateEditSice->date() <= ui->dateEditFor->date())
    {
        double kasiora=0;

        QDateTime tSince;
        tSince.setDate(ui->dateEditSice->date());
        tSince.setTime(QTime(0,0,0));

        QDateTime tFor;
        tFor.setDate( ui->dateEditFor->date() );
        tFor.setTime(QTime(23,59,59));

        QSqlQuery query("SELECT orderID, peselNIP, invoice, description, endServiceDate, repairCostTotal, repairCostService  FROM `orders` WHERE `endServiceDate` BETWEEN ? AND ?");
        query.addBindValue(tSince);
        query.addBindValue(tFor);
        query.exec();


        QDomDocument xml;
        QDomElement orders = xml.createElement("Orders");
        xml.appendChild(orders);

        while(query.next())
        {
            if(query.value("repairCostTotal").toDouble() != 0)
            {

                QString tPeselNIP = query.value("peselNIP").toString();
                QSqlQuery client("SELECT isCompany, name, surName, companyName FROM `clients` WHERE `peselNIP`=?");
                client.addBindValue( tPeselNIP );
                client.exec();
                client.next();

                QString tName;
                if(client.value("isCompany").toBool())
                    tName = client.value("companyName").toString();
                else
                    tName = client.value("name").toString() + client.value("surName").toString();
                QDomElement order = xml.createElement(tName);

                order.setAttribute("ID", query.value("orderID").toString());
                order.setAttribute("Data wydania", query.value("endServiceDate").toDate().toString("yyyy-MM-dd"));
                order.setAttribute("Opis", query.value("description").toString());
                order.setAttribute("Koszt klienta", query.value("repairCostTotal").toString() + " (-" + query.value("repairCostService").toString() + ")");

                if(query.value("invoice").toBool())
                {
                    order.setAttribute("Faktura","Tak");
                    kasiora +=  (query.value("repairCostTotal").toDouble() * 0.77 ) - query.value("repairCostService").toDouble();
                }
                else
                {
                    order.setAttribute("Faktura","Nie");
                    kasiora += query.value("repairCostTotal").toDouble() - query.value("repairCostService").toDouble();
                }

                int  tOrderID = query.value("orderID").toInt();
                QSqlQuery itemQuery("SELECT name, priceNet  FROM `serviceItems` WHERE `orderID`=?");
                itemQuery.addBindValue( tOrderID );
                itemQuery.exec();
                while (itemQuery.next())
                {
                    QDomElement item = xml.createElement(itemQuery.value("name").toString());
                    item.setAttribute("Cena", "-" + itemQuery.value("priceNet").toString());
                    order.appendChild(item);
                }

                orders.appendChild(order);
            }
        }

        QString filePath = QFileDialog::getSaveFileName(this, tr("Zapisz"), QCoreApplication::applicationDirPath() + "/raport.xml" );
        QFile file(filePath);
        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << xml.toString();
            file.close();
        }

        QMessageBox::information(this,"", QString::number( kasiora ));
    }
    close();
}
