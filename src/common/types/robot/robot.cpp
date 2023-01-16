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

#include "robot.h"

#include <src/common/utils/utils.h>

using namespace Suassuna;

Robot::Robot(const Common::Enums::Color& teamColor, const quint8& robotId) : Common::Types::Object(Geometry::Vector2D(0.0, 0.0), Geometry::Vector2D(0.0, 0.0), Geometry::Vector2D(0.0, 0.0)) {
    _teamColor = teamColor;
    _robotId = robotId;
}

Armorial::RobotIdentifier Robot::identifier() {
    return Suassuna::Utils::buildIdentifierFromArgs(teamColor(), robotId());
}

Common::Enums::Color Robot::teamColor() {
    return _teamColor;
}

quint8 Robot::robotId() {
    return _robotId;
}
