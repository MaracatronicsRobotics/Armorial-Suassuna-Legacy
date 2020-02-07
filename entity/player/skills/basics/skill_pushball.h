#ifndef SKILL_PUSHBALL_H
#define SKILL_PUSHBALL_H

#include <entity/player/skills/skill.h>

class Skill_PushBall : public Skill {
private:
    // Parameters
    Position _destination;
    float _maxPushDistance;

    // State machine
    enum {
        STATE_POS,
        STATE_PUSH
    };
    int _state;

    // Internal
    Position _currPos;
    Position _lastPos;
    float _distPushed;
    void run();
    bool isBehindBall(Position posObjective);
    bool isBallInFront();
public:
    Skill_PushBall();
    QString name();
    void setDestination(const Position &destination) { _destination = destination; }
    void setMaxPushDistance(float maxPushDistance) { _maxPushDistance = maxPushDistance; }
};

#endif // SKILL_PUSHBALL_H
