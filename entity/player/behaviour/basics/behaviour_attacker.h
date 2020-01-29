#ifndef BEHAVIOUR_ATTACKER_H
#define BEHAVIOUR_ATTACKER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Attacker : public Behaviour {
private:
    void configure();
    void run();
    int _state;
public:
    Behaviour_Attacker();
    QString name();

    Skill_Kick *_teste;
    Skill_GoToLookTo *_sk_goto;

    enum{
        STATE_GOTO,
        STATE_KICK
    };
};

#endif // BEHAVIOUR_ATTACKER_H
