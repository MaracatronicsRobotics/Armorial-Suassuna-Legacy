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

#include <GEARSystem/Types/types.hh>

class Obstacle {
private:
    Position _position;
    quint8 _team;
    quint8 _id;
    float _initialAngle;
    float _finalAngle;
    float _radius;
public:
    Obstacle();

    Position& position() { return _position; }
    quint8& team() { return _team; }
    quint8& id() { return _id; }
    float& initialAngle() { return _initialAngle; }
    float& finalAngle() { return _finalAngle; }
    float& radius() { return _radius; }

    void calcAnglesFrom(const Position &watcher, float factor = 1.0f);

    Obstacle &operator=(const Obstacle &obstacle) {
        _position = obstacle._position;
        _team = obstacle._team;
        _id = obstacle._id;
        _initialAngle = obstacle._initialAngle;
        _finalAngle = obstacle._finalAngle;
        _radius = obstacle._radius;
        return *this;
    }
};

#endif // OBSTACLE_H
