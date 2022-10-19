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

#ifndef PLAYER_H
#define PLAYER_H

#include <src/entities/worldmap/worldmap.h>
#include <src/entities/controller/controller.h>
#include <src/entities/referee/sslreferee.h>
#include <src/entities/player/PID/pid.h>
#include <src/entities/player/PID/anglepid.h>

class Role;

class Player : public Common::Types::Object, public Threaded::Entity
{
public:
    /*!
     * \brief Player constructor.
     * \param playerId, teamColor Player identifiers.
     */
    Player(const quint8 playerId, const Common::Enums::Color& teamColor, VSSReferee* referee, WorldMap* worldMap, Controller* controller, bool useSimEnv);

    /*!
     * \brief Player params getters
     */
    Common::Enums::Color teamColor();
    quint8 playerId();
    bool canEnterGoalArea() { return _canEnterGoalArea; }

    /*!
     * \brief Player params setters
     */
    void setCanEnterGoalArea(bool canEnter) { _canEnterGoalArea = canEnter; }

    /*!
     * \brief Mark player as idle, setting its speeds to zero.
     */
    void idle();

    /*!
     * \brief goTo
     * \param position
     */
    void goTo(const Geometry::Vector2D& target, const float& swap = false);

    /*!
     * \brief rotateTo
     * \param position
     */
    void rotateTo(const Geometry::Angle& targetAngle);

    /*!
     * \brief Charges, that's it!
     */

    void move(float leftWheelPower, float rightWheelPower);

    void charge(const bool deCostinha);

    /*!
     * \brief kick
     * \param kickSpeed
     * \param chipKickAngle
     * \param kickAngle
     */

    void kick(const float& kickSpeed, const float& chipKickAngle = 0, const float& kickAngle = 0);

    /*!
     * \brief dribble
     * \param dribbling
     */
    void dribble(const bool& dribbling);

    /*!
     * \brief spin
     * \param clockWise1
     */
    void spin(const bool& clockWise, int wheelSpeed = 40);

    // Role management
    QString roleName();
    QString behaviorName();
    void setRole(Role *role);

protected:
    friend class SSLTeam;

    /*!
     * \brief Update this Player class with a given Common::Types::Object containing
     * the data.
     * \param playerData The given data to update this Player instance.
     */
    void updatePlayer(Common::Types::Object playerData);

private:
    void initialization();
    void loop();
    void finalization();

    // Player vars
    quint8 _playerId;
    Common::Enums::Color _teamColor;
    PID *_vxPID;
    PID *_vyPID;
    AnglePID *_vwPID;

    // Internal
    WorldMap *_worldMap;
    VSSReferee *_referee;
    Controller *_controller;

    // Idle control
    Utils::Timer _idleTimer;
    bool firstIt;

    // Role management
    Role *_playerRole;
    QMutex _mutexRole;

    bool _useSimEnv;
    bool _canEnterGoalArea;

};

#endif // PLAYER_H
