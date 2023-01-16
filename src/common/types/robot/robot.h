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

#ifndef ROBOT_H
#define ROBOT_H

#include <Armorial/Common/Types/Object/Object.h>
#include <Armorial/Common/Enums/Color/Color.h>

#include <include/proto/messages.pb.h>

namespace Suassuna {

class Robot : public Common::Types::Object
{
public:
    Robot(const Common::Enums::Color& teamColor, const quint8& robotId);

    /*!
     * \brief Get this Robot instance identifier.
     */
    Armorial::RobotIdentifier identifier();

    /*!
     * \return The team color which this Robot belongs to.
     */
    Common::Enums::Color teamColor();

    /*!
     * \return This robot id.
     */
    quint8 robotId();

private:
    Common::Enums::Color _teamColor;
    quint8 _robotId;
};

}

#endif // ROBOT_H
