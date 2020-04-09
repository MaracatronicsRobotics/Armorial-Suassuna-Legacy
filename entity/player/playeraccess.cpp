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

#include "playeraccess.h"
#include <entity/player/player.h>

PlayerAccess::PlayerAccess(bool self, Player *player, Locations *loc) {
    _self = self;
    _player = player;
    _loc = loc;
}

PlayerAccess::~PlayerAccess() {

}

void PlayerAccess::kick(float power) {
    if(_self==false)
        return;
    _player->kick(false);
}

quint8 PlayerAccess::playerId() const {
    return _player->playerId();
}

quint8 PlayerAccess::teamId() const {
    return _player->teamId();
}

quint8 PlayerAccess::opTeamId() const {
    return _player->opTeamId();
}

MRCTeam* PlayerAccess::team() const {
    return _player->playerTeam();
}

Position PlayerAccess::position() const {
    return _player->position();
}

Position PlayerAccess::nextPosition() const {
    return _player->nextPosition();
}

Angle PlayerAccess::orientation() const {
    return _player->orientation();
}

Angle PlayerAccess::nextOrientation() const {
    return _player->nextOrientation();
}

Velocity PlayerAccess::velocity() const {
    return _player->velocity();
}

float PlayerAccess::lastSpeed() const {
    return _player->lastSpeed();
}

Angle PlayerAccess::nextDirection() const {
    return _player->nextDirection();
}

bool PlayerAccess::isAtPosition(const Position &position) const {
    return _player->isAtPosition(position);
}

bool PlayerAccess::isNearbyPosition(const Position &pos, float error) const {
    return _player->isNearbyPosition(pos, error);
}

bool PlayerAccess::isLookingTo(const Position &position) const {
    return _player->isLookingTo(position);
}

bool PlayerAccess::isLookingTo(const Position &pos, float error) const {
    return _player->isLookingTo(pos, error);
}

bool PlayerAccess::hasBallPossession() const {
    return _player->hasBallPossession();
}

bool PlayerAccess::canKickBall() const {
    return _player->canKickBall();
}

float PlayerAccess::distanceTo(const Position &pos) const {
    return _player->distanceTo(pos);
}

float PlayerAccess::distBall() const {
    return _player->distBall();
}

float PlayerAccess::distOurGoal() const {
    return _player->distOurGoal();
}

float PlayerAccess::distTheirGoal() const {
    return _player->distTheirGoal();
}

Angle PlayerAccess::angleTo(const Position &pos) const {
    return _player->angleTo(pos);
}

QString PlayerAccess::roleName(){
    return _player->getRoleName();
}
/*
float PlayerAccess::lError() const {
    return _player->lError();
}

float PlayerAccess::aError() const {
    return _player->aError();
}

void PlayerAccess::setLinearPIDParameters(double kp, double ki, double kd, double ilimit) {
    _player->setLinearPIDParameters(kp, ki, kd, ilimit);
}

void PlayerAccess::setAngularPIDParameters(double kp, double ki, double kd, double ilimit) {
    _player->setAngularPIDParameters(kp, ki, kd, ilimit);
}

void PlayerAccess::setMaxLSpeed(float maxLSpeed) {
    _player->setMaxLSpeed(maxLSpeed);
}

void PlayerAccess::setMaxLAcceleration(float maxLAcceleration) {
    _player->setMaxLAcceleration(maxLAcceleration);
}
*/
