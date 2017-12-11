#include "userdata.h"
#include "ui_userdata.h"

userdata::userdata(DataBase *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userdata)
{

    b = db;
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable(TABLE2);
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Дата регистрации"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Длительность абонемента"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Уплачено:"));
}

userdata::~userdata()
{
    delete ui;
}

void userdata::setData(int userID)
{
    QSqlQuery *s = new QSqlQuery();
    s->exec("SELECT * FROM PoolVisit WHERE id = " + QString::number(userID));
    s->next();
    id = s->value(0).toInt();
    name = s->value(1).toString();
    phone = s->value(2).toString();
    gender = s->value(3).toString();
    DateOfReg = QDate::fromString(s->value(4).toString(), "yyyy-MM-dd");
    SubDuration = QDate::fromString(s->value(5).toString(), "yyyy-MM-dd");
    QByteArray BDN = s->value(6).toByteArray();
    DocNote.loadFromData(BDN);
    DocNoteDur = QDate::fromString(s->value(7).toString(), "yyyy-MM-dd");
    ui->isCost->setChecked(s->value(8).toInt());

    ui->username->setText(name);
    ui->phone->setText("Телефон: " + phone);
    ui->gender->setText("Пол: " + gender);
    ui->abonDurFromLine->setText(DateOfReg.toString("yyyy-MM-dd"));
    ui->abonDurToLine->setText(SubDuration.toString("yyyy-MM-dd"));
    ui->docphoto->setPixmap(DocNote);
    ui->untilLine->setText(DocNoteDur.toString("yyyy-MM-dd"));
    int status = DocNoteDur.daysTo(QDate::currentDate());
    QPalette pal = ui->untilLine->palette();
    if(status >= 0)
        pal.setColor(ui->untilLine->foregroundRole(), Qt::red);
    else if(status > -7)
        pal.setColor(ui->untilLine->foregroundRole(), Qt::yellow);
    else
        pal.setColor(ui->untilLine->foregroundRole(), Qt::green);
    ui->untilLine->setPalette(pal);

    s->exec("SELECT DateOfReg, SubDuration FROM Abonements WHERE id = " + QString::number(userID) +
            ", DateOfReg <= " + QDate::currentDate().toString("yyyy-MM-dd") +
            ", SubDuration >= " + QDate::currentDate().toString("yyyy-MM-dd"));
    if(s->next()) { // if abonement is existing now
        ui->groupBox_2->show();
        ui->addAbon->hide();
        DateOfReg = QDate::fromString(s->value(0).toString(), "yyyy-MM-dd");
        SubDuration = QDate::fromString(s->value(1).toString(), "yyyy-MM-dd");
        ui->abonDurFromLine->setText(DateOfReg.toString("yyyy-MM-dd"));
        ui->abonDurToLine->setText(SubDuration.toString("yyyy-MM-dd"));
    } else {
        ui->groupBox_2->hide();
        ui->addAbon->show();
    }


    model->setFilter(QString("idAbonements = %1").arg(id));
    model->select();
    ui->prevAbon->setModel(model);
    ui->prevAbon->setColumnHidden(0, true);
    ui->prevAbon->setColumnHidden(3, true);
    ui->prevAbon->setAlternatingRowColors(true);
    ui->prevAbon->resizeColumnsToContents();
    ui->prevAbon->resizeRowsToContents();
}

void userdata::on_addDocNote_clicked()
{
    QString picPath = QFileDialog::getOpenFileName(this, "Открыть изображение");
    QFile file(picPath);
    file.open(QFile::ReadWrite);
    QByteArray bImage = file.readAll();
    DocNote.loadFromData(bImage);
    ui->docphoto->setPixmap(DocNote);
    QSqlQuery query;
    query.prepare("UPDATE PoolVisit SET Doc_Note = :Note WHERE id = :id");
    query.bindValue(":Note", bImage);
    query.bindValue(":id", id);
    if(!query.exec()) {
        qDebug() << "Failed to load data in database";
    }
}


void userdata::on_addAbon_clicked()
{

}
