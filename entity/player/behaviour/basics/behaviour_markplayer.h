#ifndef BEHAVIOUR_MARKPLAYER_H
#define BEHAVIOUR_MARKPLAYER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_MarkPlayer : public Behaviour {
private:
    void configure();
    void run();

    // Parameters
    bool _markBetweenBall;
    int _targetID;
    double _markDistance;

    // SKills
    Skill_GoToLookTo *_sk_goto;
public:
    Behaviour_MarkPlayer();
    QString name();

    void setMarkBetweenBall(bool markBetweenBall) { _markBetweenBall = markBetweenBall; }
    void setTargetID(int targetID) { _targetID = targetID; }
    void setMarkDistance(double markDistance) { _markDistance = markDistance; }
};

#endif // BEHAVIOUR_MARKPLAYER_H
