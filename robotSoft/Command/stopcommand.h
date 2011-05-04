#ifndef STOPCOMMAND_H
#define STOPCOMMAND_H
#include<Command/command.h>
class stopCommand : public command
{
    Q_OBJECT
public:
    explicit stopCommand(QObject *parent = 0);
    stopCommand(QObject *parent = 0,int distance=0);
    void execute(robot *myRobot,int distance){execute(myRobot);}
    void execute(robot *myRobot);
signals:

public slots:

};

#endif // STOPCOMMAND_H
