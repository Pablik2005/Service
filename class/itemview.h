#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include <QDialog>
#include <QFileDialog>
#include <QtSql>

#include "inifile.h"

namespace Ui {
class ItemView;
}

class ItemView : public QDialog
{
    Q_OBJECT

public:
    explicit ItemView(QWidget *parent = 0);
    ~ItemView();

    QString action;
    bool    successful;

    int     orderID;
    int     itemID;
    double  netMargin;
    double  grossMargin;

    void    editItem(int fItemID, int fOrderID);


private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();
    void on_checkBoxWarranty_clicked();
    void on_doubleSpinBoxPriceMoxum_valueChanged(double arg1);

    void on_pushButtonBillAdd_clicked();

    void on_pushButtonBillSave_clicked();

    void on_pushButtonBillDelete_clicked();

private:
    Ui::ItemView *ui;
};

#endif // ITEMVIEW_H
