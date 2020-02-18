#include "skill_aroundtheball.h"
#include <entity/player/skills/skills_include.h>

QString Skill_AroundTheBall::name() {
    return "Skill_AroundTheBall";
}

Skill_AroundTheBall::Skill_AroundTheBall() {
    _desiredPosition = Position(true, 0.0, 0.0, 0.0);
    _offsetBall = 0.1;
}

void Skill_AroundTheBall::run() {
    player()->aroundTheBall(player()->position().x(), player()->position().y(), _desiredPosition.x(), _desiredPosition.y(), player()->orientation().value(), _offsetBall);
}
