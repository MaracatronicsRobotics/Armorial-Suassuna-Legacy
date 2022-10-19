#ifndef ROLE_GOALKEEPER_H
#define ROLE_GOALKEEPER_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/behaviors.h>

class Role_Goalkeeper : public Role
{
public:
    Role_Goalkeeper();

private:
    // Role inherited methods
    void configure();
    void run();

    // Behaviors enum
    enum {
        BEHAVIOR_MOVETO,
        BEHAVIOR_CATCH
    };

    enum state {
        CATCH,
        ROTATE,
        MOVETO,
        SPIN
    };

    float limitGKSide(float desiredY);
    bool isInsideGoalArea();

    state _currState;
    Geometry::Vector2D _desiredPosition;

    // Behaviors pointers
    Behavior_MoveTo *_behavior_moveTo;
    Behavior_Catch *_behavior_catch;
};

#endif // ROLE_GOALKEEPER_H
