#ifndef PUTONPIONCOMMAND_H
#define PUTONPIONCOMMAND_H
#include <Command/command.h>
class putOnPionCommand : public command
{
    Q_OBJECT
public:
    explicit putOnPionCommand(QObject *parent = 0);
    void execute(robot *myRobot,int distance){}
    void execute(robot *myRobot);
signals:

public slots:

};

#endif // PUTONPIONCOMMAND_H
