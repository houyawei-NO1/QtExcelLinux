#include <QCoreApplication>
#include "qtexcellinux.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtExcelLinux excel;
    return a.exec();
}
