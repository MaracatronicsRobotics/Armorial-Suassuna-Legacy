#ifndef SKILL_DRIBBLE_H
#define SKILL_DRIBBLE_H

#include <entity/player/skills/skill.h>

class Skill_Dribble : public Skill {
private:
    void run();
public:
    Skill_Dribble();
    QString name();
};

#endif // SKILL_DRIBBLE_H
