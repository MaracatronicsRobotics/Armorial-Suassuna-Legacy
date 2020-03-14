#ifndef SKILL_SAMPLE_HH
#define SKILL_SAMPLE_HH

#include <entity/player/skills/skill.h>

class Skill_Sample : public Skill {
private:
    void run();
public:
    Skill_Sample();
    QString name();
};

#endif // SKILL_SAMPLE_HH
