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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Armorial/Common/Enums/Enums.h>
#include <Armorial/Utils/ParameterHandler/ParameterHandler.h>

#define ROBOT_INVALID_ID 200

namespace Suassuna {

class Constants
{
public:
    /*!
     * \brief Load a json file in the given file path, extracting its components.
     * \param fileName The given file path.
     */
    static void loadFile(QString filePath);

    /// Network
    // Referee
    /*!
     * \return A QString object containing the referee network address.
     */
    static QString refereeNetworkAddress();

    /*!
     * \return A quint16 object containing the referee network port.
     */
    static quint16 refereeNetworkPort();

    /*!
     * \return A QString object containing the referee network interface.
     */
    static QString refereeNetworkInterface();

    // WorldMap
    /*!
     * \return A QString object containing the vision address.
     */
    static QString visionAddress();

    /*!
     * \return A quint16 object containing the vision port.
     */
    static quint16 visionPort();

    /*!
     * \return A QString object containing the vision network interface.
     */
    static QString visionNetworkInterface();

    // Controller
    /*!
     * \return A QString object containing the actuator service address.
     */
    static QString actuatorAddress();

    /*!
     * \return A quint16 object containing the actuator service port.
     */
    static quint16 actuatorPort();

    /*!
     * \return A QString object containing the actuator network interface.
     */
    static QString actuatorNetworkInterface();

    /// Team
    /*!
     * \return A Common::Enums::Color containing our team color.
     */
    static Common::Enums::Color teamColor();

    /*!
     * \return A Common::Types::Side containing our play side.
     */
    static Common::Enums::Side teamPlaySide();

    /*!
     * \return A integer containing the maximum num of players (patterns).
     */
    static quint16 maxNumPlayers();

    /*!
     * \return Univector Field's Radius value.
     */
    static float univectorRadius();

    /*!
     * \return Univector Field's Kr value.
     */
    static float univectorKr();

    /*!
     * \return Univector Field's Delta value.
     */
    static float univectorDelta();

    /*!
     * \return Univector Field's D minimum.
     */
    static float univectorDmin();

    /*!
     * \return Univector Field's Ko.
     */
    static float univectorKo();

private:
    static Utils::ParameterHandler _parameterHandler;
};

}

#endif // CONSTANTS_H