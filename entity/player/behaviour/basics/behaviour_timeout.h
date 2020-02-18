#ifndef BEHAVIOUR_TIMEOUT_H
#define BEHAVIOUR_TIMEOUT_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_TimeOut : public Behaviour {
private:
    void configure();
    void run();
    Position _desiredPosition;
    Skill_GoToLookTo *_skill_GoToLookTo;
    double _offsetBall;

public:
    Behaviour_TimeOut();
    QString name();

    void setSkillPosition(Position pos);
};

#endif // BEHAVIOUR_TIMEOUT_H
