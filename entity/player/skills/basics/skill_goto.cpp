#include "skill_goto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GoTo::name() {
    return "Skill_GoTo";
}

Skill_GoTo::Skill_GoTo() {
}

void Skill_GoTo::run() {
    player()->GoTo(player()->position().x(), player()->position().y(), _desiredPosition.x(), _desiredPosition.y(), player()->orientation().value());
}
