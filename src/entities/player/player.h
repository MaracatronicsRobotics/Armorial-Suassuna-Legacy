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

#include <QReadWriteLock>

#include <src/entities/entity.h>
#include <src/services/actuator/actuatorservice.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/utils/utils.h>

// This has to be on Coach or a BaseCoach header
#include <src/entities/referee/referee.h>

class Player : public Entity
{
public:
    Player(int playerID, WorldMap *worldMap, SSLReferee *referee, Constants *constants);
    ~Player();
    QString name();

    // Player getters
    int getPlayerID();
    bool isDribbling();
    float getPlayerRadius();
    Position getPlayerPos();
    Angle getPlayerOrientation();
    AngularSpeed getPlayerAngularSpeed();
    Velocity getPlayerVelocity();
    Acceleration getPlayerAcceleration();
    RobotStatus getPlayerStatus();

    // Player Aux methods
    float getPlayerAngleTo(Position targetPos);
    float getRotationAngleTo(Position targetPos, Position referencePos);
    float getPlayerDistanceTo(Position targetPos);
    bool hasBallPossession();
    bool isLookingTo(Position targetPos);

    // Role Management
    QString roleName();
    QString behaviorName();
    //void setRole(/* Here comes the Role Class */);

    // Skills
    void playerGoTo(Position pos);
    void playerRotateTo(Position pos, Position referencePos = Position(Utils::getPositionObject(0.0f, 0.0f, 0.0f, true)));
    void playerDribble(bool enable);
    void playerKick(float power, bool isChip);

    //PlayerControls
    void getPlayerControl(int ID, bool isBlue);
private:
    //Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Player Control
    QList<ControlPacket> _playerControls;
    ControlPacket *_playerControl;
    int _playerID;
    bool _isDribbling;
    Position _playerPos;

    // Player Role
    /* Here comes the Role Class */

    // Mutex
    QReadWriteLock _mutex;

    // Actuator Service
    ActuatorService *_actuatorService;
    ActuatorService* getActuatorService();

    // Coach Service
    CoachService *_coachService;
    CoachService* getCoachService();

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorld();

    // Referee
    SSLReferee *_referee;
    SSLReferee* getReferee();

    Position _dest;
    Position _lookTo;
};

#endif // PLAYER_H
