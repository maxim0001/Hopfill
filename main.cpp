#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Размеры поля.
    int paintHeight = 5;
    int paintWidth = 3;

    MainWindow w(paintHeight, paintWidth);
    w.setFixedSize(320, 240);
    w.show();
    return a.exec();
}
