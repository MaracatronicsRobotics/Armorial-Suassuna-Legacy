#ifndef ROLE_DEFAULT_H
#define ROLE_DEFAULT_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behaviour/behaviours.h>

class Role_Default : public Role
{
public:
    Role_Default();

    // Inherited from Role
    QString name();
    void initializeBehaviours();

private:
    // Inherited from Role
    void configure();
    void run();

    // Behaviour pointers and enumerate
    Behaviour_GoToLookTo* _bhv_gotolookto;

    enum {
        BEHAVIOR_GOTOLOOKTO
    };

    enum {
        PID_TEST
    };

    int _actualState;
    Position _positionToGo;
    Position _positionToLook;

    // PID Test
    Position _endPos;
    bool _testRunning;
    int _testType;
    Velocity _playerVel;
    Acceleration _playerAcc;

    enum {
        VX,
        VY,
        VW
    };
};

#endif // ROLE_DEFAULT_H
