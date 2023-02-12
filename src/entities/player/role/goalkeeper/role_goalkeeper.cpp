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

#include "role_goalkeeper.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Role_Goalkeeper::Role_Goalkeeper() {
    _gkOverlap = false;
}

void Role_Goalkeeper::configure() {
    // Starting behaviors
    _behavior_move = new Behavior_Move();
    _behavior_intercept = new Behavior_Intercept();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_MOVE, _behavior_move);
    addBehavior(BEHAVIOR_INTERCEPT, _behavior_intercept);

    _state = ROTATE;
}

void Role_Goalkeeper::run() {
    // Ball relevant parameters
    Geometry::Vector2D ballVelocity = getWorldMap()->getBall().getVelocity();
    Geometry::Vector2D ballPosition = getWorldMap()->getBall().getPosition();

    // Defining position where the GK will await for new instrunctions
    Geometry::Vector2D standardPosition;
    if (getWorldMap()->getField().ourGoalCenter().x() > 0.0f) {
        standardPosition = Geometry::Vector2D(0.7f, 0.0f);
    } else {
        standardPosition = Geometry::Vector2D(-0.7f, 0.0f);
    }

    if (_gkOverlap) {
        // Defense routine on a penalty
        _behavior_move->setTargetPosition(getWorldMap()->getBall().getPosition());
        _behavior_move->setMovementType(Behavior_Move::MOVEMENT::MOVE);
        setBehavior(BEHAVIOR_MOVE);
    } else {
        // Defense routine on a normal game state
        Geometry::Rectangle ourGoalArea = getWorldMap()->getField().ourPenaltyArea();
        setBehavior(BEHAVIOR_MOVE);

        // Await in the defense line when located outside our goal area
        if (!ourGoalArea.contains(player()->getPosition())) {
            _behavior_move->setTargetPosition(standardPosition);
            _behavior_move->setMovementType(Behavior_Move::MOVEMENT::MOVE);
        } else {
            // Reference position to look at
            Geometry::Vector2D lookingPosition(standardPosition.x(), 2.0f);
            float angleDifference = fabs(player()->getOrientation().value()
                    - (lookingPosition - player()->getPosition()).angle());
            if (angleDifference > fabs(static_cast<float>(M_PI) - angleDifference)) {
                angleDifference = fabs(static_cast<float>(M_PI) - angleDifference);
            }

            // Align orientation with defense line when located in it. Then, intercept ball
            if (_state == ROTATE) {
                _behavior_move->setTargetAngle((lookingPosition - standardPosition).angle());
                _behavior_move->setMovementType(Behavior_Move::MOVEMENT::ROTATE);
                setBehavior(BEHAVIOR_MOVE);

                if (angleDifference < M_PI/36.0f) {
                    _state = INTERCEPT;
                }
            } else {
                Geometry::Vector2D upperDefenseLimit(standardPosition.x(), 0.3f);
                Geometry::Vector2D bottomDefenseLimit(standardPosition.x(), -0.3f);
                Geometry::LineSegment defenseLine(upperDefenseLimit, bottomDefenseLimit);
                _behavior_intercept->setInterceptSegment(defenseLine);
                _behavior_intercept->setObjectPosition(ballPosition);
                _behavior_intercept->setObjectVelocity(ballVelocity);
                _behavior_intercept->setIntersectionAccuracy(getIntersectionAccuracy(defenseLine));
                setBehavior(BEHAVIOR_INTERCEPT);

                if (angleDifference > M_PI/4.5f) {
                    _state = ROTATE;
                }
            }
        }
    }
}

float Role_Goalkeeper::getIntersectionAccuracy(Geometry::LineSegment interceptionSegment) {
    Geometry::Vector2D ballPosition = getWorldMap()->getBall().getPosition();
    float ballDangerDistance = Geometry::Line(interceptionSegment).distanceToLine(ballPosition);
    float ballVelocityMagnitude = getWorldMap()->getBall().getVelocity().length();

    return ballDangerDistance * (1.5f * ballVelocityMagnitude);
}
