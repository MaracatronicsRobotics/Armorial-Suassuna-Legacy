#ifndef SKILL_ROTATETO_H
#define SKILL_ROTATETO_H

#include <entity/player/skills/skill.h>

class Skill_RotateTo : public Skill {
private:
    void run();
    Position _desiredPosition;
public:
    Skill_RotateTo();
    QString name();

    void setDesiredPosition(Position pos) { _desiredPosition = pos; }
};

#endif // SKILL_ROTATETO_H
