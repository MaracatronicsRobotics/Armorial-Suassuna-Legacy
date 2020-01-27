#include "skill_gotolookto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GoToLookTo::name() {
    return "Skill_GoToLookTo";
}

Skill_GoToLookTo::Skill_GoToLookTo() {
    _ballPosition = Position(true, 0.0, 0.0, 0.0);
    _desiredPosition = Position(true, 3.0, 3.0, 0.0);
}

void Skill_GoToLookTo::run() {
    player()->goToLookTo(player()->position().x(), player()->position().y(), _ballPosition.x(), _ballPosition.y(), _desiredPosition.x(), _desiredPosition.y(), player()->orientation().value(), _offsetBall);
}
