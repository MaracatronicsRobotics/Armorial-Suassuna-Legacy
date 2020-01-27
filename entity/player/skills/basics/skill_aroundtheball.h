#ifndef SKILL_AROUNDTHEBALL_H
#define SKILL_AROUNDTHEBALL_H

#include <entity/player/skills/skill.h>

class Skill_AroundTheBall : public Skill {
private:
    void run();
    Position _desiredPosition;
    double _offsetBall;
public:
    Skill_AroundTheBall();
    QString name();

    void setDesiredPosition(Position pos) { _desiredPosition = pos; }
    void setOffsetToBall(double value) { _offsetBall = value; }
};

#endif // SKILL_AROUNDTHEBALL_H
