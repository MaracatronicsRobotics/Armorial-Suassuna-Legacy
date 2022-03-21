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

#include <src/services/actuator/actuatorservice.h>
#include <src/services/coach/coachservice.h>
#include <src/utils/utils.h>
#include <src/entities/entity.h>

class Player : public Entity
{
public:
    Player(ActuatorService *actuator, CoachService *coach);

    Position getPlayerPos(int ID, bool isBlue);
    Angle getPlayerOrientation(int ID, bool isBlue);
    AngularSpeed getPlayerAngularSpeed(int ID, bool isBlue);
    Velocity getPlayerVelocity(int ID, bool isBlue);
    Acceleration getPlayerAcceleration(int ID, bool isBlue);
    RobotStatus getPlayerStatus(int ID, bool isBlue);

    void playerGoTo(int ID, bool isBlue, Position pos);
    void playerRotateTo(int ID, bool isBlue, Position pos);

    //PlayerControls
    void getPlayerControl(int ID, bool isBlue);
private:
    //Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    ActuatorService *_actuator;
    CoachService *_coach;
    ControlPacket _playerControl;
};

#endif // PLAYER_H
