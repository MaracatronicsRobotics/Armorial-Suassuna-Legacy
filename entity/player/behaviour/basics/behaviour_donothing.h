#ifndef BEHAVIOUR_DONOTHING_H
#define BEHAVIOUR_DONOTHING_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_DoNothing : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    Skill_DoNothing *_skill_doNothing;
public:
    Behaviour_DoNothing();
    QString name();
};

#endif // BEHAVIOUR_DONOTHING_H
