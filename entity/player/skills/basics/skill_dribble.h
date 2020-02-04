#ifndef SKILL_DRIBBLE_H
#define SKILL_DRIBBLE_H

#include <entity/player/skills/skill.h>

class Skill_Dribble : public Skill {
private:
    void run();
    bool _isActive;
public:
    Skill_Dribble();
    QString name();

    void setActive(bool isActive) { _isActive = isActive; }
};

#endif // SKILL_DRIBBLE_H
