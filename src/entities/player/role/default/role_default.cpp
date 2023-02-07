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

#include "role_default.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Role_Default::Role_Default() {
}

void Role_Default::configure() {
    // Starting behaviors
    _behavior_default = new Behavior_Default();
    _behavior_move = new Behavior_Move();
    _behavior_intercept = new Behavior_Intercept();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_DEFAULT, _behavior_default);
    addBehavior(BEHAVIOR_MOVE, _behavior_move);
    addBehavior(BEHAVIOR_INTERCEPT, _behavior_intercept);
}

void Role_Default::run() {
    // Test logic for the 3 main skills ocurring simultaneously
    if(player()->robotId() == 0) {
        // Move the robot to other positions and see if it moves to the target position using the best face no move
        _behavior_move->setMovementType(Behavior_Move::MOVEMENT::MOVE);
        _behavior_move->setTargetPosition(getWorldMap()->getBall().getPosition());
        setBehavior(BEHAVIOR_MOVE);
    } else if (player()->robotId() == 1) {
        // Move the robot to other positions and see if it rotate to the target position using the smarter rotation diretion route
        _behavior_move->setMovementType(Behavior_Move::MOVEMENT::ROTATE);
        _behavior_move->setTargetAngle((getWorldMap()->getBall().getPosition() - player()->getPosition()).angle());
        setBehavior(BEHAVIOR_MOVE);
    } else {
        // Move the robot to positions where you can see both spin directions
        _behavior_intercept->setInterceptSegment(Geometry::LineSegment(Geometry::Vector2D(0.4f, -0.5f), Geometry::Vector2D(0.4f, 0.5f)));
        _behavior_intercept->setObjectPosition(getWorldMap()->getBall().getPosition());
        _behavior_intercept->setObjectVelocity(getWorldMap()->getBall().getVelocity());
        _behavior_intercept->setIntersectionAccuracy(0.3f);
        setBehavior(BEHAVIOR_INTERCEPT);
    }
}
