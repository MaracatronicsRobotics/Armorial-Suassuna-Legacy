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

#include "role_attacker.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Role_Attacker::Role_Attacker() {
}

void Role_Attacker::configure() {
    // Starting behaviors
    _behavior_default = new Behavior_Default();
    _behavior_move = new Behavior_Move();
    _behavior_intercept = new Behavior_Intercept();
    _behavior_push = new Behavior_Push();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_DEFAULT, _behavior_default);
    addBehavior(BEHAVIOR_MOVE, _behavior_move);
    addBehavior(BEHAVIOR_INTERCEPT, _behavior_intercept);
    addBehavior(BEHAVIOR_PUSH, _behavior_push);
}

void Role_Attacker::run() {
    // Chase the ball while distant from it
    if(player()->getPosition().dist(getWorldMap()->getBall().getPosition()) > 0.1f){
        _behavior_move->setTargetPosition(getWorldMap()->getBall().getPosition());
        _behavior_move->setMovementType(Behavior_Move::MOVE);
        setBehavior(BEHAVIOR_MOVE);
    }else{
        setBehavior(BEHAVIOR_PUSH);
    }
}
