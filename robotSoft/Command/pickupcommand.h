#ifndef PICKUPCOMMAND_H
#define PICKUPCOMMAND_H
#include <Command/command.h>
class pickUpCommand : public command
{
    Q_OBJECT
public:
    explicit pickUpCommand(QObject *parent = 0);
    void execute(robot *myRobot,int distance){}
    void execute(robot *myRobot);
signals:

public slots:

};

#endif // PICKUPCOMMAND_H
