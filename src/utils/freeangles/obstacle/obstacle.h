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

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <src/utils/utils.h>

class Obstacle
{
public:
    Obstacle(float obstacleRadius = 0.09f);

    // Getters
    Position position() { return _obstaclePosition; }
    Angle initialAngle() { return _initialAngle; }
    Angle finalAngle() { return _finalAngle; }
    float radius() { return _obstacleRadius; }

    // Setters
    void setPosition(Position position) { _obstaclePosition = position; }
    void setInitialAngle(Angle initialAngle) { _initialAngle = initialAngle; }
    void setFinalAngle(Angle finalAngle) { _finalAngle = finalAngle; }
    void setRadius(float radius) { _obstacleRadius = radius; }

    void calcAnglesFrom(const Position &watcher, float radiusFactor = 1.0f);

    Obstacle &operator=(Robot robot);
    Obstacle &operator=(Ball ball);

private:
    Position _obstaclePosition;
    Angle _initialAngle;
    Angle _finalAngle;
    float _obstacleRadius;

    // Angle control
    void setInitialAngle(float value);
    void setFinalAngle(float value);
};

#endif // OBSTACLE_H
