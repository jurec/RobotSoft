#include "cell.h"

cell::cell(QObject *parent) :
    QObject(parent)
{
}
cell::cell(QColor col, QObject *parent):
            QObject(parent), cellColor(col)
    {

    }
void cell::setCoords(int x1, int y1, int x2, int y2)
{
    QRect::setCoords(x1,y1,x2,y2);
}
