#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleF;
    styleF.setFileName(":/qss/styles.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    a.setStyleSheet(qssStr);
    MainWindow w;
    w.show();

    return a.exec();
}
