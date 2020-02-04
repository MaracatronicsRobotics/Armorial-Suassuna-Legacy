#ifndef SKILL_KICK_H
#define SKILL_KICK_H

#include <entity/player/skills/skill.h>

class Skill_Kick : public Skill {
private:
    void run();
    bool _isPass;
    Position _aimPosition;

public:
    Skill_Kick();
    QString name();

    void setIsPass(bool isPass) { _isPass = isPass; }
    void setAim (Position pos) { _aimPosition = pos; }
};

#endif // SKILL_KICK_H
