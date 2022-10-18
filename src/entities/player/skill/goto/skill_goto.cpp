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

#include "skill_goto.h"

#include <math.h>
#include <spdlog/spdlog.h>

#define ROBOT_MIN_VELOCITY_TO_CONSIDER_STUCK 0.1
#define ROBOT_DIST_TO_WALL_TO_CONSIDER_STUCK 0.15
#define CHECK_STUCK_ROUTINE_DELAY 0.5

Skill_GoTo::Skill_GoTo() {
    _useSwappedOri = false;
    _useWallAntiStuck = false;
}

void Skill_GoTo::setTargetPosition(const Geometry::Vector2D& targetPosition) {
    _targetPosition = targetPosition;
}

void Skill_GoTo::configure() {
    _checkStuckTimer.start();
}

void Skill_GoTo::run() {
    // Check if robot has stucked in some point
    if(_useWallAntiStuck) {
        if (_checkStuckTimer.getSeconds() >= CHECK_STUCK_ROUTINE_DELAY) {
            {
                std::vector<Geometry::LineSegment> fieldBoundary = getWorldMap()->getField().field().boundary();
                for (auto &ls : fieldBoundary) {
                    if(ls.distanceToPoint(player()->getPosition()) <= ROBOT_DIST_TO_WALL_TO_CONSIDER_STUCK
                            && player()->getVelocity().length() <= ROBOT_MIN_VELOCITY_TO_CONSIDER_STUCK) {
                        _useSwappedOri = !_useSwappedOri;
                        break;
                    }
                }
            }

            // reset delay
            _checkStuckTimer.start();
        }
    }
    else {
        _checkStuckTimer.start();
    }

    player()->goTo(_targetPosition, _useSwappedOri);
}
