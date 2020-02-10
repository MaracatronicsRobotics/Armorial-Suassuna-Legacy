#ifndef BAHAVIOUR_MARKBALL_H
#define BAHAVIOUR_MARKBALL_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_MarkBall : public Behaviour {
private:
    void configure();
    void run();
    int _state;
    Skill_GoToLookTo *_sk_GoToLookTo;

public:
    Behaviour_MarkBall();
    QString name();
};

#endif // BAHAVIOUR_MARKBALL_H
