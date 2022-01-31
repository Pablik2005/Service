#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QDialog>
#include <QtSql>
#include <QDateTime>
#include <QDate>
#include <QFileDialog>
#include <QtXML>
#include <QMessageBox>

namespace Ui {
class ReportGenerator;
}

class ReportGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit ReportGenerator(QWidget *parent = 0);
    ~ReportGenerator();

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonGenerate_clicked();

private:
    Ui::ReportGenerator *ui;
};

#endif // REPORTGENERATOR_H
