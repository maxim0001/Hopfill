#define DEBUG true

#include "mainwindow.h"

MainWindow::MainWindow(int h, int w, QWidget *parent)
    : QWidget(parent), paintHeight(h), paintWidth(w)
{
    paintArr = new QPushButton**[paintHeight+1];
    for (int i = 0; i < paintHeight+1; i++) {
        paintArr[i] = new QPushButton*[paintWidth+1];
    }
    for (int i = 0; i < paintHeight; i++) {
        for (int j = 0; j < paintWidth; j++) {
            paintArr[i][j] = newButton();
        }
    }

    boxSize = paintHeight*paintWidth;

    // Memory array init
    // Нужно сохранять его куда-нить
    // И не инициализировать каждый раз, а считывать из сохранения
    memArr = new int*[boxSize];
    for (int i = 0; i < boxSize; i++) {
        memArr[i] = new int[boxSize];
    }
    // Этого тоже быть не должно
    // Ну, только если сохранения не будут найдены
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            memArr[i][j] = 0;
        }
    }

    recButton = new QPushButton("Запомнить");
    remButton = new QPushButton("Распознать");
    clearButton = new QPushButton("Очистить");

    connect(recButton, SIGNAL(clicked()), this, SLOT(rec()));
    connect(remButton, SIGNAL(clicked()), this, SLOT(rem()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));

    lineEdit = new QLineEdit;
    QRegExp rx("[0-9]{2}");
    QValidator *validator = new QRegExpValidator(rx, this);
    lineEdit->setValidator(validator);

    display = new QLCDNumber;
    display->setMinimumHeight(50);
    display->setSegmentStyle(QLCDNumber::Flat);
    display->display(0);

    groupBox = new QGroupBox;

    QGridLayout *paintLayout = new QGridLayout;
//    paintLayout->setSpacing(0);
    paintLayout->setMargin(0);
    paintLayout->setAlignment(Qt::AlignLeft);
    for (int i = 0; i < paintHeight; i++) {
        for (int j = 0; j < paintWidth; j++) {
            paintLayout->addWidget(paintArr[i][j],i, j, Qt::AlignHCenter);
        }
    }

    QHBoxLayout *readValueLayout = new QHBoxLayout;
    readValueLayout->addWidget(new QLabel("Значение:"));
    readValueLayout->addWidget(lineEdit);

    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->addWidget(display);
    controlLayout->addStretch(5);
    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(remButton);
    controlLayout->addLayout(readValueLayout);
    controlLayout->addWidget(recButton);

    QVBoxLayout *paintLayout_2 = new QVBoxLayout;
    paintLayout_2->addWidget(new QLabel("<H1>Значение:</H1>"));
    paintLayout_2->addLayout(paintLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(paintLayout_2);
    mainLayout->addLayout(controlLayout);

    setLayout(mainLayout);
}

MainWindow::~MainWindow()
{

}

QPushButton* MainWindow::newButton()
{
    QPushButton *but = new QPushButton(" ");
    but->setCheckable(true);
    return but;
}

void MainWindow::zeroing()
{
    for (int i = 0; i < boxSize; i++) {
        memArr[i][i] = 0;
    }
}

// Запись
void MainWindow::rec()
{
    // Read
    QVector<int> vec(boxSize);
    for (int i = 0, k = 0; i < paintHeight; i++) {
        for (int j = 0; j < paintWidth; j++) {
            if(paintArr[i][j]->isChecked())
            {
                vec[k++] = 1;
            }
            else
            {
                vec[k++] = -1;
            }
        }
    }

#if DEBUG
    QDebug deb(QtDebugMsg);
    qDebug() << "Value:" << lineEdit->text() << "Vec:" << vec;
#endif

    // Сохранение в итоговую матрицу
    for(int i = 0; i < (boxSize); i++) {
        for(int j = 0; j < boxSize; j++) {
            memArr[i][j] += vec[i] * vec[j];
        }
    }

    zeroing();

    cellMap[lineEdit->text().toInt(NULL)] = vec;

    lineEdit->clear();
    emit clear();

#if DEBUG
    deb << "memArr:" << endl;
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            deb << memArr[i][j] << " ";
        }
        deb << endl;
    }
#endif
}

// Распознание
void MainWindow::rem()
{
    // Read
    QVector<int> vec(boxSize);
    for (int i = 0, k = 0; i < paintHeight; i++) {
        for (int j = 0; j < paintWidth; j++) {
            if(paintArr[i][j]->isChecked())
            {
                vec[k++] = 1;
            }
            else
            {
                vec[k++] = -1;
            }
        }
    }

    QVector<int> vecY(boxSize);

    bool eq = false;

    while(!eq)
    {
        for (int i = 0; i < boxSize; i++) {
            vecY[i] = 0;
        }

        for(int i = 0; i < (boxSize); i++) {
            for(int j = 0; j < boxSize; j++) {
                vecY[i] += memArr[i][j]*vec[j];
            }
        }

        // Activation
        for (int i = 0; i < boxSize; i++) {
            if(vecY[i] >= 0) vecY[i] = 1;
            else vecY[i] = -1;
        }

        QMap<int, QVector<int>>::iterator iter = cellMap.begin();
        for (; iter != cellMap.end(); ++iter) {
            if(qEqual(iter.value().begin(), iter.value().end(), vecY.begin()))
            {
                eq = true;
                display->display(iter.key());
            }
        }

        qDebug() << vec;

        vec = vecY;

        QCoreApplication::processEvents();
    }
}

void MainWindow::clear()
{
    for (int i = 0; i < paintHeight; i++) {
        for (int j = 0; j < paintWidth; j++) {
            paintArr[i][j]->setChecked(false);
        }
    }
    display->display(0);
}
