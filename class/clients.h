#ifndef CLIENTS_H
#define CLIENTS_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>


#include "clientview.h"

namespace Ui {
class Clients;
}

class Clients : public QDialog
{
    Q_OBJECT

public:
    explicit Clients(QWidget *parent = 0);
    ~Clients();


    bool        successful;
    bool        selectedIsCompany;

    QStringList listName;
    QStringList listSurName;
    QStringList listAddress;
    QStringList listPostOffice;

    QString     selectedPESELNIP;
    QString     selectedNameCompany;
    QString     selectedAddress;
    QString     selectedPostOffice;
    QString     selectedEmail;
    QString     selectedLandlinePhone;
    QString     selectedMobilePhone;



    void        filter();
    void        refresh();
    void        refreshClient(int fIndex);
    void        setSelectButtonEnable(bool fBool);
    QString     prepareAddresToCompleter(QString fAddress);

private slots:
    void on_pushButtonAddClient_clicked();
    void on_pushButtonDelClient_clicked();
    void on_pushButtonEditClient_clicked();
    void on_pushButtonSelect_clicked();

    void on_lineEditPESEL_textChanged();
    void on_lineEditName_textChanged();
    void on_lineEditAddress_textChanged();
    void on_lineEditPostOffice_textChanged();
    void on_lineEditEmail_textChanged();
    void on_lineEditLandlinePhone_textChanged();
    void on_lineEditMobilePhone_textChanged();
    void on_comboBoxCompany_currentIndexChanged();
    void on_dateEditRegistrationSince_userDateChanged();
    void on_dateEditRegistrationTo_userDateChanged();

private:
    Ui::Clients *ui;
};

#endif // CLIENTS_H
