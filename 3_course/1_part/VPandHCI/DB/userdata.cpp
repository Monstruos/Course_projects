#include "userdata.h"
#include "ui_userdata.h"

userdata::userdata(DataBase *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userdata)
{

    b = db;
    ui->setupUi(this);
    ui->username->setStyleSheet("color:#ffffff; font-size: 16px");
    ui->gender->setStyleSheet("color:#ffffff; font-size: 14px");
    ui->phone->setStyleSheet("color:#ffffff; font-size: 14px");
    ui->abonDurFromLabel->setStyleSheet("color:#ffffff");
    ui->abonDurToLabel->setStyleSheet("color:#ffffff");
    ui->untilLabel->setStyleSheet("color:#ffffff");
    ui->abonDurToLine->setText("");
    ui->abonDurToLine->setText("");
    ui->isCost->setChecked(false);
    ui->prevAbon->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->prevAbon->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->prevAbon->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowTitle("Информация о пользователе/Редактирование");
    model = new QSqlRelationalTableModel();
    model->setTable(TABLE2);
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Дата регистрации"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Длительность абонемента"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Уплачено:"));
    model->setRelation(5, QSqlRelation("AbonType", "AbonId", "Name"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Тип абонемента:"));
    abonModel = new QSqlTableModel();
    abonModel->setTable(TABLE3);
}

userdata::~userdata()
{
    delete ui;
}

void userdata::setData(int userID)
{
    model->setFilter("");
    model->select();
    abonModel->setFilter("");
    abonModel->select();
    ui->abonType->setModel(abonModel);
    ui->abonType->setModelColumn(1);
    QSqlQuery *s = new QSqlQuery();
    s->exec("SELECT * FROM PoolVisit WHERE id = " + QString::number(userID));
    s->next();
    id = s->value(0).toInt();
    name = s->value(1).toString();
    phone = s->value(2).toString();
    gender = s->value(3).toString();
    QByteArray BDN = s->value(4).toByteArray();
    DocNote.loadFromData(BDN);
    if(!DocNote.isNull())
        DocNote = DocNote.scaledToWidth(471);
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
        pal.setColor(ui->untilLine->foregroundRole(), Qt::darkYellow);
    else
        pal.setColor(ui->untilLine->foregroundRole(), Qt::green);
    ui->untilLine->setPalette(pal);
    QString cond = "SELECT DateOfReg, SubDuration, AbonId FROM Abonements WHERE UserID = " + QString::number(id) +
            " AND DateOfReg <= '" + QDate::currentDate().toString("yyyy-MM-dd") +
            "' AND SubDuration >= '" + QDate::currentDate().toString("yyyy-MM-dd") + "'";
    s->exec(cond);
    ui->includeAbon->hide();
    ui->cancelAbon->hide();
    if(s->next()) {
        ui->addAbon->hide();
        ui->groupBox_2->show();
        ui->abonDurFromLine->setText(s->value(0).toString());
        ui->abonDurToLine->setText(s->value(1).toString());
        ui->abonType->setCurrentIndex(s->value(2).toInt() - 1);
    } else {
        ui->addAbon->show();
        ui->groupBox_2->hide();
    }



    model->setFilter("UserID = " + QString::number(id) +
                     " AND NOT (DateOfReg <= '" + QDate::currentDate().toString("yyyy-MM-dd") +
                     "' AND SubDuration >= '" + QDate::currentDate().toString("yyyy-MM-dd") + "')");
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
    QString picPath = QFileDialog::getOpenFileName(this,
                                                   tr("Открыть изображение"),
                                                   "",
                                                   tr("Images (*.png *.jpg *.jpeg)"));
    if(!picPath.isEmpty()) {
        QFile file(picPath);
        file.open(QFile::ReadWrite);
        QByteArray bImage = file.readAll();
        DocNote.loadFromData(bImage);
        DocNote = DocNote.scaledToWidth(471);
        ui->docphoto->setPixmap(DocNote);
        QSqlQuery query;
        query.prepare("UPDATE PoolVisit SET Doc_Note = :Note WHERE id = :id");
        query.bindValue(":Note", bImage);
        query.bindValue(":id", id);
        if(!query.exec()) {
            qDebug() << "Failed to load data in database";
        }
    }
}


void userdata::on_addAbon_clicked()
{
    ui->abonDurFromLine->setText("");
    ui->abonDurToLine->setText("");
    ui->groupBox_2->setTitle("Новый абонемент");
    ui->addAbon->hide();
    ui->includeAbon->show();
    ui->cancelAbon->show();
    ui->groupBox_2->show();
    ui->abonDurFromLine->setText("");
    ui->abonDurToLine->setText("");
    ui->isCost->setChecked(false);
}

void userdata::on_includeAbon_clicked()
{
    QDate DateOfReg;
    DateOfReg = QDate::fromString(ui->abonDurFromLine->text(), "yyyy-MM-dd");
    QDate SubDuration;
    SubDuration = QDate::fromString(ui->abonDurToLine->text(), "yyyy-MM-dd");
    int isPaid = ui->isCost->isChecked();
    int AbonID = ui->abonType->currentIndex() + 1;
    qDebug() << isPaid << AbonID << DateOfReg.toString("yyyy-MM-dd")
             << SubDuration.toString("yyyy-MM-dd") << ui->abonDurFromLine->text() << ui->abonDurToLine->text();
    if(!DateOfReg.isValid() || !SubDuration.isValid()) {
        ui->groupBox_2->setTitle("Неправильно введена дата");
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO Abonements (DateOfReg, SubDuration, UserID, isPaid, AbonId) VALUES (:DoR, :SDur, :UID, :isP, :AID)");
    query.bindValue(":DoR", DateOfReg.toString("yyyy-MM-dd"));
    query.bindValue(":SDur", SubDuration.toString("yyyy-MM-dd"));
    query.bindValue(":UID", id);
    query.bindValue(":isP", isPaid);
    query.bindValue(":AID", AbonID);
    if(!query.exec()) {
        qDebug() << "Failed to load data in database";
    }
    ui->groupBox_2->setTitle("Текущий абонемент");
    ui->includeAbon->hide();
    ui->cancelAbon->hide();
}

void userdata::on_cancelAbon_clicked()
{
    ui->addAbon->show();
    ui->includeAbon->hide();
    ui->cancelAbon->hide();
    ui->groupBox_2->hide();
}

void userdata::on_prevAbon_doubleClicked(const QModelIndex &index)
{
    int iden = model->data(model->index(index.row(), 0)).toInt();
    Dialog a(this, true, true, false, false, true, true);
    model->setFilter(QString("idAbonements=" + QString::number(iden)));
    model->select();
    QSqlRecord rec = model->record(0);
    QString from = rec.field(1).value().toString();
    QString to = rec.field(2).value().toString();
    bool isCost = rec.field(4).value().toBool();
    int AbonId = rec.field(5).value().toInt();
    qDebug() << from << to << isCost << AbonId;
    a.setHeader("Просмотр и редактирование абонемента");
    a.setFirstLine("Действителен с:", from);
    a.setSecLine("Действителен до:", to);
    a.setCheckBox("Оплачено", isCost);
    a.setComboBox(abonModel, 1, AbonId-1);
    int res = a.exec();
    if(res == Dialog::Accepted) {
        QSqlQuery query;
        QString RegFrom, RegTo, buf1, TypeAbon;
        bool buf2, cost;

        a.getData(RegFrom, RegTo, buf1, buf2, cost, TypeAbon);
        int c = cost;
        qDebug() << RegFrom << RegTo << cost << "(" << c << ")"
                 << TypeAbon;

        //parsing to Date

        QDate dateFrom = QDate::fromString(RegFrom, "yyyy-MM-dd");
        QDate dateTo = QDate::fromString(RegTo, "yyyy-MM-dd");
        if(!dateFrom.isValid() || !dateTo.isValid()) {
            QMessageBox::warning(this,
                                 "Ошибка",
                                 "Не удалось изменить данные. Возможно, даты не соответствовали формату (YYYY-MM-DD).");
            return;
        }

        //finding Abonement type
        qDebug() << "TypeAbon = " << TypeAbon
                 << "FILTER: " << QString("Name = '" + TypeAbon + "'");
        abonModel->setFilter(QString("Name = '" + TypeAbon + "'"));
        abonModel->select();


        query.prepare("UPDATE Abonements SET "
                      "DateOfReg = :dof, "
                      "SubDuration = :sd, "
                      "isPaid = :pd, "
                      "AbonId = :aid "
                      "WHERE idAbonements = :id");
        query.bindValue(":id", id);
        query.bindValue(":dof", dateFrom.toString("yyyy-MM-dd"));
        query.bindValue(":sd", dateTo.toString("yyyy-MM-dd"));
        query.bindValue(":pd", c);
        query.bindValue(":aid", abonModel->record(0).field(0).value().toInt());
        qDebug() << iden << dateFrom << dateTo << c << abonModel->record(0).field(0).value().toInt();
        qDebug() << query.exec();
        abonModel->setFilter("");
        abonModel->select();
        model->setFilter("UserID = " + QString::number(id) +
                         " AND NOT (DateOfReg <= '" + QDate::currentDate().toString("yyyy-MM-dd") +
                         "' AND SubDuration >= '" + QDate::currentDate().toString("yyyy-MM-dd") + "')");
        model->select();
        ui->prevAbon->repaint();
    }

}
