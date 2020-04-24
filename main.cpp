#include "diff_solver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Diff_solver w;
    w.show();
    return a.exec();
}
