#ifndef ROBOTFIELD_H
#define ROBOTFIELD_H

#include <Field/cell.h>
#include <QGraphicsView>
#include <QVector>
#include <robotparameters.h>
class robotField : public QGraphicsView
{
    Q_OBJECT
public:
    explicit robotField(QGraphicsView *parent = 0);
    ~robotField();

    void drawRobot(robotParameters *robotParameter);
//    void drawRobot(QPointF &center);
//    void drawRobot(qreal xCenter,qreal yCenter);
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
   // void mousePressed();
public slots:
    void moveRobot(int x,int y);
    void rotateRobot(int angle);
//    void placePion(int x,int y);
   // void drawRobot();
  //  void drawPion();
   // void drawEnemy();
private:
    void calculateCenter();
    void initZonesSize();
    void drawRobot();
    void drawField();
    static const int N=8;// Все поле разделим на части 35х35, стартовые зоны остануться 40х40
   //в итоге получиться массив клеток 8х6(с учетом стартовых зон и зон раздачи)
    static const int M=6;//M - кол-во игровых клеток по горизонатли и вертикали
    cell field[N][M];
    bool flag;
  //  QPainter *myP;
    QGraphicsScene *scene;//ПОЛЕ
    QGraphicsRectItem *robot;   //РОБОТ
    QVector<QGraphicsItem*> *pions;//ПЕШКИ
    QPointF *robotCenter;//Центр робота, вокруг которого вращаемся
    QRect *robotRect;//Сам робот
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
