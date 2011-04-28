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
    virtual void execute(robot *myRobot)=0;
    virtual void undo()=0;
public slots:
signals:
private:
    QString name;
};











#endif //COMMAND_H
