#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (QApplication::arguments().size() > 1)   //если есть входной параметр
    {
        const QString filename = QApplication::arguments().at(1);
        w.PreloadFile(filename);
    }
    w.show();
    return a.exec();
}
