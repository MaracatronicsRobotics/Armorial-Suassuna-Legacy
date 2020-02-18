#include "skill_dribble.h"
#include <entity/player/skills/skills_include.h>

QString Skill_Dribble::name() {
    return "Skill_Dribble";
}

Skill_Dribble::Skill_Dribble() {
    _isActive = false;
}

void Skill_Dribble::run() {
    player()->dribble(_isActive);
}
