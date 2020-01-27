#include "behaviour_followball.h"

QString Behaviour_FollowBall::name() {
    return "Behaviour_FollowBall";
}

Behaviour_FollowBall::Behaviour_FollowBall() {
    _skill_GoToLookTo = NULL;
    _offsetBall = 0.6;
}

void Behaviour_FollowBall::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_FollowBall::run() {
    _skill_GoToLookTo->setDesiredPosition(loc()->ball());
    _skill_GoToLookTo->setOffsetToBall(_offsetBall);
}

