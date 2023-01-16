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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#include <Armorial/Geometry/Geometry.h>
#include <Armorial/Common/Enums/Enums.h>
#include <Armorial/Common/Types/Traits/Traits.h>

#include <include/proto/messages.pb.h>

namespace Suassuna {

class Utils
{
public:
    /*!
     * \brief Check if the given color is blue.
     * \param color The given color.
     */
    static bool isColorBlue(const Common::Enums::Color& color);

    /*!
     * \brief Check if the given color is blue.
     * \param color The given color.
     */
    static bool isColorBlue(const Armorial::Color& color);

    /*!
     * \brief Transforms any given T type that has coordinates in a Geometry::Vector2D type.
     * \tparam coordinates The given T coordinate type.
     */
    template<typename T>
    static std::enable_if_t<Common::Types::has_coordinates_v<T>, Geometry::Vector2D> convertCoordinatesToVector2D(const T& coordinates) {
        Geometry::Vector2D vec(coordinates.x(), coordinates.y());
        return vec;
    }

    /*!
     * \brief Build Armorial::RobotIdentifier data type from given args.
     * \param teamColor, robotId The given args.
     */
    static Armorial::RobotIdentifier buildIdentifierFromArgs(const Common::Enums::Color& teamColor, const quint8& robotId);
};

}

#endif // UTILS_H
