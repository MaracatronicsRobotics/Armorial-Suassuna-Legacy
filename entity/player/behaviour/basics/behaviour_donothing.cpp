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

    _state = STATE_GOTO;
};

void Behaviour_DoNothing::run() {

    _sk_goto->setDesiredPosition(Position(true, loc()->ball().x(), loc()->ball().y(), 0.0));
    _sk_goto->setAimPosition(loc()->ourGoal());

    double modDist = sqrt(pow((player()->position().x() - loc()->ball().x()), 2) + pow((player()->position().y() - loc()->ball().y()), 2));

    switch(_state){
    case STATE_GOTO:
        _sk_goto->setOffsetToBall(0.15);
        if(modDist <= 0.2){
            _teste->setIsPass(false);
            enableTransition(0);
            _state = STATE_KICK;
        }
    break;
    case STATE_KICK:
        if(loc()->ballVelocity().abs() >= 0.3){
            enableTransition(1);
            _state = STATE_GOTO;
        }else{
            _sk_goto->setOffsetToBall(0.01);
            enableTransition(1);
            _state = STATE_GOTO;
        }
    break;
    }
}

