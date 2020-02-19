#ifndef BEHAVIOUR_GOALKEEPER_H
#define BEHAVIOUR_GOALKEEPER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Goalkeeper : public Behaviour {
private:
    void configure();
    void run();
    Skill_InterceptBall *_skill_Goalkeeper;
    Skill_GoToLookTo *_skill_goToLookTo;
    Skill_Kick *_skill_gkick;
    double _offsetBall;

    // Parameters
    float _radius;
    bool _takeoutEnabled;
    float _takeoutFactor;
    bool _useAttackerOri;

    // Auxiliary
    Position getAttackerInterceptPosition();
    Position calcAttackerBallImpact();
    bool isBallComingToGoal(float minSpeed, float postsFactor = 1.0);

    // Skill transitions
    enum{
        STATE_GOTO,
        STATE_KICK,
        STATE_GK
    };


public:
    Behaviour_Goalkeeper();
    QString name();

    void setRadius(float radius) { _radius = radius; }
    void setTakeoutEnabled(bool enable) { _takeoutEnabled = enable; }
    void setTakeoutFactor(float takeoutFactor) { _takeoutFactor = takeoutFactor; }
    void useAttackerOri(bool useAttackerOri) { _useAttackerOri = useAttackerOri; }
};

#endif // BEHAVIOUR_GOALKEEPER_H
