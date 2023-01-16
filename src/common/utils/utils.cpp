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

#include "utils.h"

using namespace Suassuna;

bool Utils::isColorBlue(const Common::Enums::Color& color) {
    return (color == Common::Enums::Color::BLUE);
}

bool Utils::isColorBlue(const Armorial::Color &color) {
    return (color.isblue());
}

Armorial::RobotIdentifier Utils::buildIdentifierFromArgs(const Common::Enums::Color& teamColor, const quint8& robotId) {
    // Build identifier object
    Armorial::RobotIdentifier identifier;

    // Create and allocate color
    Armorial::Color* color = new Armorial::Color();
    color->set_isblue(isColorBlue(teamColor));
    identifier.set_allocated_robotcolor(color);

    // Allocate id
    identifier.set_robotid(robotId);

    return identifier;
}
