#ifndef ROBOTPARAMETERS_H
#define ROBOTPARAMETERS_H
//Класс служит для передачи парметров робота
#include <QObject>
#include <QRect>
#include <QRectF>
#include <QMatrix>
#include <qmath.h>
class robotParameters : public QObject
{
    Q_OBJECT
public:
    explicit robotParameters(QObject *parent = 0);
    void setRobotRect(QRectF rect){robotModel=rect; robotModelMatrix.mapRect(robotModel);}
    void setRobotCenter(QPointF center){robotCenter=center; robotCenterMatrix.map(robotCenter);}

    void setWidth(int w){robotModel.setWidth(w);}
    void setHeight(int h){robotModel.setHeight(h);}
    void setAngle(int a){angle=a;}
    QRectF getRobotModel(){return robotModel;}
    QPointF getRobotCenter(){return robotCenter;}
    QMatrix getRobotCenterMatrix(){return robotCenterMatrix;}
    QMatrix getRobotModelMatrix(){return robotModelMatrix;}

    void setTeamRed(){red=true;}
    void setTeamBlue(){red=false;}
    bool isRed(){return red;}
    int getAngle(){return angle;}
    void rotateRobot(int angle); //функция поворота менятся угол и вращаются матрицы

    void moveRobot(int distance);

signals:

public slots:   
private:
    int angle;//угол поворта
    QRectF robotModel;//Тут храняться начальные координаты робота его размеры(длина, ширина)
    bool red;//хранится цвет команды 0-синий 1-красный
    QPointF robotCenter;//хранятся координаты центра робота, относительно которых он разворачивается
    QMatrix robotCenterMatrix;//матрицы поворота для робота и его центральной точки
    QMatrix robotModelMatrix;//инициализируются в конструкторе
};

#endif // ROBOTPARAMETERS_H
