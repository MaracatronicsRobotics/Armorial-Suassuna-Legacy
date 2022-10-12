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


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Armorial/Base/Client/Client.h>
#include <Armorial/Common/Enums/Color/Color.h>
#include <Armorial/Geometry/Vector2D/Vector2D.h>
#include <Armorial/Threaded/Entity/Entity.h>

#include <include/proto/actuatorservice.grpc.pb.h>

/*!
 * \brief The Controller class
 */
class Controller : public Threaded::Entity, public Base::GRPC::Client<Armorial::Actuator::ActuatorService>
{
public:
    /*!
     * \brief Controller
     */
    Controller(QString serviceAddress, quint16 servicePort);

protected:
    friend class Player;

    typedef struct {
        quint8 playerId;
        float vx, vy, vw;
        float wl, wr;
        float kickPower, chipKickAngle, kickAngle;
        bool dribbling;
        bool isUpdated;
    } ControlData;

    /*!
     * \brief setWheelsSpeed
     * \param playerId
     * \param wheelLeft
     * \param wheelRight
     */
    void setWheelsSpeed(const quint8& playerId, const float &wheelLeft, const float &wheelRight);

    /*!
     * \brief Set angular speed for a given robot.
     * \param playerId The given robot.
     * \param angularSpeed The given liner speed.
     */
    void setAngularSpeed(const quint8& playerId, const float& angularSpeed);

    /*!
     * \brief Set kick data for a given robot.
     * \param playerId The given robot.
     * \param kickSpeed, chipKickAngle, kickAngle The given kick data.
     */
    void setKick(const quint8& playerId, const float& kickSpeed, const float& chipKickAngle, const float& kickAngle);

    /*!
     * \brief Set dribbling for a given robot.
     * \param playerId The given robot.
     * \param dribbling The given dribbling status.
     */
    void setDribble(const quint8& playerId, const bool& dribbling);

private:
    void initialization();
    void loop();
    void finalization();

    /*!
     * \brief Initialize ControlData, allocating the required amount of robots.
     */
    void initializeData();

    /*!
     * \brief Transforms a given ControlData into a Armorial::ControlPacket instance.
     * \param controlData The given ControlData.
     */
    Armorial::ControlPacket getControlPacketFromControlData(const ControlData& controlData);

    ControlData *_controlData;
    QMutex _mutex;
};

#endif // CONTROLLER_H
