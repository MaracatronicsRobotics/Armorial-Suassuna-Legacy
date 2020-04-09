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

#include "navigation.h"
#include <entity/locations.h>
#include <entity/player/player.h>
#include <entity/contromodule/mrcteam.h>

Navigation::Navigation(Player *player, NavAlgorithm *navAlg) {
    _player = player;
    _navAlg = navAlg;
    _ourTeam = _player->playerTeam();
    _opTeam = _ourTeam->opTeam();
    _loc = _player->playerTeam()->loc();
    if(_navAlg!=NULL)
        _navAlg->initialize(_loc);
}

Navigation::~Navigation() {
    if(_navAlg!=NULL)
        delete _navAlg;
}

void Navigation::setGoal(const Position &destination, const Angle &orientation, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea) {
    // Reset algorithm
    _navAlg->reset();
    // Set origin position and orientation
    _navAlg->setOrigin(_player->position(), _player->orientation(), _player->velocity());
    // Sets goal position and orientation
    _navAlg->setGoal(destination, orientation);
    // Add ball as repulsive
    if(avoidBall)
        _navAlg->addBall(_loc->ball(), _loc->ballVelocity());
    // Add teammates as repulsive
    if(avoidTeammates) {
        QList<Player*> avPlayers = _ourTeam->avPlayers().values();
        for(int i=0; i<avPlayers.size(); i++) {
            Player *teammate = avPlayers.at(i);
            // Check if calling player
            if(teammate->playerId()==_player->playerId())
                continue;
            // Add robot as repulsive
            _navAlg->addOwnRobot(teammate->position(), teammate->velocity());
        }
    }
    // Adds opponents as repulsive
    if(avoidOpponents) {
        QList<Player*> avOpPlayers = _opTeam->avPlayers().values();
        for(int i=0; i<avOpPlayers.size(); i++) {
            Player *opponent = avOpPlayers.at(i);
            // Add robot as repulsive
            _navAlg->addEnemyRobot(opponent->position(), opponent->velocity());
        }
    }
    //Adds goal area as repulsive
    if(avoidOurGoalArea || avoidTheirGoalArea) {
        // Check restritions
        if(_loc->isOutsideField(_player->position()))
            return;
        if(_loc->isInsideOurArea(_player->position()))
            return;
        if(_loc->isInsideTheirArea(_player->position()))
            return;
        // Our goal area
        if(avoidOurGoalArea) {
            // Add our goal area
            Position far = WR::Utils::projectPointAtSegment(_loc->ourGoalLeftMidPost(), _loc->ourGoalRightMidPost(), _player->position());
            Position repulsion = WR::Utils::threePoints(far, _player->position(), _loc->fieldDefenseRadius(), 0.0f);
            _navAlg->addGoalArea(repulsion);
        }
        // Their goal area
        if(avoidTheirGoalArea) {
            // Add their goal area
            Position far = WR::Utils::projectPointAtSegment(_loc->theirGoalLeftMidPost(), _loc->theirGoalRightMidPost(), _player->position());
            Position repulsion = WR::Utils::threePoints(far, _player->position(), _loc->fieldDefenseRadius(), 0.0f);
            _navAlg->addGoalArea(repulsion);
        }
    }
}

Angle Navigation::getDirection() const {
    // Execute algorithm
    _navAlg->runNavAlgorithm();
    return _navAlg->getDirection();
}

float Navigation::getDistance() const {
    return _navAlg->getDistance();
}

QList<Position> Navigation::getPath() const {
    _navAlg->runNavAlgorithm();
    return _navAlg->getPath();
}
