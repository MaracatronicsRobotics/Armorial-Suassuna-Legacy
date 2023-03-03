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

#include <src/entities/basesuassuna.h>
#include <src/common/types/robot/robot.h>
#include <src/entities/player/navigation/navigation.h>
#include <src/gui/gui.h>

class Player : public Suassuna::Robot, public Threaded::Entity
{
public:
    Player(const Common::Enums::Color& teamColor, const quint8& robotId, BaseStation* controller, WorldMap *worldMap);

    // Role management
    QString roleName();
    QString behaviorName();

    /*!
     * \brief setRole
     * \param role
     */
    void setRole(Role* role);
    void setGUI(GUI* gui);

protected:
    // Friend classes for update and internal control of the player
    friend class Team;
    friend class Skill;
    friend class Behavior;
    friend class Role;
    friend class Playbook;

    /*!
     * \brief Update this Player class with a given Common::Types::Object containing
     * the data.
     * \param playerData The given data to update this Player instance.
     */
    void updatePlayer(Common::Types::Object playerData);

    /*!
     * \brief Mark player as idle, setting its speeds to zero.
     */
    void idle();

    /*!
     * \brief Set this player wheels speed.
     */
    void setWheelsSpeed(const float& wheelLeft, const float& wheelRight);

    /*!
     * \return The Controller pointer of this player instance.
     */
    BaseStation* controller();

private:
    /*!
     * \brief Entity inherited methods.
     */
    void initialization();
    void loop();
    void finalization();

    // Role management
    Role *_playerRole;
    QMutex _mutexRole;

    // Control
    Armorial::ControlPacket _controlPacket;

    // Modules
    BaseStation *_controller;
    WorldMap *_worldMap;

    // Idle control
    Utils::Timer _idleTimer;
    bool _firstIt;

    GUI *_gui;

};

#endif // PLAYER_H
