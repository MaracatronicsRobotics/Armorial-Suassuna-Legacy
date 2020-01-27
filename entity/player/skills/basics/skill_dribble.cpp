#include "skill_dribble.h"
#include <entity/player/skills/skills_include.h>

QString Skill_Dribble::name() {
    return "Skill_DoNothing";
}

Skill_Dribble::Skill_Dribble() {
}

void Skill_Dribble::run() {
    player()->Dribble();
}
