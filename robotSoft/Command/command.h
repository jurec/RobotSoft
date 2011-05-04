#ifndef COMMAND_H
#define COMMAND_H
#include <QObject>
#include <robot.h>
class command : public QObject
{
    Q_OBJECT
public:
    explicit command(QObject* parent);
    //command(QObject *parent,int distance);
    virtual ~command(){}
    //virtual void execute(robot *myRobot,int distance)=0;
    virtual void execute(robot *myRobot)=0;
    enum roboCommands{
    FORWARD=1,BACKWARD,LEFT,RIGHT,
    TURN,
    STOP,
    BEGIN_STATE=1,
    READY_CATCH,
    CATCH_PAWN,
    PAWN_UP,
    PAWN_DOWN,
    RELEASE,
    CATCH_UP=7,
    DOWN_RELEASE=8,
    TORQUE_OFF=10,
    TORQUE_ENABLE,
    DOWN_ON_PION=12
    };
    void setName(QString s){name = s;}
    void setDistance(int d){dist=d;}
    int getDistance(){return dist;}
    QString getName(){return name;}
  //  virtual void undo()=0;
public slots:
signals:
private:
    int dist;
    QString name;
};











#endif //COMMAND_H
