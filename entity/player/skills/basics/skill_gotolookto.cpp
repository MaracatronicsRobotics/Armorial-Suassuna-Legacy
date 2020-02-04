#include "skill_gotolookto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GoToLookTo::name() {
    return "Skill_GoToLookTo";
}

Skill_GoToLookTo::Skill_GoToLookTo() {
    _aimPosition = Position(true, 0.0, 0.0, 0.0);
    _desiredPosition = Position(true, 0.0, 0.0, 0.0);
    _offsetBall = 0.2;
}

void Skill_GoToLookTo::run() {
    player()->goToLookTo(player()->position().x(), player()->position().y(), _desiredPosition.x(), _desiredPosition.y(), _aimPosition.x(), _aimPosition.y(), player()->orientation().value(), _offsetBall);
}
