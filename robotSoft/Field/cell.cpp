#include "cell.h"

cell::cell(QObject *parent) :
    QObject(parent)
{
}
cell::cell(QColor col, QObject *parent):
            QObject(parent), cellColor(col)
    {

    }
