#ifndef ROBOTFIELD_H
#define ROBOTFIELD_H

#include <Field/cell.h>
#include <QGraphicsView>
#include <QVector>
#include <robotparameters.h>
//#include <
class robotField : public QGraphicsView
{
    Q_OBJECT
public:
    explicit robotField(QGraphicsView *parent = 0);
    ~robotField();
    static qreal degrees(qreal radians){return (180*radians)/M_PI;}
    static qreal radians(qreal degrees){return (M_PI*degrees)/180;}
    void drawRobot(robotParameters *robotParameter);
//    void drawRobot(QPointF &center);
//    void drawRobot(qreal xCenter,qreal yCenter);
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
   // void mousePressed();
public slots:
   // void moveRobot(int x,int y);
    void moveRobot(int distance);
    void rotateRobot(int angle);

  //  cell[8][6] getField(){}
//    void placePion(int x,int y);
   // void drawRobot();
  //  void drawPion();
   // void drawEnemy();
  //  QVector< QVector<cell> > getField(){return field_;}
    //cell *getField(){return field;}
private:
    void calculateCenter();
    void initZonesSize();
    void drawRobot();
    void drawField();
    static const int N=8;// Все поле разделим на части 35х35, стартовые зоны остануться 40х40
   //в итоге получиться массив клеток 8х6(с учетом стартовых зон и зон раздачи)
    static const int M=6;//M - кол-во игровых клеток по горизонатли и вертикали
    typedef QVector<cell> cellRow;
    typedef QVector<cellRow> fieldType;
    // QVector< QVector<cell> > field_;
    cell field[N][M];
    bool flag;
  //  QPainter *myP;
    QGraphicsScene *scene;//ПОЛЕ
    QGraphicsRectItem *robot;   //РОБОТ
    QVector<QGraphicsItem*> *pions;//ПЕШКИ
    QPointF *robotCenter;//Центр робота, вокруг которого вращаемся
    QRectF *robotRect;//Сам робот
    bool isRed;//Цвет влияет на угол в котором появится робот
    //QSize *robotSize;//размер робота

    QSize cellSize;
    QSize startzoneSize;
    QSize greenzoneSize;
    QSize blackLineSize;

    QPoint cellCoordinates[M][M];
    QPoint startzones[2];
    QPoint greenzones[2];
    QPoint blackLines[2];
};

#endif // ROBOTFIELD_H
