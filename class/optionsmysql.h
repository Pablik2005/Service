#ifndef OPTIONSMYSQL_H
#define OPTIONSMYSQL_H

#include <QDialog>
#include <QMessageBox>
#include "inifile.h"

namespace Ui {
class OptionsMySQL;
}

class OptionsMySQL : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsMySQL(QWidget *parent = 0);
    ~OptionsMySQL();

    bool    successful;

    void    setOptions(QString fAddress, QString fLogin, QString fPassword, QString fDB);
    QString getAddress();
    QString getLogin();
    QString getPassword();
    QString getDB();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::OptionsMySQL *ui;
};

#endif // OPTIONSMYSQL_H
