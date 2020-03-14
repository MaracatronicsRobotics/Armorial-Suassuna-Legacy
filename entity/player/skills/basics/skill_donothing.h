#ifndef SKILL_DONOTHING_HH
#define SKILL_DONOTHING_HH

#include <entity/player/skills/skill.h>

class Skill_DoNothing : public Skill {
private:
    void run();

    Position *posicao;
public:
    Skill_DoNothing();
    QString name();
};

#endif // SKILL_DONOTHING_HH
