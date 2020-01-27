#ifndef SKILL_KICK_H
#define SKILL_KICK_H

#include <entity/player/skills/skill.h>

class Skill_Kick : public Skill {
private:
    void run();
    bool _isPass;
public:
    Skill_Kick();
    QString name();

    void setIsPass(bool isPass) { _isPass = isPass; }
};

#endif // SKILL_KICK_H
