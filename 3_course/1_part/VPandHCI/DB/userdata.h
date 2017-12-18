#ifndef USERDATA_H
#define USERDATA_H

#include <QDialog>
#include "database.h"
#include "dialog.h"
#include <QMessageBox>
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

    void on_includeAbon_clicked();

    void on_cancelAbon_clicked();

    void on_prevAbon_doubleClicked(const QModelIndex &index);

private:
    Ui::userdata *ui;
    DataBase *b;
    QSqlRelationalTableModel *model;
    QSqlTableModel *abonModel;
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
