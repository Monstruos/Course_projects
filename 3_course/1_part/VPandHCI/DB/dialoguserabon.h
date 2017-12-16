#ifndef DIALOGUSERABON_H
#define DIALOGUSERABON_H

#include <QDialog>
#include "database.h"


namespace Ui {
class DialogUserAbon;
}

class DialogUserAbon : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUserAbon(QSqlTableModel *model, QWidget *parent = 0);
    ~DialogUserAbon();
    int choose;

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::DialogUserAbon *ui;
    QSqlTableModel *mod;
};

#endif // DIALOGUSERABON_H
