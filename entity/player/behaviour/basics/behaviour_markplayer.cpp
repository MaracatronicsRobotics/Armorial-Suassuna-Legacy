/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

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
