#include "skill_goto.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GoTo::name() {
    return "Skill_GoTo";
}

Skill_GoTo::Skill_GoTo() {
    _desiredPosition = Position(true, 0.0, 0.0, 0.0);
}

void Skill_GoTo::run() {
    std::pair<double, double> pp = player()->goTo(player()->position().x(), player()->position().y(), _desiredPosition.x(), _desiredPosition.y(), player()->orientation().value());

    player()->setSpeed(pp.first, pp.second, 0.0);
}
