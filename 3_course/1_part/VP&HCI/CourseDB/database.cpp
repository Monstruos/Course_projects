#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{
    closeDB();
}

void DataBase::connectDB(const QString &name)
{
    if(!QFile(name).exists()){
        qDebug() << "restore DB";
        restoreDB(name);
    } else {
        qDebug() << "open DB";
        openDB(name);
    }
}

QSqlDatabase DataBase::getDB()
{
    return db;
}

bool DataBase::openDB(const QString &name)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(name);
    if(db.open()){
        qDebug() << "Can open DB " << name;
        return true;
    } else {
        qDebug() << "Can't open DB " << name;
        return false;
    }
}

bool DataBase::restoreDB(const QString &name)
{
    qDebug() << "Can't restore DB";
    qDebug() << db.lastError().text();
    return false;
}

void DataBase::closeDB()
{
    db.close();
}



