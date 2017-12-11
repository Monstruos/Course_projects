#ifndef USERDATA_H
#define USERDATA_H

#include <QDialog>
#include "database.h"
#include <QFileDialog>

namespace Ui {
class userdata;
}

class userdata : public QDialog
{
    Q_OBJECT

public:
    explicit userdata(DataBase *db, QWidget *parent = 0);
    ~userdata();

    void setData(int userID);

private slots:
    void on_addDocNote_clicked();

    void on_addAbon_clicked();

private:
    Ui::userdata *ui;
    DataBase *b;
    QSqlTableModel *model;
    int id;
    QString name;
    QString phone;
    QString gender;
    QDate DateOfReg;
    QDate SubDuration;
    QPixmap DocNote;
    QDate DocNoteDur;
};

#endif // USERDATA_H
