#include "behaviour_timeout.h"

QString Behaviour_TimeOut::name() {
    return "Behaviour_FollowBall";
}

Behaviour_TimeOut::Behaviour_TimeOut() {
    _skill_GoToLookTo = NULL;
    _offsetBall = 0.01;
    _desiredPosition = Position(true, 0.0, 0.0, 0.0);
}

void Behaviour_TimeOut::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_TimeOut::run() {
    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
    _skill_GoToLookTo->setOffsetToBall(_offsetBall);
}

void Behaviour_TimeOut::setSkillPosition(Position pos){
    _desiredPosition = pos;
}
