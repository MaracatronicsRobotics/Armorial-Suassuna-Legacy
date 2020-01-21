#ifndef SKILL_ROTATETO_H
#define SKILL_ROTATETO_H

#include <entity/player/skills/skill.h>

class Skill_RotateTo : public Skill {
private:
    void run();
public:
    Skill_RotateTo();
    QString name();
};

#endif // SKILL_ROTATETO_H
