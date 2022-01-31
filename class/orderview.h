#ifndef ORDERVIEW_H
#define ORDERVIEW_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QMovie>

#include "clients.h"
#include "itemview.h"
#include "serviceview.h"
#include "structures.h"
#include "sendsms.h"

namespace Ui {
class OrderView;
}

class OrderView : public QDialog
{
    Q_OBJECT

public:
    explicit OrderView(QWidget *parent = 0);
    ~OrderView();

    QString              action;
    bool                 successful;
    int                  editOrderID;
    double               priceTotalItem;
    double               priceTotalService;
    QList<ServiceItem>   items;
    QList<ServiceItem>   services;

    void    addOrdr();
    void    editOrder(int orderID);

    void    refreshItemsList();
    void    refreshServicesList();

    int     getOrderID();

private slots:
    void on_pushButtonSearchClient_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonPrint_clicked();

    void on_pushButtonAddItem_clicked();
    void on_pushButtonRemoveItem_clicked();
    void on_listWidgetRepairItems_doubleClicked(const QModelIndex &index);

    void on_pushButtonAddService_clicked();
    void on_pushButtonRemoveService_clicked();
    void on_listWidgetRepairServices_doubleClicked(const QModelIndex &index);

    void on_checkBoxInvoice_stateChanged(int arg1);
    void on_checkBoxComplaint_stateChanged(int arg1);
    void on_checkBoxNotification_stateChanged(int arg1);
    void on_checkBoxEndService_stateChanged(int arg1);

    void on_pushButtonComplaint_clicked();
    void on_pushButtonAddTermsService_clicked();
    void on_pushButtonTermsFileSave_clicked();

    void on_pushButton_clicked();

private:
    Ui::OrderView *ui;
};

#endif // ORDERVIEW_H
