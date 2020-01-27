#include "behaviour_donothing.h"
#include <entity/player/skills/skills_include.h>

QString Behaviour_DoNothing::name() {
    return "Behaviour_DoNothing";
}

Behaviour_DoNothing::Behaviour_DoNothing() {
}

void Behaviour_DoNothing::configure() {
    usesSkill(_teste = new Skill_AroundTheBall());
};

void Behaviour_DoNothing::run() {
    _teste->setOffsetToBall(0.75);
}

