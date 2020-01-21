#ifndef SKILL_GOTO_H
#define SKILL_GOTO_H

#include <entity/player/skills/skill.h>

class Skill_GoTo : public Skill {
private:
    void run();
public:
    Skill_GoTo();
    QString name();
};

#endif // SKILL_GOTO_H
