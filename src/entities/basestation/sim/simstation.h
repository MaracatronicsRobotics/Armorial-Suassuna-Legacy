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

#ifndef SIMSTATION_H
#define SIMSTATION_H

#include <Armorial/Base/Client/Client.h>

#include <src/entities/basestation/basestation.h>

class SimStation : public BaseStation, public Base::UDP::Client
{
public:
    /*!
     * \brief SimStation class constructor.
     */
    SimStation();

    /*!
     * \brief sendData Override from parent that describes how to send a given data using this actuator.
     * \param packet The given data.
     */
    void sendData(const Armorial::ControlPacket& packet);

    /*!
     * \brief sendZeroData Override from parent that sends a zero packet to a given identifier.
     * \param robotIdentifier The given identifier.
     */
    void sendZeroData(const Armorial::RobotIdentifier& robotIdentifier);

private:
    /*!
     * \brief Entity inherited method.
     */
    void initialization();
    void loop();
    void finalization();

    // Internal management of control packets
    QMutex _controlPacketsLock;
    QMap<quint8, Armorial::ControlPacket> _controlPackets;
};

#endif // SIMSTATION_H
