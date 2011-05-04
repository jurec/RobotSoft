#ifndef MOVEBACKWARDCOMMAND_H
#define MOVEBACKWARDCOMMAND_H
#include <Command/command.h>
#include <QObject>
class moveBackwardCommand : public command
{
    Q_OBJECT
public:
    explicit moveBackwardCommand(QObject *parent = 0);
    moveBackwardCommand(QObject *parent = 0,int distance=0);
    void execute(robot *myRobot,int distance);
    void execute(robot *myRobot){execute(myRobot,getDistance());}
signals:

public slots:
private:
};

#endif // MOVEBACKWARDCOMMAND_H
