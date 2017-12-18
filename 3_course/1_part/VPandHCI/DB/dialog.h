#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSqlTableModel>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0,
           QString header = "header",
           QString first = "first",
           QString second = "second",
           QString third = "third",
           QString firstLine = "",
           QString secLine = "",
           QString thirdLine = "");
    Dialog(QWidget *parent = 0,
           bool firstLine = false,
           bool secLine = false,
           bool thirdLine = false,
           bool radios = false,
           bool checkbox = false,
           bool combobox = false);
    ~Dialog();
    void getData(QString &first, QString &second, QString &third,
                 bool &radio, bool &checkbox, QString &combobox);
    void setFirstLine(QString label, QString line);
    void setSecLine(QString label, QString line);
    void setThirdLine(QString label, QString line);
    void setRadio(QString first, QString second);
    void setCheckBox(QString check, bool state);
    void setComboBox(QSqlTableModel *model, int column, int active);
    void setHeader(QString header);


private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
