#ifndef ROTATECOMMAND_H
#define ROTATECOMMAND_H
#include <Command/command.h>
class rotateCommand : public command
{
    Q_OBJECT
public:
    explicit rotateCommand(QObject *parent = 0);
    rotateCommand(QObject *parent = 0,int distance=0);
    void execute(robot *myRobot,int distance);
    void execute(robot *myRobot){execute(myRobot,getDistance());}
signals:

public slots:

};

#endif // ROTATECOMMAND_H
