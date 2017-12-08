#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME    "PoolDataBase"

#define TABLE1          "PoolVisit"
#define TABLE1_N        "Name"
#define TABLE1_PNUM     "Phone"
#define TABLE1_GND      "Gender"
#define TABLE1_PHOTO    "DocNote"
#define TABLE1_NOTDR    "DocNoteDur"

#define TABLE2          "Abonements"
#define TABLE2_USERID   "UserID"
#define TABLE2_DOR      "DateOfReg"
#define TABLE2_SUBD     "SubDuration"
#define TABLE2_COST     "Cost"
#define TABLE2_ISPAID   "IsPaid"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectDB(const QString &name);

    QSqlDatabase getDB();

private:
    QSqlDatabase db1;

    bool openDB(const QString &name);
    bool restoreDB(const QString &name);
    void closeDB();
};

#endif // DATABASE_H
