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

#include "obstacle.h"
#include <utils/utils.hh>
#include <math.h>
#include <iostream>

Obstacle::Obstacle() {
    _position.setUnknown();
    _initialAngle = _finalAngle = 0;
    _radius = 0;
}

void Obstacle::calcAnglesFrom(const Position &watcher, float radiusFactor) {
    if(_radius==0) {
        std::cout << "[ERROR] Obstacle::calcAnglesFrom() with radius==0!\n";
        return;
    }

    const float R = radiusFactor*_radius;

    // Obstacle angulation
    const float dist = WR::Utils::distance(watcher, _position);
    const float fix = atan2(R, dist);

    // Calc angle
    float angle = WR::Utils::getAngle(watcher, _position);
    _initialAngle = angle - fix;
    _finalAngle = angle + fix;

    // Ensure initial and final angles are in 0-360 degree range
    WR::Utils::angleLimitZeroTwoPi(&_initialAngle);
    WR::Utils::angleLimitZeroTwoPi(&_finalAngle);
}
