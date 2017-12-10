#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "userdata.h"

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
    void on_FNsearch_textChanged(const QString &arg1);

    void on_minData_textChanged(const QString &arg1);

    void on_maxData_textChanged(const QString &arg1);

    void on_dbView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *mod;
    DataBase *db;
    userdata *ud;
    void date_comp(const QString &arg1, const QString &arg2);
};

#endif // MAINWINDOW_H
