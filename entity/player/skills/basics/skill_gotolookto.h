#ifndef SKILL_GOTOLOOKTO_H
#define SKILL_GOTOLOOKTO_H

#include <entity/player/skills/skill.h>

class Skill_GoToLookTo : public Skill {
private:
    void run();
    Position _desiredPosition;
    double _distBall;
public:
    Skill_GoToLookTo();
    QString name();

    void setDesiredPosition(Position pos) { _desiredPosition = pos; }
    void setDistToBall(double value) { _distBall = value; }
};

#endif // SKILL_GOTOLOOKTO_H
