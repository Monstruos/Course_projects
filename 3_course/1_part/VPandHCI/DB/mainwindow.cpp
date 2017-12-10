#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase(this);
    db->connectToDataBase("C:\\database.db");

    ud = new userdata(db, this);
    ud->hide();

    ui->dbView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dbView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dbView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mod = new QSqlRelationalTableModel();
    mod->setTable(TABLE1);
    mod->setHeaderData(1, Qt::Horizontal, QObject::tr("ФИО"));
    mod->setHeaderData(2, Qt::Horizontal, QObject::tr("Мобильный телефон"));
    mod->setHeaderData(3, Qt::Horizontal, QObject::tr("Пол"));
    mod->select();
    ui->dbView->setModel(mod);
    ui->dbView->setColumnHidden(0, true);
    ui->dbView->setColumnHidden(4, true);
    ui->dbView->setColumnHidden(5, true);
    ui->dbView->setColumnHidden(6, true);
    ui->dbView->setColumnHidden(7, true);
    ui->dbView->setColumnHidden(8, true);
    ui->dbView->setAlternatingRowColors(true);
    ui->dbView->resizeColumnsToContents();
    ui->dbView->resizeRowsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::date_comp(const QString &arg1, const QString &arg2)
{
    ui->dbView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->dbView->clearSelection();
    QDate pointFrom = QDate::fromString(arg1, "yyyy-MM-dd");
    QDate pointTo = QDate::fromString(arg2, "yyyy-MM-dd");


    for (int i = 0; i < mod->rowCount(); ++i){
        ui->dbView->showRow(i);
        QString str1 = mod->index(i,4).data().toString();
        QString str2 = mod->index(i,5).data().toString();
        QDate buf1 = QDate::fromString(str1, "yyyy-MM-dd");
        QDate buf2 = QDate::fromString(str2, "yyyy-MM-dd");
        bool show1 = true, show2 = true;
        if(pointFrom.isValid())
            show1 = (buf2.daysTo(pointFrom) <= 0);
        if(pointTo.isValid())
            show2 = (buf1.daysTo(pointTo) >= 0);
        if(show1 && show2)
            ui->dbView->showRow(i);
        else
            ui->dbView->hideRow(i);
    }
    if (!pointFrom.isValid() && !pointTo.isValid()) {
        ui->dbView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void MainWindow::on_FNsearch_textChanged(const QString &arg1)
{
    ui->dbView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->dbView->clearSelection();
    for (int i = 0; i < mod->rowCount(); ++i){
        ui->dbView->showRow(i);
        QString str = mod->index(i,1).data().toString();
        if (str.startsWith(arg1, Qt::CaseInsensitive))
            ui->dbView->showRow(i);
        else
            ui->dbView->hideRow(i);
    }
    if (arg1 == "")
        ui->dbView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_minData_textChanged(const QString &arg1)
{
    date_comp(arg1, ui->maxData->text());
}

void MainWindow::on_maxData_textChanged(const QString &arg1)
{
    date_comp(ui->minData->text(), arg1);
}

void MainWindow::on_dbView_doubleClicked(const QModelIndex &index)
{
    ud->setData(index.row()+1);
    ud->exec();
}
