#include "skill_kick.h"
#include <entity/player/skills/skills_include.h>

QString Skill_Kick::name() {
    return "Skill_Kick";
}

Skill_Kick::Skill_Kick() {
    _isPass = false;
}

void Skill_Kick::run() {
    player()->Kick(_isPass);
}
