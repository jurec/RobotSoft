#ifndef TASKRUNNER_H
#define TASKRUNNER_H

#include <QObject>
#include <Command/moveforwardcommand.h>
#include <QVector>
#include <Command/command.h>
#include <QQueue>
class taskRunner : public QObject
{
    Q_OBJECT
public:
    explicit taskRunner(QObject *parent = 0);
    QQueue<command*> tasks;
    void runTasks();
    void addTask(command* com){tasks.push_back(com);}
signals:

public slots:
private:
};

#endif // TASKRUNNER_H
