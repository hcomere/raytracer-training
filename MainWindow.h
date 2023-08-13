#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *m_ui;
    bool m_exit;
};
#endif // MAINWINDOW_H
