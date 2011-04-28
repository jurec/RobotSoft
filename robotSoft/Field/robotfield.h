#ifndef ROBOTFIELD_H
#define ROBOTFIELD_H

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QVector>
#include <Field/cell.h>
#include <QGraphicsView>
class robotField : public QGraphicsView
{
    Q_OBJECT
public:

    explicit robotField(QGraphicsView *parent = 0);
    //void drawField();
    void changeFlag();
    ~robotField();
protected:
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void mousePressed();
public slots:
    void drawRobot();
   // void drawRobot();
  //  void drawPion();
   // void drawEnemy();
private:
    void initZonesSize();
    void drawField();
    static const int N=8;// ��� ���� �������� �� ����� 35�35, ��������� ���� ���������� 40�40
   //� ����� ���������� ������ ������ 8�6(� ������ ��������� ��� � ��� �������)
    static const int M=6;//M - ���-�� ������� ������ �� ����������� � ���������
    cell field[N][M];
    bool flag;
  //  QPainter *myP;
    QGraphicsScene *scene;
    QGraphicsRectItem *robot;    
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
