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

#ifndef BASESTATION_H
#define BASESTATION_H

#include <QObject>

#include <Armorial/Threaded/Entity/Entity.h>

#include <include/proto/messages.pb.h>

/*!
 * \brief The BaseStation class
 */
class BaseStation : public Threaded::Entity
{
    Q_OBJECT
public:
    BaseStation() = default;
    virtual ~BaseStation() = default;

protected:
    friend class Player;

    /*!
     * \brief Virtual sendData that allows the children implementation to interpret and send the control packet
     *  to the robots.
     * \param packet The control packet which will be sent.
     */
    virtual void sendData(const Armorial::ControlPacket& packet) = 0;

    /*!
     * \brief Virtual sendZeroData that allows the children implementation to send a zero-control packet
     * to the robots.
     * \param robotIdentifier The robot identifier which will receive the data.
     */
    virtual void sendZeroData(const Armorial::RobotIdentifier& robotIdentifier) = 0;

private:
    /*!
     * \brief Threaded::Entity inherited methods.
     */
    virtual void initialization() = 0;
    virtual void loop() = 0;
    virtual void finalization() = 0;

signals:
    /*!
     * \brief sendFeedback method that allows the children implementations to cast and send the status packets
     * obtained from the robots.
     * \param robotStatus The robot status packet which will be sent.
     */
    void sendFeedback(const Armorial::RobotStatus& robotStatus);
};

#endif // BASESTATION_H
