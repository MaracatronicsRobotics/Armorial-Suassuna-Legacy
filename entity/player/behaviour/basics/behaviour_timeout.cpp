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

#include "behaviour_timeout.h"

QString Behaviour_TimeOut::name() {
    return "Behaviour_TimeOut";
}

Behaviour_TimeOut::Behaviour_TimeOut() {
    _skill_GoToLookTo = NULL;
    _alreadyReachedPosition = false;
}

void Behaviour_TimeOut::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_doNothing = new Skill_DoNothing());

    addTransition(ENABLE_HALT, _skill_GoToLookTo, _skill_doNothing);
    addTransition(ENABLE_POS, _skill_doNothing, _skill_GoToLookTo);
};

void Behaviour_TimeOut::run() {
    // Getting aim to look to "the front of the robot"
    Position aimPos = player()->position();
    aimPos = Position(true, aimPos.x() + (loc()->ourSide().isLeft() ? 0.19f : -0.19f), aimPos.y(), aimPos.z());

    if(player()->distanceTo(_desiredPosition) <= 0.01f)
        _alreadyReachedPosition = true;

    if(!_alreadyReachedPosition)
        enableTransition(ENABLE_POS);
    else
        enableTransition(ENABLE_HALT);

    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
    _skill_GoToLookTo->setAimPosition(aimPos);
    _skill_GoToLookTo->setAvoidBall(true);
    _skill_GoToLookTo->setAvoidOpponents(true);
    _skill_GoToLookTo->setAvoidTeammates(true);
    _skill_GoToLookTo->setAvoidOurGoalArea(false);

    player()->dribble(false);
}

void Behaviour_TimeOut::reset(){
    _alreadyReachedPosition = false;
}

void Behaviour_TimeOut::getPlayerTimeOutId(){
    Position initialPos = loc()->ourGoalLeftPost();
    initialPos = Position(true, initialPos.x() + (loc()->ourSide().isLeft() ? 1.1f : -1.1f), initialPos.y() + (loc()->ourSide().isLeft() ? -0.5f : 0.5f), initialPos.z());

    // Get the player number in the array of available players
    int decision = player()->playerId();
    /*
    int decision = 0;
    QList<Player *> players = loc()->getMRCPlayers().values();
    for(int x = 0; x < players.size(); x++){
        if(players.at(x)->playerId() == player()->playerId()){
            decision = x;
            break;
        }
    }
    */

    // Adjust position
    initialPos = Position(true, initialPos.x(), initialPos.y() - (decision * 0.39f), initialPos.z());

    _desiredPosition = initialPos;
}
