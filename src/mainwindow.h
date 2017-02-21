#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QValidator>
#include <QDebug>
#include <QCoreApplication>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QGroupBox *groupBox;
    QPushButton *recButton;
    QPushButton *remButton;
    QLineEdit *lineEdit;
    QPushButton *clearButton;
    QLCDNumber *display;
    QPushButton ***paintArr;

    int paintHeight, paintWidth;
    int boxSize;
    int **memArr;
    QMap<int, QVector<int>> cellMap;

    QPushButton* newButton();
    void zeroing();
public:
    MainWindow(int h, int w, QWidget *parent = 0);
    ~MainWindow();
public slots:
    void rec();
    void rem();
    void clear();
};

#endif // MAINWINDOW_H
