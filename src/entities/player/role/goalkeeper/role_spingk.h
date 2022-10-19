#ifndef ROLE_SPINGK_H
#define ROLE_SPINGK_H


#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/behaviors.h>

class Role_spinGK : public Role
{
public:
    Role_spinGK();

private:
    // Role inherited methods
    void configure();
    void run();

    enum state{
        MOVETO,
        SPIN,
        IDLE
    };

    // Behaviors enum
    enum {
        BEHAVIOR_DEFAULT,
        BEHAVIOR_MOVETO
    };

    state _currState;

    // Behaviors pointers
    Behavior_Default *_behavior_default;
    Behavior_MoveTo *_behavior_moveTo;

private:
    bool _spinOrientation;
};


#endif // ROLE_SPINGK_H
