#include "intellicator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    intellicator w;
    w.show();
    return a.exec();
}
