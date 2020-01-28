#include "skill_gkick.h"
#include <entity/player/skills/skills_include.h>

QString Skill_GKick::name() {
    return "Skill_Kick";
}

Skill_GKick::Skill_GKick() {
}

void Skill_GKick::run() {
    player()->goToLookTo(player()->position().x(), player()->position().y(), loc()->ball().x(), loc()->ball().y(), loc()->ball().x(), loc()->ball().y(), player()->orientation().value(), 0.1);
    player()->Kick(_isPass);
}
