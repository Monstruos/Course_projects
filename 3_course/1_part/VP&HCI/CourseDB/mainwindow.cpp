#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DataBase a;
    a.connectDB(QString("C:\\database.db"));
    db = a.getDB();
    ui->DBView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->DBView->setSelectionBehavior(QAbstractItemView::SelectRows);
    model = new QSqlTableModel();
    submod = new QSqlTableModel();
    mod = QAbstractItemModel();

    model->setTable(TABLE);
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ФИО"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Мобильный телефон"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Пол"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Дата регистрации абонемента"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Длительность абонемента"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Прошлые абонементы"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Справка"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Длительность справки"));
    model->select();
    ui->DBView->setModel(model);
    ui->DBView->setColumnHidden(0, true);
    ui->DBView->setAlternatingRowColors(true);
    ui->DBView->resizeColumnsToContents();
    ui->DBView->resizeRowsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchFullName_textChanged(const QString &arg1)
{
    ui->DBView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->DBView->clearSelection();
    for (int i = 0; i < model->rowCount(); ++i){
        ui->DBView->showRow(i);
        QString str = model->index(i,1).data().toString();
        if (str.startsWith(arg1, Qt::CaseInsensitive))
            ui->DBView->showRow(i);
        else
            ui->DBView->hideRow(i);
    }
    if (arg1 == "")
        ui->DBView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_searchDateFrom_textChanged(const QString &arg1)
{
    date_comp(arg1, ui->searchDateTo->text());
}

void MainWindow::on_searchDateTo_textChanged(const QString &arg1)
{
    date_comp(ui->searchDateFrom->text(), arg1);
}

void MainWindow::date_comp(const QString &arg1, const QString &arg2)
{
    if (arg1 == "" && arg2 == "") {
        ui->DBView->setSelectionMode(QAbstractItemView::SingleSelection);
        return;
    }
    QDate pointFrom = QDate::fromString(arg1, "yyyy-MM-dd");
    QDate pointTo = QDate::fromString(arg2, "yyyy-MM-dd");
    if (!pointFrom.isValid() && !pointTo.isValid()) {
        ui->DBView->setSelectionMode(QAbstractItemView::SingleSelection);
        return;
    }
    ui->DBView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->DBView->clearSelection();

    for (int i = 0; i < model->rowCount(); ++i){
        ui->DBView->showRow(i);
        QString str1 = model->index(i,4).data().toString();
        QString str2 = model->index(i,5).data().toString();
        QDate buf1 = QDate::fromString(str1, "yyyy-MM-dd");
        QDate buf2 = QDate::fromString(str2, "yyyy-MM-dd");
        bool show1 = true, show2 = true;
        if(pointFrom.isValid())
            show1 = (buf2.daysTo(pointFrom) <= 0);
        if(pointTo.isValid())
            show2 = (buf1.daysTo(pointTo) >= 0);
        if(show1 && show2)
            ui->DBView->showRow(i);
        else
            ui->DBView->hideRow(i);
    }
}
