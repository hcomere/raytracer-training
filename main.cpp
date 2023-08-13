#include <MainWindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    MainWindow w;
    w.show();
    return w.render();
}
