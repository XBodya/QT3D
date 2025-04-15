#include "viewport.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Viewport w;
    // w.show();
    MainWindow w;
    w.show();
    return a.exec();
}
