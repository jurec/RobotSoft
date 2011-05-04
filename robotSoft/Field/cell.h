#ifndef CELL_H
#define CELL_H
#include <QObject>
#include <QRect>
#include <QtGui/QColor>
class cell : public QObject,public QRect
{
    Q_OBJECT
public:
   explicit cell(QObject *parent = 0);
    explicit cell(QColor col,QObject *parent=0);
    enum cellType{start,game,green,locked,bonus};//виды клеток - старторвая зона, игровая клетка, раздаточная зона, запрещенная зона, бонусная клетка
    enum state{EMPTY,MIXED,FULL};
    void setType(int type){cellType=type;}
    void setColor(QColor col){cellColor=col;}
    //void setSize(const QSize &s){setSize(s);}
    void setEmpty(){state=EMPTY;}
    void setFull(){state=FULL;}
    void setMixed(){state=MIXED;}
    //void setCoords(int x1, int y1, int x2, int y2);

signals:

public slots:
private:
    QColor cellColor;
    int state;
    int cellType;

};

#endif // CELL_H
