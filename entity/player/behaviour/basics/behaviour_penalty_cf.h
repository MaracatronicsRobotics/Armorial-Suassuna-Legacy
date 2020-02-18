#ifndef BEHAVIOUR_PENALTY_CF_H
#define BEHAVIOUR_PENALTY_CF_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>
#include <utils/freeangles/freeangles.h>

class Behaviour_Penalty_CF : public Behaviour {
private:
    void configure();
    void run();
    Skill_GoToLookTo *_skill_goToLookTo;
    Skill_Kick *_skill_kick;
    int _state;
    Position _kickPosition;
    Position getBestKickPosition();

    // Auxiliary
    bool isGKRight();

    // Skill transitions
    enum{
        STATE_GOTO,
        STATE_KICK
    };

public:
    Behaviour_Penalty_CF();
    QString name();
};
#endif // BEHAVIOUR_PENALTY_CF_H
