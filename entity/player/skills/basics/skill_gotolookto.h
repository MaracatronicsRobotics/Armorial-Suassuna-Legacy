#ifndef SKILL_GOTOLOOKTO_H
#define SKILL_GOTOLOOKTO_H

#include <entity/player/skills/skill.h>

class Skill_GoToLookTo : public Skill {
private:
    void run();
public:
    Skill_GoToLookTo();
    QString name();
};

#endif // SKILL_GOTOLOOKTO_H
