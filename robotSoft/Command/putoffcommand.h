#ifndef PUTOFFCOMMAND_H
#define PUTOFFCOMMAND_H
#include <Command/command.h>
class putOffCommand : public command
{
    Q_OBJECT
public:
    explicit putOffCommand(QObject *parent = 0);
    void execute(robot *myRobot,int distance){}
    void execute(robot *myRobot);
signals:

public slots:

};

#endif // PUTOFFCOMMAND_H
