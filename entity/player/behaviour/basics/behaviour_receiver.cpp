#include "behaviour_receiver.h"
#include <entity/player/playerbus.h>

#define DIST_TO_ATK 1.5f

QString Behaviour_Receiver::name() {
    return "Behaviour_Receiver";
}

Behaviour_Receiver::Behaviour_Receiver() {
    _skill_GoToLookTo = NULL;
    _attackerId = -1;
    _followAttacker = false;
}

void Behaviour_Receiver::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());

    _state = STATE_FOLLOW;
};

void Behaviour_Receiver::run() {
    if(_attackerId == -1){
        std::cout << "Attacker isn't set.\n" << std::endl;
    }

    Position _desiredPosition = player()->position();
    double modDistToAttacker = fabs(WR::Utils::distance(player()->position(), PlayerBus::ourPlayer(_attackerId)->position()));

    switch(_state){
    case STATE_FOLLOW:
        if(_followAttacker && modDistToAttacker > DIST_TO_ATK){
            _desiredPosition = PlayerBus::ourPlayer(_attackerId)->position();
        }else{
            _desiredPosition = player()->position();
            _state = STATE_STOP;
        }
    break;
    case STATE_STOP:
        _desiredPosition = player()->position(); // stay here
        if(_followAttacker && modDistToAttacker > DIST_TO_ATK){
            _state = STATE_FOLLOW;
        }
    break;
    }

    _skill_GoToLookTo->setAimPosition(PlayerBus::ourPlayer(_attackerId)->position());
    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);

}

void Behaviour_Receiver::goingToReceive(quint8 id){
    if(id == player()->playerId()){
        _state = STATE_FOLLOW; // tem q ser estado de recepção (interceptBall)
    }
}

void Behaviour_Receiver::attackerShooted(quint8 id){
    if(id == player()->playerId()){
        _state = STATE_STOP; // criar estado p verificar se ta em posicao boa p receber (mover caso n esteja)
    }
}
