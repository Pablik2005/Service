#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QCompleter>

namespace Ui {
class ClientView;
}

class ClientView : public QDialog
{
    Q_OBJECT

public:
    explicit ClientView(QWidget *parent = 0);
    ~ClientView();

    QString action;
    QString editUserCompany;
    bool    successful;

    bool    getIsCompany();
    QString getPESEL();
    QString getNIP();
    int     getSubjectedCompanyLastID(QString fNIP);

    void    setEditUserCompany(QString fPK);
    void    setAddressCompleter(QStringList fAddresses);
    void    setPostOfficeCompleter(QStringList fPostoOffices);
    void    setNameCompleter(QStringList fNames);
    void    setSurNameCompleter(QStringList fSurNames);

    bool    checkClientNotHaveOrderOrAgreement(QString fPeselNIP);
    bool    checkClientExist(QString fPeselNIP);
    bool    checkPESELisOK(QString fPESEL);
    bool    checkNIPisOK(QString fNIP);


private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();
    void on_checkBoxIsCompany_stateChanged(int arg1);


private:
    Ui::ClientView *ui;
};

#endif // CLIENTVIEW_H
