#ifndef COMMAND_H
#define COMMAND_H
#include <QObject>
#include <robot.h>
class command : public QObject
{
    Q_OBJECT
public:
    explicit command(QObject* parent);
    virtual ~command(){}
    virtual void execute(robot *myRobot,int distance)=0;
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
    TORQUE_ENABLE
    };
    void setName(QString s){name = s;}
   QString getName(){return name;}
  //  virtual void undo()=0;
public slots:
signals:
private:

    QString name;
};











#endif //COMMAND_H
