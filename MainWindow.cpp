#include <MainWindow.h>
#include <ui_mainwindow.h>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QVector3D>
#include <QDebug>
#include <Camera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_exit(false)
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::closeEvent(QCloseEvent *a_event)
{
    m_exit = true;
}

int MainWindow::render()
{
    bool finished = false;
    float percent = 0.0f;
    Camera camera;

    m_ui->progressBar->setRange(0, 100);
    m_ui->progressBar->setValue(0);

    qDebug() << percent << "%";

    while(!m_exit)
    {
        QApplication::processEvents();

        while(!finished)
        {
            QPixmap pixmap = camera.computeImage(16, finished, percent);
            m_ui->progressBar->setValue(percent);
            m_ui->label->setPixmap(pixmap);

            qDebug() << percent << "%";
        }
    }

    qDebug() << "ciao";
    return 0;
}
