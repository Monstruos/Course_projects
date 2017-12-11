#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getData(QString &name, QString &cost, QString &dur)
{
    name = ui->name->text();
    cost = ui->cost->text();
    dur = ui->dur->text();
}
