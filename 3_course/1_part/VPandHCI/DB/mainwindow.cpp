#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase(this);
    db->connectToDataBase("/home/monstruos/GitHub/Course_projects/3_course/1_part/VPandHCI/database/poolbase.db");
    this->setWindowTitle("База данных");
    ud = new userdata(db, this);
    ud->hide();
    this->setFocus();
    ui->FN->setStyleSheet("color: #ffffff");
    ui->From->setStyleSheet("color: #ffffff");
    ui->To->setStyleSheet("color: #ffffff");
    ui->dbView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dbView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dbView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mod = new QSqlTableModel();
    mod->setTable(TABLE1);
    mod->setHeaderData(1, Qt::Horizontal, QObject::tr("ФИО"));
    mod->setHeaderData(2, Qt::Horizontal, QObject::tr("Мобильный телефон"));
    mod->setHeaderData(3, Qt::Horizontal, QObject::tr("Пол"));
    qDebug() << mod->columnCount();
    mod->select();
    ui->dbView->setModel(mod);
    ui->dbView->setColumnHidden(0, true);
    ui->dbView->setColumnHidden(4, true);
    ui->dbView->setColumnHidden(5, true);
    ui->dbView->setAlternatingRowColors(true);
    ui->dbView->resizeColumnsToContents();
    ui->dbView->resizeRowsToContents();
    ui->dbView->horizontalHeader()->setStretchLastSection(true);

    mod2 = new QSqlTableModel();
    mod2->setTable(TABLE3);
    mod2->setHeaderData(1, Qt::Horizontal, QObject::tr("Название"));
    mod2->setHeaderData(2, Qt::Horizontal, QObject::tr("Стоимость"));
    mod2->setHeaderData(3, Qt::Horizontal, QObject::tr("Длительность (дней)"));
    mod2->select();
    ui->dbView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dbView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dbView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dbView_2->setModel(mod2);
    ui->dbView_2->setColumnHidden(0, true);
    ui->dbView_2->setAlternatingRowColors(true);
    ui->dbView_2->resizeColumnsToContents();
    ui->dbView_2->resizeRowsToContents();
    ui->dbView_2->horizontalHeader()->setStretchLastSection(true);

    mod3 = new QSqlRelationalTableModel();
    mod3->setTable(TABLE2);
    mod3->setHeaderData(1, Qt::Horizontal, QObject::tr("Дата регистрации абонемента"));
    mod3->setHeaderData(2, Qt::Horizontal, QObject::tr("Длительность абонемента"));
    mod3->setHeaderData(3, Qt::Horizontal, QObject::tr("ФИО"));
    mod3->setHeaderData(4, Qt::Horizontal, QObject::tr("Оплачено"));
    mod3->setHeaderData(5, Qt::Horizontal, QObject::tr("Тип абонемента"));
    mod3->setRelation(3, QSqlRelation("PoolVisit", "id", "Name"));
    mod3->setRelation(5, QSqlRelation("AbonType", "AbonId", "Name"));
    mod3->select();
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
    qDebug() << pointFrom << pointTo;
    if (!pointFrom.isValid() && !pointTo.isValid()) {
        if(ui->dbView->model() != mod) {
            ui->dbView->setModel(mod);
            ui->dbView->horizontalHeader()->swapSections(2, 3);
            ui->dbView->horizontalHeader()->swapSections(1, 3);
        }
        ui->dbView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->dbView->setColumnHidden(4, true);
        ui->dbView->setColumnHidden(5, true);
        return;
    }
    if(pointTo.isValid() && pointFrom.isValid()) {
        mod3->setFilter(QString("DateOfReg <= '" + pointTo.toString("yyyy-MM-dd") + "' AND SubDuration >= '" + pointFrom.toString("yyyy-MM-dd") + "'"));
    } else {
        if(pointTo.isValid())
            mod3->setFilter(QString("DateOfReg <= " + pointTo.toString("yyyy-MM-dd")));
        if(pointFrom.isValid())
            mod3->setFilter(QString("SubDuration >= " + pointFrom.toString("yyyy-MM-dd")));
    }
    if(ui->dbView->model() != mod3) {
        ui->dbView->setModel(mod3);
        ui->dbView->horizontalHeader()->swapSections(1, 3);
        ui->dbView->horizontalHeader()->swapSections(2, 3);
    }
    ui->dbView->setColumnHidden(4, false);
    ui->dbView->setColumnHidden(5, false);
    ui->dbView->setAlternatingRowColors(true);
    ui->dbView->resizeColumnsToContents();
    ui->dbView->resizeRowsToContents();
}

