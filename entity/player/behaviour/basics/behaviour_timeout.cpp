#include "behaviour_timeout.h"
#include <entity/player/skills/skills_include.h>

QString Behaviour_TimeOut::name() {
    return "Behaviour_FollowBall";
}

Behaviour_TimeOut::Behaviour_TimeOut() {
    _skill_GoToLookTo = NULL;
    _distBall = 0.1;
    _desiredPosition = Position(0.0, 0.0, 0.0, true);
}

void Behaviour_TimeOut::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_TimeOut::run() {
    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
    _skill_GoToLookTo->setDistToBall(_distBall);
}

void Behaviour_TimeOut::setSkillPosition(Position pos){
    _desiredPosition = pos;
}
