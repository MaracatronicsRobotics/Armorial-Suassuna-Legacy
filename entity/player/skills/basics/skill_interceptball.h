#ifndef SKILL_INTERCEPTBALL_H
#define SKILL_INTERCEPTBALL_H

#include <entity/player/skills/skill.h>

class Skill_InterceptBall : public Skill {
private:
    void run();
    bool _interceptAdvance;
    Position _posLook;
public:
    Skill_InterceptBall();
    QString name();

    void setInterceptAdvance(bool interceptAdvance) { _interceptAdvance = interceptAdvance; }
    void setPositionToLook(const Position &posLook) { _posLook = posLook; }
};

#endif // SKILL_INTERCEPTBALL_H
