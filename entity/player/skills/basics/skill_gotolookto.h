#ifndef SKILL_GOTOLOOKTO_H
#define SKILL_GOTOLOOKTO_H

#include <entity/player/skills/skill.h>

class Skill_GoToLookTo : public Skill {
private:
    void run();
    Position _desiredPosition;
    Position _aimPosition;
    double _offsetBall;
public:
    Skill_GoToLookTo();
    QString name();

    void setDesiredPosition(Position pos) { _desiredPosition = pos; }
    void setAimPosition(Position pos) { _aimPosition = pos; }
    void setOffsetToBall(double value) { _offsetBall = value; }
};

#endif // SKILL_GOTOLOOKTO_H
