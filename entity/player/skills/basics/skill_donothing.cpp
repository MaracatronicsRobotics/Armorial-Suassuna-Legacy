#include "skill_donothing.h"
#include <entity/player/skills/skills_include.h>

QString Skill_DoNothing::name() {
    return "Skill_DoNothing";
}

Skill_DoNothing::Skill_DoNothing() {
}

void Skill_DoNothing::run() {
    player()->idle();
}
