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

    Position _objective;
    // por trás da bola
    Position _testPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 0.1, GEARSystem::Angle::pi);
    _sk_goto->setDesiredPosition(_testPosition);
    _sk_goto->setAimPosition(loc()->ourGoal());
    _objective = loc()->ourGoal();

    double modDist = sqrt(pow((player()->position().x() - loc()->ball().x()), 2) + pow((player()->position().y() - loc()->ball().y()), 2));

    quint8 bestReceiver = getBestReceiver();
    if(bestReceiver != 200){
        _sk_goto->setDesiredPosition(WR::Utils::threePoints(loc()->ball(), PlayerBus::ourPlayer(bestReceiver)->position(), 0.1, GEARSystem::Angle::pi));
        _sk_goto->setAimPosition(PlayerBus::ourPlayer(bestReceiver)->position());
        _objective = PlayerBus::ourPlayer(bestReceiver)->position();
    }

    switch(_state){
    case STATE_GOTO:
        _sk_goto->setOffsetToBall(0.05);
        if(isBehindBall(_objective) && bestReceiver != 200){
            _teste->setIsPass(true);
            enableTransition(0);
            _state = STATE_KICK;
        }else if(isBehindBall(_objective)){
            _teste->setIsPass(false);
            enableTransition(0);
            _state = STATE_KICK;
        }
    break;
    case STATE_KICK:
        if(bestReceiver != 200){
            //emit goingToShoot(bestReceiver);
        }
        if(loc()->ballVelocity().abs() >= 0.3){
            //if(bestReceiver != 200) emit shooted(bestReceiver);
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

quint8 Behaviour_Attacker::getBestReceiver(){
    quint8 bestRcv = 200;
    double dist = INFINITY;
    for(int x = 0; x < _recvs.size(); x++){
        double distToAtk = sqrt(pow(player()->position().x() - PlayerBus::ourPlayer(_recvs.at(x))->position().x(), 2) + pow(player()->position().y() - PlayerBus::ourPlayer(_recvs.at(x))->position().y(), 2));
        if(distToAtk < dist){
            dist = distToAtk;
            bestRcv = _recvs.at(x);
        }
    }

    return bestRcv;
}

bool Behaviour_Attacker::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/2.0f);
}
