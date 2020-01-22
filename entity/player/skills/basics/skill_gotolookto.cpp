#include "skill_gotolookto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GoToLookTo::name() {
    return "Skill_GoToLookTo";
}

Skill_GoToLookTo::Skill_GoToLookTo() {
    _desiredPosition = Position(0.0, 0.0, 0.0, true);
}

void Skill_GoToLookTo::run() {
    player()->goToLookTo(player()->position().x(), player()->position().y(), _desiredPosition.x(), _desiredPosition.y(), player()->orientation().value(), _distBall);
}
