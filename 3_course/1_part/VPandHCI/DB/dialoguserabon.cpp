#include "dialoguserabon.h"
#include "ui_dialoguserabon.h"

DialogUserAbon::DialogUserAbon(QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUserAbon)
{
    ui->setupUi(this);
    mod = model;
    ui->tableView->setModel(mod);
    choose = -1;
}

DialogUserAbon::~DialogUserAbon()
{
    delete ui;
}

void DialogUserAbon::on_tableView_doubleClicked(const QModelIndex &index)
{
    choose = index.row()+1;
    this->close();
}
