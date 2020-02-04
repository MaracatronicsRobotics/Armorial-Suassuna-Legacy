#include "behaviour_donothing.h"

QString Behaviour_DoNothing::name() {
    return "Behaviour_DoNothing";
}

Behaviour_DoNothing::Behaviour_DoNothing() {
}

void Behaviour_DoNothing::configure() {
    usesSkill(_skill_doNothing = new Skill_DoNothing());
};

void Behaviour_DoNothing::run() {

}
