#ifndef SKILL_GOALKEEPER_H
#define SKILL_GOALKEEPER_H

#include <entity/player/skills/skill.h>

class Skill_GoalKeeper : public Skill {
private:
    void run();
    bool _interceptAdvance;
    Position _posLook;
public:
    Skill_GoalKeeper();
    QString name();

    void setInterceptAdvance(bool interceptAdvance) { _interceptAdvance = interceptAdvance; }
    void setPositionToLook(const Position &posLook) { _posLook = posLook; }
};

#endif // SKILL_GOALKEEPER_H
