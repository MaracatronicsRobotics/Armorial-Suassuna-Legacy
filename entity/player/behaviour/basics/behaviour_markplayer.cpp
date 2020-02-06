#include "behaviour_markplayer.h"

QString Behaviour_MarkPlayer::name() {
    return "Behaviour_MarkPlayer";
}

Behaviour_MarkPlayer::Behaviour_MarkPlayer() {
    _markBetweenBall = true;
    _targetID = -1;
    _markDistance = 0.5;
}

void Behaviour_MarkPlayer::configure() {
    usesSkill(_sk_goto = new Skill_GoToLookTo());
};

void Behaviour_MarkPlayer::run() {
    /* avoid segfaults */
    if(_targetID == -1){
        printf("[BEHAVIOUR MARKPLAYER] Target not set in MarkPlayer (Player id: %u)\n", player()->playerId());
        return ;
    }
    if(PlayerBus::theirPlayerAvailable(quint8(_targetID)) == false){
        printf("[BEHAVIOUR MARKPLAYER] Target set is not available in MarkPlayer (Player id: %u)\n", player()->playerId());
        return ;
    }

    //

    Position _desiredPosition;
    Position _aimPosition;
    Position _targetPosition = PlayerBus::theirPlayer(quint8(_targetID))->position();

    if(_markBetweenBall){
        _desiredPosition = WR::Utils::threePoints(_targetPosition, loc()->ball(), _markDistance, 0.0);
        _aimPosition = WR::Utils::threePoints(loc()->ball(), _targetPosition, _markDistance, GEARSystem::Angle::pi);
    }else{
        _desiredPosition = WR::Utils::threePoints(_targetPosition, loc()->ourGoal(), _markDistance, 0.0);
        _aimPosition = WR::Utils::threePoints(loc()->ourGoal(), _targetPosition, _markDistance, GEARSystem::Angle::pi);
    }

    _sk_goto->setDesiredPosition(_desiredPosition);
    _sk_goto->setAimPosition(_aimPosition);

}
