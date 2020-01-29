#include "behaviour_attacker.h"

QString Behaviour_Attacker::name() {
    return "Behaviour_Attacker";
}

Behaviour_Attacker::Behaviour_Attacker() {
}

void Behaviour_Attacker::configure() {
    usesSkill(_teste = new Skill_Kick());
    usesSkill(_sk_goto = new Skill_GoToLookTo());

    addTransition(0, _sk_goto, _teste);
    addTransition(1, _teste, _sk_goto);
    setInitialSkill(_sk_goto);

    _state = STATE_GOTO;
};

void Behaviour_Attacker::run() {

    Position _testPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 0.1, GEARSystem::Angle::pi);
    _sk_goto->setDesiredPosition(_testPosition);
    //_sk_goto->setDesiredPosition(Position(true, loc()->ball().x(), loc()->ball().y(), 0.0));
    _sk_goto->setAimPosition(loc()->ourGoal());

    double modDist = sqrt(pow((player()->position().x() - loc()->ball().x()), 2) + pow((player()->position().y() - loc()->ball().y()), 2));

    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, loc()->ourGoal());
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);
    bool isBehindBall = (diff>GEARSystem::Angle::pi/2.0f);

    switch(_state){
    case STATE_GOTO:
        _sk_goto->setOffsetToBall(0.15);
        if(isBehindBall){
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
