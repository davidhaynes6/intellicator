#include "intellicator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Intellicator w;
    w.show();
    return a.exec();
}
