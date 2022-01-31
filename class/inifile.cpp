#include "inifile.h"

INIFile::INIFile()
{

}

INIFile::INIFile(QString fFilePatch)
{
    filePatch = fFilePatch;
    loadFromFile();
}

void INIFile::setIniFile(QString fFilePatch)
{
    filePatch = fFilePatch;
    loadFromFile();
}

void INIFile::saveToFile()
{
    QSettings settings( filePatch, QSettings::IniFormat );

    settings.beginGroup("MySQL");
    settings.setValue("Address", mysqlAddres);
    settings.setValue("Login", mysqlLogin);
    settings.setValue("Password", mysqlPassword);
    settings.setValue("DB", mysqlDB);
    settings.endGroup();

    settings.beginGroup("MyMobKit");
    settings.setValue("Address", mymobkitAddress);
    settings.endGroup();

    settings.beginGroup("Service");
    settings.setValue("NetMargin", netMargin);
    settings.setValue("GrossMargin", grossMargin);
    settings.endGroup();

}

void INIFile::loadFromFile()
{
    QFile iniFile(filePatch);
    if(iniFile.exists())
    {
        QSettings settings( filePatch, QSettings::IniFormat);

        settings.beginGroup("MySQL");
        mysqlAddres = settings.value("Address").toString();
        mysqlLogin = settings.value("Login").toString();
        mysqlPassword = settings.value("Password").toString();
        mysqlDB = settings.value("DB").toString();
        settings.endGroup();

        settings.beginGroup("MyMobKit");
        mymobkitAddress = settings.value("Address").toString();
        settings.endGroup();

        settings.beginGroup("Service");
        netMargin = settings.value("NetMargin").toDouble();
        grossMargin = settings.value("GrossMargin").toDouble();
        settings.endGroup();
    }

}

void INIFile::setMySQL(QString fAddres, QString fLogin, QString fPassword, QString fDB)
{
    mysqlAddres = fAddres;
    mysqlLogin = fLogin;
    mysqlPassword = fPassword;
    mysqlDB = fDB;
    saveToFile();
}

void INIFile::setMyMobKit(QString fAddress)
{
    mymobkitAddress = fAddress;
    saveToFile();
}

void INIFile::setService(double fNetMargin, double fGrossMargin)
{
    netMargin = fNetMargin;
    grossMargin = fGrossMargin;
    saveToFile();
}

