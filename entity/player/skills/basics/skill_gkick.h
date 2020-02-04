#ifndef SKILL_GKICK_H
#define SKILL_GKICK_H

#include <entity/player/skills/skill.h>

class Skill_GKick : public Skill {
private:
    void run();
    bool _isPass;
    Position _aimPosition;
public:
    Skill_GKick();
    QString name();

    void setIsPass(bool isPass) { _isPass = isPass; }
    void setAimPosition(Position pos) { _aimPosition = pos; }
};

#endif // SKILL_GKICK_H
