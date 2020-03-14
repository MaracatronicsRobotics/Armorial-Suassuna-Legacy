#include "skill_donothing.h"
#include <entity/player/skills/skills_include.h>

QString Skill_DoNothing::name() {
    return "Skill_DoNothing";
}

Skill_DoNothing::Skill_DoNothing() {
    posicao = new Position(false, 0, 0, 0);
}

void Skill_DoNothing::run() {
    player()->idle();
}
