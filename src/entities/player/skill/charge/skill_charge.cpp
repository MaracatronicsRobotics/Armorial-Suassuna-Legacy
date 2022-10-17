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

#include "skill_charge.h"

#define ROBOT_MIN_VELOCITY_TO_CONSIDER_STUCK 0.1
#define ROBOT_DIST_TO_WALL_TO_CONSIDER_STUCK 0.15
#define CHECK_STUCK_ROUTINE_DELAY 2.0

Skill_Charge::Skill_Charge() {
    _useSwappedOri = false;
}

void Skill_Charge::configure() {

}

void Skill_Charge::run() {
    // Check if robot has stucked in some point
    std::vector<Geometry::LineSegment> fieldBoundary = getWorldMap()->getField().field().boundary();
    for (auto &ls : fieldBoundary) {
        if(ls.distanceToPoint(player()->getPosition()) <= ROBOT_DIST_TO_WALL_TO_CONSIDER_STUCK
                && player()->getVelocity().length() <= ROBOT_MIN_VELOCITY_TO_CONSIDER_STUCK) {
            _useSwappedOri = !_useSwappedOri;
            break;
        }
    }

    player()->charge(_useSwappedOri);
}