void MainWindow::on_FNsearch_textChanged(const QString &arg1)
{
    ui->dbView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->dbView->clearSelection();

    QSqlTableModel *m = (QSqlTableModel *)ui->dbView->model();
    m->setFilter(QString("Name LIKE '%" + arg1 + "%'"));
    m->select();
    if(arg1 == "") {
        ui->dbView->setSelectionMode(QAbstractItemView::SingleSelection);
    }
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


void MainWindow::on_addAbon_clicked()
{
    Dialog a(this, true, true, true, false, false);
    a.setHeader("Новый абонемент");
    a.setFirstLine("Название:", "");
    a.setSecLine("Стоимость:", "");
    a.setThirdLine("Длительность:", "");
    int res = a.exec();
    if(res == Dialog::Accepted) {
        QSqlQuery query;
        QString name, cost, dur, buf3;
        bool buf1, buf2;
        a.getData(name, cost, dur, buf1, buf2, buf3);
        qDebug() << name << cost << dur;
        if(name.isEmpty() || cost.isEmpty() || dur.isEmpty()) {
            QMessageBox::warning(this,
                                 "Ошибка",
                                 "Не удалось создать абонемент: некоторые поля не были заполнены");
            return;
        }
        query.prepare("INSERT INTO AbonType (Name, Cost, Dur)"
                      "VALUES(:name, :cost, :dur);");
        query.bindValue(":name", name);
        query.bindValue(":cost", cost.toInt());
        query.bindValue(":dur", dur.toInt());
        qDebug() << query.exec();
        mod2->select();
        ui->dbView_2->repaint();
        ui->dbView_2->resizeColumnsToContents();
        ui->dbView_2->resizeRowsToContents();
    }
}


void MainWindow::on_dbView_2_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QString name = mod2->data(mod2->index(row, 1)).toString();
    QString cost = mod2->data(mod2->index(row, 2)).toString();
    QString dur = mod2->data(mod2->index(row, 3)).toString();
    qDebug() << name;
    Dialog a(this, true, true, true, false, false);
    a.setHeader("Изменить абонемент");
    a.setFirstLine("Название:", name);
    a.setSecLine("Стоимость:", cost);
    a.setThirdLine("Длительность:", dur);
    int res = a.exec();
    if(res == Dialog::Accepted) {
        QSqlQuery query;
        bool buf1, buf2;
        QString buf3;
        a.getData(name, cost, dur, buf1, buf2, buf3);
        qDebug() << name << cost << dur;
        query.prepare("UPDATE AbonType SET "
                      "Name = :name, Cost = :cost, Dur = :dur "
                      "WHERE AbonId = :id");
        query.bindValue(":id", row + 1);
        query.bindValue(":name", name);
        query.bindValue(":cost", cost.toInt());
        query.bindValue(":dur", dur.toInt());
        qDebug() << query.exec();
        mod2->select();
        ui->dbView_2->repaint();
    }
}

void MainWindow::on_newUser_clicked()
{
    Dialog a(this, true, true, false, true, false);
    a.setHeader("Новый пользователь");
    a.setFirstLine("ФИО:","");
    a.setSecLine("Телефон:", "");
    a.setRadio("Мужчина", "Женщина");
    int res = a.exec();
    if(res == Dialog::Accepted) {
        QSqlQuery query;

        QString name, phone, buf1, buf3; // variables for getData()
        bool gender, buf2;
        a.getData(name, phone, buf1, gender, buf2, buf3);
        qDebug() << name << phone << gender;
        if(name.isEmpty() || phone.isEmpty())
        {
            QMessageBox::warning(this,
                                 "Ошибка",
                                 "Не удалось создать посетителя: некоторые поля не были заполнены");
            return;
        }
        QString g;
        if(gender == true) {
            g = "Мужской";
        } else {
            g = "Женский";
        }
        query.prepare("INSERT INTO PoolVisit (Name, Phone, Gender)"
                      "VALUES(:name, :phone, :gend);");
        query.bindValue(":name", name);
        query.bindValue(":phone", phone.toInt());
        query.bindValue(":gend", g);
        qDebug() << query.exec();
        mod->setFilter("");
        mod->select();
        mod->setFilter(QString("Name='" + name + "' AND Phone=" + phone + " AND Gender='" + g + "'"));
        mod->select();
        int rec = mod->record(0).field(0).value().toInt();
        ud->setData(rec);
        ud->exec();
        mod->setFilter("");
        mod->select();
        ui->dbView->repaint();
    }
}
