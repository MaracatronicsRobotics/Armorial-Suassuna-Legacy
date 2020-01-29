#include "behaviour_receiver.h"
#include <entity/player/playerbus.h>

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
        if(_followAttacker && modDistToAttacker > 1.0){
            _desiredPosition = PlayerBus::ourPlayer(_attackerId)->position();
        }else{
            _desiredPosition = player()->position();
            _state = STATE_STOP;
        }
    break;
    case STATE_STOP:
        _desiredPosition = player()->position(); // stay here
        if(_followAttacker && modDistToAttacker > 1.0){
            _state = STATE_FOLLOW;
        }
    break;
    }

    _skill_GoToLookTo->setAimPosition(PlayerBus::ourPlayer(_attackerId)->position());
    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);

}

