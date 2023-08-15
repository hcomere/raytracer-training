#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <Camera.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    int render();

private:
    void closeEvent(QCloseEvent *a_event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Ui::MainWindow *m_ui;
    bool m_exit;
    bool m_imageComputationIsFinished;
    std::unique_ptr<Camera> m_camera;
};
#endif // MAINWINDOW_H
