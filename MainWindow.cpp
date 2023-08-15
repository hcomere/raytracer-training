#include <MainWindow.h>
#include <ui_mainwindow.h>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QVector3D>
#include <QDebug>
#include <Camera.h>
#include <windows.h>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_exit(false)
{
    m_ui->setupUi(this);
    m_ui->drawSurface->installEventFilter(this);
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
    float percent = 0.0f;

    WId wid = m_ui->drawSurface->winId();
    int width = m_ui->drawSurface->width();
    int height = m_ui->drawSurface->height();

    m_camera.reset(new Camera((HWND)wid, width, height));

    m_ui->progressBar->setRange(0, 100);
    m_ui->progressBar->setValue(0);

    m_imageComputationIsFinished = false;

    while(!m_exit)
    {
        QApplication::processEvents();

        if(!m_imageComputationIsFinished)
        {
            m_camera->computeImage(100, m_imageComputationIsFinished, percent);
            m_ui->progressBar->setValue(percent);
        }
    }
    return 0;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_ui->drawSurface)
    {
        if(event->type() == QEvent::Resize)
        {
            if(m_camera != nullptr)
            {
                QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
                m_camera->setSize(resizeEvent->size().width(), resizeEvent->size().height());
                m_imageComputationIsFinished = false;
            }
        }
    }

    return false;
}
