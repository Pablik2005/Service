#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

#include "inifile.h"

namespace Ui {
class ServiceView;
}

class ServiceView : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceView(QWidget *parent = 0);
    ~ServiceView();

    QString action;
    bool    successful;

    int     orderID;
    int     serviceID;
    double  netMargin;
    double  grossMargin;

    void    editService(int fServiceID, int fOrderID);

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();
    void on_doubleSpinBoxPriceNet_valueChanged(double arg1);

private:
    Ui::ServiceView *ui;
};

#endif // SERVICEVIEW_H
