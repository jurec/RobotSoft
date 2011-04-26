#ifndef ROBOTCOMMANDS_H
#define ROBOTCOMMANDS_H

#include <QObject>

class robotCommands : public QObject
{
    Q_OBJECT
public:
    explicit robotCommands(QObject *parent = 0);
    enum roboCommands{
    FORWARD=1,BACKWARD,LEFT,RIGHT,
    TURN,
    STOP,
    BEGIN_STATE=1,
    READY_CATCH,
    CATCH_PAWN,
    PAWN_UP,
    PAWN_DOWN,
    RELEASE,
    CATCH_UP=7,
    DOWN_RELEASE=8,
    TORQUE_OFF=10,
    TORQUE_ENABLE
    };

//   static char* getCommand(){return  "GET";}
signals:
public slots:

};

#endif // ROBOTCOMMANDS_H
