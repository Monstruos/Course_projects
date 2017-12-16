#include "userdata.h"
#include "ui_userdata.h"

userdata::userdata(DataBase *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userdata)
{

    b = db;
    ui->setupUi(this);
    this->setWindowTitle("Информация о пользователе");
    model = new QSqlRelationalTableModel();
    model->setTable(TABLE2);
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Дата регистрации"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Длительность абонемента"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Уплачено:"));
    model->setRelation(5, QSqlRelation("AbonType", "AbonId", "Name"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Тип абонемента:"));
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
    QByteArray BDN = s->value(4).toByteArray();
    DocNote.loadFromData(BDN);
    DocNoteDur = QDate::fromString(s->value(5).toString(), "yyyy-MM-dd");
    ui->isCost->setChecked(s->value(6).toInt());

    ui->username->setText(name);
    ui->phone->setText("Телефон: " + phone);
    ui->gender->setText("Пол: " + gender);
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
    QString cond = "SELECT DateOfReg, SubDuration FROM Abonements WHERE UserID = " + QString::number(id) +
            " AND DateOfReg <= '" + QDate::currentDate().toString("yyyy-MM-dd") +
            "' AND SubDuration >= '" + QDate::currentDate().toString("yyyy-MM-dd") + "'";
    s->exec(cond);
    if(s->next()) {
        ui->addAbon->hide();
        ui->groupBox_2->show();
        ui->abonDurFromLine->setText(s->value(0).toString());
        ui->abonDurToLine->setText(s->value(1).toString());
    } else {
        ui->addAbon->show();
        ui->groupBox_2->hide();
    }



    model->setFilter(QString("UserID = %1").arg(id));
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
//    DialogUserAbon *a = new DialogUserAbon(mod2);
//    a.exec();
//    int ch = a.choose;
//    if(choose != -1) {

//    }
}
