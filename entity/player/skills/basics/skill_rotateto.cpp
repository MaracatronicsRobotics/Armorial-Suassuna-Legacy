#include "skill_rotateto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_RotateTo::name() {
    return "Skill_RotateTo";
}

Skill_RotateTo::Skill_RotateTo() {
    _desiredPosition = Position(true, 3.0, 3.0, 0.0);
}

void Skill_RotateTo::run() {
    player()->RotateTo(player()->position().x(), player()->position().y(), _desiredPosition.x(), _desiredPosition.y(), player()->orientation().value());
}
