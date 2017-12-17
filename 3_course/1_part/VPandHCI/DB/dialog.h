#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

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
    ~Dialog();
    void getData(QString &name, QString &cost, QString &dur);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
