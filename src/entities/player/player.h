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

#include <src/entities/baseCoach.h>

#define IDLE_COUNT 60

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
    Position getPlayerKickerDevicePos();
    Angle getPlayerOrientation();
    AngularSpeed getPlayerAngularSpeed();
    Velocity getPlayerVelocity();
    Acceleration getPlayerAcceleration();
    RobotStatus getPlayerStatus();
    bool isPlayerInAvaliableRobots();

    // Player Aux methods
    float getPlayerAngleTo(Position targetPos);
    float getPlayerOrientationTo(Position targetPos, Position referencePos = Position(Utils::getPositionObject(0.0f, 0.0f, 0.0f, true)));
    float getRotationAngleTo(Position targetPos, Position referencePos);
    float getPlayerDistanceTo(Position targetPos);
    bool hasBallPossession();
    bool isLookingTo(Position targetPos);
    bool isSufficientlyAlignedTo(Position targetPos, Position referencePos = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true));

    // Player Error
    float getLinearError();
    float getAngularError();

    // Role Management
    QString roleName();
    QString behaviorName();
    void setRole(Role *role);

    // Skills
    void playerGoTo(Position pos);
    void playerRotateTo(Position pos, Position referencePos = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true));
    void playerDribble(bool enable);
    void playerKick(float power, bool isChip);
    void playerIdle();

    //PlayerControls
    void getPlayerControl(int ID, bool isBlue);
private:
    //Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Player Control
    //QList<ControlPacket> _playerControls;
    ControlPacket _playerControl;
    int _playerID;
    int _idleCount;
    bool _isDribbling;
    Position _playerPos;

    // Player Role
    Role *_playerRole;

    // Mutex
    QReadWriteLock _mutexRole;

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
    WorldMap* getWorldMap();

    // Referee
    SSLReferee *_referee;
    SSLReferee* getReferee();

    //Only for testing purposes
    Position _dest;
    Position _lookTo;
};

#endif // PLAYER_H
