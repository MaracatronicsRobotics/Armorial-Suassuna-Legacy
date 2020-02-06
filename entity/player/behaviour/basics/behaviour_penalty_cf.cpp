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
    int _rightShoot;
    if (isGKRight()) _skill_goToLookTo->setAimPosition(loc()->theirGoalLeftPost());
    else _skill_goToLookTo->setAimPosition(loc()->theirGoalRightPost());
    _skill_goToLookTo->setOffsetToBall(0.05);
    loc()->theirGoalLeftPost().y();
    if (player()->distBall() < 0.05) enableTransition(STATE_KICK); //distBall deve ser trocado de acordo com o offset
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
