#ifndef BEHAVIOR_PUSH_H
#define BEHAVIOR_PUSH_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/skill/goto/skill_goto.h>
#include <src/entities/player/skill/rotateto/skill_rotateto.h>

class Behavior_Push : public Behavior
{
public:
    Behavior_Push();

private:
    // Behavior inherited methods
    void configure();
    void run();

    // Skills pointers
    Skill_GoTo *_skill_goTo;
    Skill_RotateTo *_skill_rotateTo;

    // Skills enum
    enum {
        SKILL_GOTO,
        SKILL_ROTATETO
    };

    // Behavior enum
    enum state{
        GOTO_STATE,
        ROTATE_STATE,
        PUSH_STATE
    };

    state _behaviorState;

    double _pushingDistance;
};

#endif // BEHAVIOR_PUSH_H
