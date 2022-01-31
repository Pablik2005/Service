#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>

#include "class/inifile.h"
#include "class/clients.h"
#include "class/orderview.h"
#include "class/optionsmysql.h"
#include "class/optionsmymobkit.h"
#include "class/reportgenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    INIFile         iniFile;
    QSqlDatabase    db;

    void            refresh();
    void            refreshOrder(int fIndex);
    void            filter();

private slots:
    void on_pushButtonAddOrder_clicked();
    void on_pushButtonEditOrder_clicked();
    void on_pushButtonDelOrder_clicked();
    void on_pushButtonClients_clicked();
    void on_pushButtonFiltr_clicked();
    void on_actionMySQL_triggered();
    void on_pushButtonLogin_clicked();

    void on_lineEditID_textChanged();
    void on_lineEditPESELNIP_textChanged();
    void on_lineEditNemCompany_textChanged();
    void on_lineEditDevice_textChanged();
    void on_lineEditProducer_textChanged();
    void on_lineEditModel_textChanged();
    void on_comboBoxNotified_currentIndexChanged();
    void on_comboBoxEnd_currentIndexChanged();

    void on_actionGeneruj_triggered();

    void on_actionMyMobKit_SMS_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
