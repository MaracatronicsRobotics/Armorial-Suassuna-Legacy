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

Obstacle::Obstacle(float obstacleRadius) {
    _obstaclePosition = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    _initialAngle = Utils::getAngleObject(0.0f, false, true);
    _finalAngle = Utils::getAngleObject(0.0f, false, true);
    _obstacleRadius = obstacleRadius;
}

void Obstacle::calcAnglesFrom(const Position &watcher, float radiusFactor) {
    if (radius() == 0) {
        spdlog::error(Text::bold("Obstacle::calcAnglesFrom(args) with radius == 0!"));
        return;
    }

    const float R = radius() * radiusFactor;

    const float dist = Utils::distance(watcher, position());
    const float fix = atan2(R, dist);

    const float angle = Utils::getAngle(watcher, position());

    setInitialAngle(angle - fix);
    setFinalAngle(angle + fix);
}

void Obstacle::setInitialAngle(float value) {
    _initialAngle = Utils::getAngleObject(value, false, false);
}

void Obstacle::setFinalAngle(float value) {
    _finalAngle = Utils::getAngleObject(value, false, false);
}

Obstacle &Obstacle::operator=(Robot robot) {
    _obstaclePosition = robot.robotposition();
    return *this;
}

Obstacle &Obstacle::operator =(Ball ball){
    _obstaclePosition = ball.ballposition();
    return *this;
}
