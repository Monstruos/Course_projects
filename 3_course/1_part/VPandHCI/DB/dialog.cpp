#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent,
               bool firstLine,
               bool secLine,
               bool thirdLine,
               bool radios,
               bool checkbox) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->firstLabel->hide();
    ui->firstLine->hide();
    ui->secLabel->hide();
    ui->secLine->hide();
    ui->thirdLabel->hide();
    ui->thirdLine->hide();
    ui->radioGroup->hide();
    ui->checkBox->hide();
    if(firstLine) {
        ui->firstLabel->show();
        ui->firstLine->show();
    }
    if(secLine) {
        ui->secLabel->show();
        ui->secLine->show();
    }
    if(thirdLine) {
        ui->thirdLabel->show();
        ui->thirdLine->show();
    }
    if(radios)
        ui->radioGroup->show();
    if(checkbox)
        ui->checkBox->show();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getData(QString &first, QString &second, QString &third, bool &radio, bool &checkbox)
{
    first = ui->firstLine->text();
    second = ui->secLine->text();
    third = ui->thirdLine->text();
    if(ui->radio1->isChecked())
        radio = true;
    else if(ui->radio2->isChecked())
        radio = false;
    else
        qDebug() << "error";

    if(ui->checkBox->isChecked())
        checkbox = true;
    else
        checkbox = false;
}

void Dialog::setFirstLine(QString label, QString line)
{
    ui->firstLabel->setText(label);
    ui->firstLine->setText(line);
}

void Dialog::setSecLine(QString label, QString line)
{
    ui->secLabel->setText(label);
    ui->secLine->setText(line);
}

void Dialog::setThirdLine(QString label, QString line)
{
    ui->thirdLabel->setText(label);
    ui->thirdLine->setText(line);
}

void Dialog::setRadio(QString first, QString second)
{
    ui->radio1->setText(first);
    ui->radio2->setText(second);
}

void Dialog::setCheckBox(QString check)
{
    ui->checkBox->setText(check);
}

void Dialog::setHeader(QString header)
{
    this->setWindowTitle(header);
}
