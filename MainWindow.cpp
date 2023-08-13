#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QVector3D>
#include <QDebug>
#include <Camera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool finished = false;
    float percent = 0.0f;
    Camera camera;

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    qDebug() << percent << "%";

    while(!finished)
    {
        QPixmap pixmap = camera.computeImage(500, finished, percent);
        ui->progressBar->setValue(percent);
        ui->label->setPixmap(pixmap);

        qDebug() << percent << "%";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
