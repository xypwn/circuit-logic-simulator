#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString loadFile;
    if(argc >= 2)
        loadFile = argv[1];
    MainWindow w(nullptr, loadFile);
    w.show();
    return a.exec();
}
