#include "skill_kick.h"
#include <entity/player/skills/skills_include.h>

QString Skill_Kick::name() {
    return "Skill_Kick";
}

Skill_Kick::Skill_Kick() {
    _isPass = false;
    _aimPosition = Position(false, 0.0, 0.0, 0.0);

    _aimPosition.setUnknown();
}

void Skill_Kick::run() {
    if(_aimPosition.isUnknown())
        return;

    player()->goToLookTo(player()->position().x(), player()->position().y(), loc()->ball().x(), loc()->ball().y(), _aimPosition.x(), _aimPosition.y(), player()->orientation().value(), 0.01);
    player()->Kick(_isPass);
}
