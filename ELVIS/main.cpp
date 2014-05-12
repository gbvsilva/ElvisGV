#include "MainWindow.h"
#include <QApplication>

/**
 * Função principal.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
