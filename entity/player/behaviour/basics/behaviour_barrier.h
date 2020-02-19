#ifndef BEHAVIOUR_BARRIER_H
#define BEHAVIOUR_BARRIER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Barrier : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    // Param
    float _d;
    float _radius;

    // Sk
    Skill_GoToLookTo *_sk_goto;
    Skill_InterceptBall *_sk_gk;
    Skill_Kick *_sk_kick;

    // Transitions
    enum{
        STATE_GOTO,
        STATE_GK,
        STATE_KICK
    };

    bool isBallComingToGoal(float minSpeed, float postsFactor = 1.0);

public:
    Behaviour_Barrier();
    QString name();

    void setD(float d) { _d = d; }
    void setRadius(float radius) { _radius = radius; }
};

#endif // BEHAVIOUR_BARRIER_H
