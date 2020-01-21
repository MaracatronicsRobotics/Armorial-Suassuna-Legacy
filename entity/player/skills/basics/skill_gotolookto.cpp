#include "skill_gotolookto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GoToLookTo::name() {
    return "Skill_LookTo";
}

Skill_GoToLookTo::Skill_GoToLookTo() {
}

void Skill_GoToLookTo::run() {
    player()->goToLookTo(player()->position().x(), player()->position().y(), 0.0, 0.0, player()->orientation().value());
}
