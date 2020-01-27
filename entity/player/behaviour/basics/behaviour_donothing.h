#ifndef BEHAVIOUR_DONOTHING_H
#define BEHAVIOUR_DONOTHING_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_DoNothing : public Behaviour {
private:
    void configure();
    void run();
public:
    Behaviour_DoNothing();
    QString name();
    Skill_AroundTheBall* _teste;
};

#endif // BEHAVIOUR_DONOTHING_H
