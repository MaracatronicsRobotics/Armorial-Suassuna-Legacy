#include "skill_rotateto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_RotateTo::name() {
    return "Skill_RotateTo";
}

Skill_RotateTo::Skill_RotateTo() {
}

void Skill_RotateTo::run() {
    player()->RotateTo(player()->position().x(), player()->position().y(), 3.0, 3.0, player()->orientation().value());
}
