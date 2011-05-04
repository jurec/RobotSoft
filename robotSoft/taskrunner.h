#ifndef TASKRUNNER_H
#define TASKRUNNER_H

#include <QObject>
#include <Command/moveforwardcommand.h>
#include <QVector>
#include <Command/command.h>
#include <QQueue>
#include <robot.h>
#include <QDebug>
class taskRunner : public QObject
{
    Q_OBJECT

public:
    explicit taskRunner(QObject *parent = 0);

    void runTasks(robot *myRobot);
    void addTask(command* com){tasks.enqueue(com);}

signals:

public slots:
private:
    QQueue<command*> tasks;
};

#endif // TASKRUNNER_H
