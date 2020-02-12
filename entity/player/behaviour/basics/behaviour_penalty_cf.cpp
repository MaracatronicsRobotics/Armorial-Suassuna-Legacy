#include "behaviour_penalty_cf.h"
#include <entity/player/playerbus.h>

QString Behaviour_Penalty_CF::name() {
    return "Behaviour_Penalty_CF";
}

Behaviour_Penalty_CF::Behaviour_Penalty_CF() {
    _skill_kick = NULL;
    _skill_goToLookTo = NULL;
}

void Behaviour_Penalty_CF::configure() {
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());
    setInitialSkill(_skill_goToLookTo);

    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_GOTO, _skill_kick, _skill_goToLookTo);
}

void Behaviour_Penalty_CF::run() {

    _skill_kick->setIsPass(false);

    /*switch(_state){
    case STATE_ATTACK:{
        enableTransition(1);
        _kickPosition = loc()->ourGoal();

        Position behindBall = WR::Utils::threePoints(loc()->ball(), _kickPosition, 0.2, GEARSystem::Angle::pi); // por trás da bola

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

        if(player()->isLookingTo(_kickPosition, 0.1)){ // 0.1 is angle error
            //_sk_kick->setIsPass(true);
            _sk_kick->setAim(_kickPosition);
            enableTransition(0); // shoot transiction
        }

        Angle anglePlayerBall = player()->angleTo(loc()->ball());
        float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());
        if (!(diff <= atan(0.7)) || player()->distBall() > 0.4f){ // isn't in front or ball shooted (atan(0.7) = 35deg
            _state = STATE_ATTACK;
        }

        if(_timer->getTimeInSeconds() >= 0.05){ // reposition of player after 0.05 sec
            _state = STATE_ATTACK;
        }
    }
    break;
    }*/





    //Position behindBall = WR::Utils::threePoints(loc()->ball(), _kickPosition, 0.2, GEARSystem::Angle::pi); // por trás da bola
    Position Aim;
    if (isGKRight()) Aim = loc()->ourGoalLeftPost();
    else Aim = loc()->ourGoalRightPost();

    _skill_goToLookTo->setAimPosition(Aim);
    _skill_kick->setAim(Aim);
    _skill_goToLookTo->setOffsetToBall(0.2f);
    _skill_goToLookTo->setDesiredPosition(loc()->ball());
    loc()->theirGoalLeftPost().y();
    if (player()->distBall() < 0.2f) {
        enableTransition(STATE_KICK); //distBall deve ser trocado de acordo com o offset
    }
    else enableTransition(STATE_GOTO);
}

bool Behaviour_Penalty_CF::isGKRight() {
    int side;

    //Descomente para usar no campo
    /*if (loc()->ourSide().isLeft() == true) side = 1;
    else side = -1;*/

    //Descomente para usar no teste
    if (loc()->ourSide().isLeft() == true) side = -1;
    else side = 1;

    QList<Player*> opPlayers = loc()->getOpPlayers().values();
    QList<Player*>::iterator id;
    for(id = opPlayers.begin(); id != opPlayers.end(); id++){
        if(loc()->isInsideTheirArea((*id)->position())){
            bool _right;
            if (PlayerBus::ourPlayerAvailable((*id)->playerId()) == true) {
                if (side * (PlayerBus::theirPlayer((*id)->playerId())->position().y() - player()->position().y()) < 0.0) return true;
                else return false;
            }
        }
    }
}
