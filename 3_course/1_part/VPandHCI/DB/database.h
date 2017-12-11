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
#define DATABASE_HOSTNAME   "PoolDataBase"

#define TABLE1          "PoolVisit"
#define TABLE1_FN       "Name"
#define TABLE1_PNUM     "Phone"
#define TABLE1_GND      "Gender"
#define TABLE1_PHOTO    "Doc_note"
#define TABLE1_NOTDR    "Doc_note_dur"

// Первая колонка содержит Autoincrement ID

#define TABLE2          "Abonements"
#define TABLE2_DOF      "DateOfReg"
#define TABLE2_SUBDR    "SubDuration"
#define TABLE2_COST     "Cost"
#define TABLE2_USRID    "UserID"
#define TABLE2_ISPAID   "IsPaid"


#define TABLE3          "AbonType"
#define TABLE3_NAME     "Name"
#define TABLE3_COST     "Cost"
#define TABLE3_DUR      "Dur"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     */
    void connectToDataBase(const QString &name);

    QSqlDatabase getDB();

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    // Внутренние методы для работы с базой данных
    bool openDataBase(const QString &name);     // Открытие базы данных
    bool restoreDataBase(const QString &name);  // Восстановление базы данных
    void closeDataBase();                       // Закрытие базы данных
    bool createTables();                        // Создание таблиц в базе данных

};

#endif // DATABASE_H
