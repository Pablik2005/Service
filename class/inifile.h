#ifndef INIFILE_H
#define INIFILE_H

#include <QtCore>
#include <QFile>


class INIFile
{
public:
    INIFile();
    INIFile(QString fFilePatch);

    QString filePatch;

    QString mysqlAddres;
    QString mysqlLogin;
    QString mysqlPassword;
    QString mysqlDB;

    double  netMargin;
    double  grossMargin;

    QString mymobkitAddress;

    void setIniFile(QString fFilePatch);
    void saveToFile();
    void loadFromFile();

    void setMySQL(QString fAddres, QString fLogin, QString fPassword, QString fDB);
    void setMyMobKit(QString fAddress);
    void setService(double fNetMargin, double fGrossMargin);

};

#endif // INIFILE_H
