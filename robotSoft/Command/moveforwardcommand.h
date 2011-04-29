#ifndef MOVEFORWARDCOMMAND_H
#define MOVEFORWARDCOMMAND_H
#include <Command/command.h>
class moveForwardCommand : public command
{
    Q_OBJECT
public:
    explicit moveForwardCommand(QObject *parent = 0);
    void execute(robot *myRobot,int distance);
    void execute(robot *myRobot);
signals:

public slots:

};

#endif // MOVEFORWARDCOMMAND_H
