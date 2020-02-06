#include "behaviour_attacker.h"

#define KICK_POWER 8.0f
#define RECEIVER_INVALID_ID 200

QString Behaviour_Attacker::name() {
    return "Behaviour_Attacker";
}

Behaviour_Attacker::Behaviour_Attacker() {
}

void Behaviour_Attacker::configure() {
    usesSkill(_sk_kick = new Skill_Kick());
    usesSkill(_sk_goto = new Skill_GoToLookTo());

    addTransition(0, _sk_goto, _sk_kick);
    addTransition(1, _sk_kick, _sk_goto);
    setInitialSkill(_sk_goto);

    _state = STATE_ATTACK;


    _timer = new MRCTimer(1000.0f);
};

void Behaviour_Attacker::run() {

    _sk_kick->setIsPass(false);

    if(player()->canKickBall() == false){ // if can't kick ball
        //_state = STATE_WAIT;
    }

    switch(_state){
    case STATE_WAIT: {
        Position waitPosition;

        /* condicoes aqui para que o atacante não voe encima da bola */
        /* mandar ele ficar a uma distancia, sei la */

        if(player()->canKickBall()){
            _state = STATE_ATTACK;
        }
    }
    break;
    case STATE_ATTACK:{
        enableTransition(1);
        _bestReceiver = RECEIVER_INVALID_ID;
        _kickPosition = loc()->ourGoal();

        _bestReceiver = getBestReceiver();
        if(_bestReceiver != RECEIVER_INVALID_ID){
            Position receiverPosition = PlayerBus::ourPlayer(_bestReceiver)->position();
            if(receiverPosition.isUnknown() == false){
                _kickPosition = receiverPosition;
            }
        }

        Position behindBall = WR::Utils::threePoints(loc()->ball(), _kickPosition, 0.2, GEARSystem::Angle::pi); // por trás da bola
        if(_bestReceiver != RECEIVER_INVALID_ID && player()->distBall() < 0.3f && isBehindBall(behindBall) == false)
            emit goingToShoot(_bestReceiver);

        _sk_goto->setDesiredPosition(behindBall);
        _sk_goto->setAimPosition(_kickPosition);

        // change state to kick
        Angle anglePlayerBall = player()->angleTo(loc()->ball());
        float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());
        if (diff <= atan(0.7)){ // can kick (behind ball, certainly) (atan(0.7) = 35deg
            _state = STATE_KICK;
            _timer->update();
        }
    }
    break;
    case STATE_KICK:{
        enableTransition(1); // move transition
        if(_bestReceiver != RECEIVER_INVALID_ID){ // if shoot is for a receiver
            if(PlayerBus::ourPlayerAvailable(_bestReceiver)){ // if receiver is available already
                _kickPosition = PlayerBus::ourPlayer(_bestReceiver)->position(); // update kickPosition (delay from loop)
            }
        }

        if(player()->isLookingTo(_kickPosition, 0.1)){ // 0.06 is angle error
            //_sk_kick->setIsPass(true);
            _sk_kick->setAim(_kickPosition);
            enableTransition(0); // shoot transiction
        }

        Angle anglePlayerBall = player()->angleTo(loc()->ball());
        float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());
        if (!(diff <= atan(0.7)) || player()->distBall() > 0.4f){ // isn't in front or ball shooted (atan(0.7) = 35deg
            if(_bestReceiver != RECEIVER_INVALID_ID)
                emit shooted(_bestReceiver);

            _state = STATE_ATTACK;
        }

        if(_timer->getTimeInSeconds() >= 0.2){ // reposition of player after 0.2 sec
            _state = STATE_ATTACK;
        }
    }
    break;
    }


}

quint8 Behaviour_Attacker::getBestReceiver(){
    quint8 bestRcv = RECEIVER_INVALID_ID;
    double dist = INFINITY;
    for(int x = 0; x < _recvs.size(); x++){
        if(PlayerBus::ourPlayerAvailable(_recvs.at(x))){
            double distToAtk = sqrt(pow(player()->position().x() - PlayerBus::ourPlayer(_recvs.at(x))->position().x(), 2) + pow(player()->position().y() - PlayerBus::ourPlayer(_recvs.at(x))->position().y(), 2));
            if(distToAtk < dist){
                dist = distToAtk;
                bestRcv = _recvs.at(x);
            }
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
