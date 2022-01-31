#ifndef OPTIONSMYMOBKIT_H
#define OPTIONSMYMOBKIT_H

#include <QDialog>
#include "inifile.h"

namespace Ui {
class OptionsMyMobKit;
}

class OptionsMyMobKit : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsMyMobKit(QWidget *parent = 0);
    ~OptionsMyMobKit();

    bool    successful;

    void    setOptions(QString fAddress);
    QString getAddress();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::OptionsMyMobKit *ui;
};

#endif // OPTIONSMYMOBKIT_H
