#include "behaviour_donothing.h"

QString Behaviour_DoNothing::name() {
    return "Behaviour_DoNothing";
}

Behaviour_DoNothing::Behaviour_DoNothing() {
}

void Behaviour_DoNothing::configure() {
    usesSkill(_teste = new Skill_Kick());
    usesSkill(_sk_goto = new Skill_GoToLookTo());

    addTransition(0, _sk_goto, _teste);
    addTransition(1, _teste, _sk_goto);
    setInitialSkill(_sk_goto);


};

void Behaviour_DoNothing::run() {
    _teste->setIsPass(false);

    _sk_goto->setOffsetToBall(0.12);
    _sk_goto->setBallPosition(Position(true, loc()->ball().x() - 0.1, loc()->ball().y() + 0.1, 0.0));
    _sk_goto->setDesiredPosition(loc()->theirGoal());

    double modDist = sqrt(pow((player()->position().x() - loc()->ball().x()), 2) + pow((player()->position().y() - loc()->ball().y()), 2));

    if(modDist <= 0.12){
        enableTransition(0);
    }else{
        enableTransition(1);
    }
}

