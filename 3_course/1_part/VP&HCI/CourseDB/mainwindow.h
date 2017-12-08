#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_searchFullName_textChanged(const QString &arg1);

    void on_searchDateFrom_textChanged(const QString &arg1);

    void on_searchDateTo_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QSqlTableModel *submod;
    QAbstractItemModel *mod;
    QSqlDatabase db;
    void date_comp(const QString &arg1 = "", const QString &arg2 = "");
};

#endif // MAINWINDOW_H
