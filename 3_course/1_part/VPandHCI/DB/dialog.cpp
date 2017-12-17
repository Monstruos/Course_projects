#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent,
               QString header,
               QString first,
               QString second,
               QString third,
               QString firstLine,
               QString secLine,
               QString thirdLine) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(header);
    ui->firstLabel->setText(first);
    ui->secLabel->setText(second);
    ui->thirdLabel->setText(third);
    ui->firstLine->setText(firstLine);
    ui->secLine->setText(secLine);
    ui->thirdLine->setText(thirdLine);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getData(QString &name, QString &cost, QString &dur)
{
    name = ui->firstLine->text();
    cost = ui->secLine->text();
    dur = ui->thirdLine->text();
}
