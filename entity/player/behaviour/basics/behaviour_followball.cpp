#include "behaviour_followball.h"

<<<<<<< Updated upstream
behaviour_followball::behaviour_followball()
{

}
=======
QString Behaviour_FollowBall::name() {
    return "Behaviour_FollowBall";
}

Behaviour_FollowBall::Behaviour_FollowBall() {
    _skill_GoToLookTo = NULL;
    _distBall = 0.6;
}

void Behaviour_FollowBall::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_FollowBall::run() {
    _skill_GoToLookTo->setDesiredPosition(loc()->ball());
    _skill_GoToLookTo->setDistToBall(_distBall);
}

>>>>>>> Stashed changes
