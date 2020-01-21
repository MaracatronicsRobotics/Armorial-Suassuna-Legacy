#include "behaviour_donothing.h"
#include <entity/player/skills/skills_include.h>

QString Behaviour_DoNothing::name() {
    return "Behavior_DoNothing";
}

Behaviour_DoNothing::Behaviour_DoNothing() {
}

void Behaviour_DoNothing::configure() {
    usesSkill(new Skill_GoToLookTo());
};

void Behaviour_DoNothing::run() {
}

