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

    Skill_Kick *_teste;
    Skill_GoToLookTo *_sk_goto;
};

#endif // BEHAVIOUR_DONOTHING_H
