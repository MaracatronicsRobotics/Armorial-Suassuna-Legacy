#include "skill_gkick.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GKick::name() {
    return "Skill_GKick";
}

Skill_GKick::Skill_GKick() {
    _isPass = false;
    _aimPosition = loc()->ball();
}

void Skill_GKick::run() {
    player()->goToLookTo(player()->position().x(), player()->position().y(), loc()->ball().x(), loc()->ball().y(), _aimPosition.x(), _aimPosition.y(), player()->orientation().value(), 0.01);
    player()->kick(_isPass, 6.0);
}
