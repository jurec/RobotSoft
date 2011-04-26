#include <QtCore/QCoreApplication>
#include <Field/robotfield.h>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    robotField myF;
    myF.show();
    return a.exec();
}
