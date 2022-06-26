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

#include "player.h"

#include <src/entities/player/role/role.h>

Player::Player(int playerID, WorldMap *worldMap, SSLReferee *referee, Constants *constants) {
    _constants = constants;
    _playerID = playerID;
    _actuatorService = nullptr;
    _coachService = nullptr;
    _worldMap = worldMap;
    _referee = referee;
    _isDribbling = false;

    _playerRole = nullptr;

    // Creating void cp
    _playerControl = Utils::controlPacket(playerID, getConstants()->isTeamBlue());
}

Player::~Player() {

}

QString Player::name() {
    return "Player";
}

int Player::getPlayerID() {
    return _playerID;
}

bool Player::isDribbling() {
    return _isDribbling;
}

float Player::getPlayerRadius() {
    return 0.09f;
}

Position Player::getPlayerPos() {
    RobotIdentifier robotId = Utils::getRobotIdObject(_playerID, Player::getConstants()->isTeamBlue());
    Robot robot = _worldMap->getRobot(robotId);
    Position robotPos = robot.robotposition();
    return robotPos;
}

Position Player::getPlayerKickerDevicePos(){
    float robotR = getConstants()->getRobotRadius()/2.0;

    const Angle ori = getPlayerOrientation();
    float dx = robotR * cos(ori.value());
    float dy = robotR * sin(ori.value());

    const Position pos = getPlayerPos();
    return Utils::getPositionObject(pos.x()+dx, pos.y()+dy);
}

Angle Player::getPlayerOrientation() {
    RobotIdentifier robotId = Utils::getRobotIdObject(_playerID, Player::getConstants()->isTeamBlue());
    Robot robot = _worldMap->getRobot(robotId);
    Angle robotOrientation = robot.robotorientation();
    return robotOrientation;
}

AngularSpeed Player::getPlayerAngularSpeed(){
    RobotIdentifier robotId = Utils::getRobotIdObject(_playerID, Player::getConstants()->isTeamBlue());
    Robot robot = _worldMap->getRobot(robotId);
    AngularSpeed robotAngularSpeed = robot.robotangularspeed();
    return robotAngularSpeed;
}

Velocity Player::getPlayerVelocity() {
    RobotIdentifier robotId = Utils::getRobotIdObject(_playerID, Player::getConstants()->isTeamBlue());
    Robot robot = _worldMap->getRobot(robotId);
    Velocity robotSpeed = robot.robotvelocity();
    return robotSpeed;
}

Acceleration Player::getPlayerAcceleration() {
    RobotIdentifier robotId = Utils::getRobotIdObject(_playerID, Player::getConstants()->isTeamBlue());
    Robot robot = _worldMap->getRobot(robotId);
    Acceleration robotAcceleration = robot.robotacceleration();
    return robotAcceleration;
}

RobotStatus Player::getPlayerStatus() {
    RobotIdentifier robotId = Utils::getRobotIdObject(_playerID, Player::getConstants()->isTeamBlue());
    Robot robot = _worldMap->getRobot(robotId);
    RobotStatus robotStatus = robot.robotstatus();
    return robotStatus;
}

bool Player::isPlayerInAvaliableRobots() {
    Color teamColor;
    teamColor.set_isblue(getConstants()->isTeamBlue());
    Robot robot;
    robot = Utils::getRobotObject(getPlayerID(), getConstants()->isTeamBlue());
    bool contain = false;
    for (Robot r : getWorldMap()->getRobots(teamColor)) {
        if (robot.robotidentifier().robotid() == r.robotidentifier().robotid()) {
            if (robot.robotidentifier().robotcolor().isblue() == r.robotidentifier().robotcolor().isblue()) {
                contain = true;
            }
        }
    }
    return contain;
}

float Player::getPlayerAngleTo(Position targetPos) {
    return atan2(targetPos.y() - Player::getPlayerPos().y(), targetPos.x() - Player::getPlayerPos().x());
}

float Player::getPlayerOrientationTo(Position targetPosition, Position referencePosition) {
    // If no reference position is defined, use the player position
    if(referencePosition.isinvalid()) {
        referencePosition = getPlayerPos();
    }

    float angleRobotToObjective = getRotationAngleTo(targetPosition, referencePosition);
    float ori = getPlayerOrientation().value();

    if(ori > M_PI) ori -= 2 * M_PI;
    if(ori < -M_PI) ori += 2 * M_PI;

    float angleRobotToTarget = ori + angleRobotToObjective;

    return angleRobotToTarget;
}

float Player::getRotationAngleTo(Position targetPos, Position referencePos) {
    float componentX = (targetPos.x() - referencePos.x());
    float componentY = (targetPos.y() - referencePos.y());
    float distToTarget = sqrt(pow(componentX, 2) + pow(componentY, 2));

    componentX = componentX / distToTarget;

    // Check divisions for 0
    if (isnanf(componentX)) {
        return 0.0f;
    }

    float angleOriginToTarget; // Angle from field origin to targetPos
    float angleRobotToTarget; // Angle from robot to targetPos

    if (componentY < 0.0f) {
        angleOriginToTarget = 2*M_PI - acos(componentX);
    } else {
        angleOriginToTarget = acos(componentX);
    }

    angleRobotToTarget = angleOriginToTarget - Player::getPlayerOrientation().value();

    // Adjust to rotate the minimum possible
    if (angleRobotToTarget > M_PI) angleRobotToTarget -= 2.0 * M_PI;
    if (angleRobotToTarget < -M_PI) angleRobotToTarget += 2.0 * M_PI;

    return angleRobotToTarget;
}

float Player::getPlayerDistanceTo(Position targetPos) {
    return sqrt(pow(Player::getPlayerPos().x() - targetPos.x(), 2) + pow(Player::getPlayerPos().y() - targetPos.y(), 2));
}

bool Player::hasBallPossession() {
    // Need sensor
    if ((Utils::distance(Player::getPlayerPos(), Player::getWorldMap()->getBall().ballposition()) >= 0.25f) && Player::isLookingTo(Player::getWorldMap()->getBall().ballposition())) {
        return true;
    }
    return false;
}

bool Player::isLookingTo(Position targetPos) {
    Angle angle;
    angle.set_value(Utils::getAngle(Player::getPlayerPos(), targetPos));
    angle.set_isinvalid(false);
    angle.set_isindegrees(false);

    float diff = fabs(Utils::angleDiff(Player::getPlayerOrientation(), angle));
    return (diff <= 0.1f); // Here goes Angular Error, should be set later
}

bool Player::isSufficientlyAlignedTo(Position targetPos, Position referencePos) {
    if (referencePos.isinvalid()) {
        referencePos = getPlayerPos();
    }
    AngularSpeed playerAS = getPlayerAngularSpeed();
    return (fabs(getRotationAngleTo(targetPos, referencePos)) <= getAngularError()
            && ((!playerAS.isindegrees()) ? playerAS.vw() <= 10.0f : playerAS.vw() <= Utils::degToRad(10.0f)));
}

float Player::getLinearError() {
    return 0.05f; // 5cm
}

float Player::getAngularError() {
    return 0.02f; // ≃ 1.15 deg
}

QString Player::roleName() {
    _mutexRole.lockForRead();
    QString roleName = (_playerRole == nullptr) ? "No Role" : _playerRole->name();
    _mutexRole.unlock();
    return roleName;
}

QString Player::behaviorName() {
    QString behaviorName = "Still needs the Behavior Class";
    return behaviorName;
}

void Player::setRole(Role *role) {
    _mutexRole.lockForWrite();
    _playerRole = role;
    _mutexRole.unlock();
}

void Player::playerGoTo(Position pos, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea, float speedFactor) {

    Position playerPos = Player::getPlayerPos();

    // Limit on field in not in ball placement
    if(!getReferee()->getGameInfo()->ballPlacement()) {
        if (Utils::isOutsideField(pos)) {
            pos = Utils::limitPositionInField(pos);
        }
    }

    float dx = _pidX->getOutput(playerPos.x(), pos.x());
    float dy = _pidY->getOutput(playerPos.y(), pos.y());

    float vx = (dx * cos(getPlayerOrientation().value()) + dy * sin(getPlayerOrientation().value()));
    float vy = (dy * cos(getPlayerOrientation().value()) - dx * sin(getPlayerOrientation().value()));

    Velocity *robotVel = new Velocity();
    robotVel->CopyFrom(Utils::getVelocityObject(vx, vy, 0.0f, false));

    float maxRobotLinearSpeed = getConstants()->maxRobotLinearSpeed();
    if(!getReferee()->getGameInfo()->allowedNearBall() || getReferee()->getGameInfo()->ourBallPlacement()) {
        maxRobotLinearSpeed = getConstants()->maxSpeedInStop();
    }

    float absVel = Utils::getVelocityAbs(*robotVel);
    if (speedFactor != 1.0f) {
        float absByScalar = absVel * speedFactor;

        // Take unitary vector of velocity
        robotVel->CopyFrom(Utils::getVelocityObject(robotVel->vx()/absVel, robotVel->vy()/absVel, 0.0f, false));

        // Multiply it by absByScalar
        robotVel->CopyFrom(Utils::getVelocityObject(robotVel->vx() * absByScalar, robotVel->vy() * absByScalar, 0.0f, false));
    }

    // Limit robot velocity to maxSpeed
    if (Utils::getVelocityAbs(*robotVel) >= maxRobotLinearSpeed) {
        // Take unitary vector of velocity
        robotVel->CopyFrom(Utils::getVelocityObject(robotVel->vx()/absVel, robotVel->vy()/absVel, 0.0f, false));

        // Multiply it by maxSpeed
        robotVel->CopyFrom(Utils::getVelocityObject(robotVel->vx() * maxRobotLinearSpeed, robotVel->vy() * maxRobotLinearSpeed, 0.0f, false));
    }

    if (isnanf(robotVel->vx()) || isnanf(robotVel->vy())) {
        robotVel->CopyFrom(Utils::getVelocityObject(0.0f, 0.0f, 0.0f, false));
    }

    // Here should come path planning to avoid obstacles

    _playerControl.set_allocated_robotvelocity(robotVel);
    //_playerControl = Utils::controlPacket(_playerID, getConstants()->isTeamBlue(), vx/2, vy/2);
}

void Player::playerRotateTo(Position pos, Position referencePos) {
    if (referencePos.isinvalid()) {
        referencePos = Player::getPlayerPos();
    }

    float angleRobotToObjective = Player::getRotationAngleTo(pos, referencePos);
    float ori = Player::getPlayerOrientation().value();
    // For 'ori', we should need check if player orientation is in degrees or rad

    if (ori > M_PI) ori -= 2.0 * M_PI;
    if (ori < -M_PI) ori += 2.0 * M_PI;

    float angleRobotToTarget = ori + angleRobotToObjective;

    float vw = ori - angleRobotToTarget;

    AngularSpeed *robotVW = new AngularSpeed();
    robotVW->CopyFrom(Utils::getAngularSpeedObject(-(2*vw), false, false));

    _playerControl.set_allocated_robotangularspeed(robotVW);
    //_playerControl = Utils::controlPacket(_playerID, getConstants()->isTeamBlue(), 0.0f, 0.0f, 0.0f, -(2*vw), false);
}

void Player::playerDribble(bool enable) {
    _playerControl.set_dribbling(enable);
}

void Player::playerKick(float power, bool isChip) {
    float robotOri = Player::getPlayerOrientation().value();

    if (robotOri > M_PI) robotOri -= 2.0 * M_PI;
    if (robotOri < -M_PI) robotOri += 2.0 * M_PI;

    float kickX = cos(robotOri) * power;
    float kickY = sin(robotOri) * power;

    Velocity *kickVel = new Velocity();
    kickVel->CopyFrom(Utils::getVelocityObject(kickX, kickY, 0.0f, false));

    if (isChip) {
        // Set chip kick
        kickVel->set_vz(power/2);
    }

    _playerControl.set_allocated_kickspeed(kickVel);
}

void Player::playerIdle() {
    _playerControl = Utils::controlPacket(getPlayerID(), getConstants()->isTeamBlue());
}

void Player::initialization() {
    // Create Actuator service pointers
    _actuatorService = new ActuatorService(getConstants());
    _coachService = new CoachService(getConstants());

    // Create Control Packet pointer
    _playerControl = Utils::controlPacket(_playerID, getConstants()->isTeamBlue());

    // Create PID pointers
    _pidX = new PID(getConstants()->playerLinearPID().at(0), getConstants()->playerLinearPID().at(1), getConstants()->playerLinearPID().at(2));
    _pidY = new PID(getConstants()->playerLinearPID().at(0), getConstants()->playerLinearPID().at(1), getConstants()->playerLinearPID().at(2));
    _pidW = new PID(getConstants()->playerAngularPID().at(0), getConstants()->playerAngularPID().at(1), getConstants()->playerAngularPID().at(2));
//    _pidX = new PID(1.0f, 0.0f, 0.0f);
//    _pidY = new PID(1.0f, 0.0f, 0.0f);
//    _pidW = new PID(1.0f, 0.0f, 0.0f);

    // Log
    spdlog::info(Text::cyan(QString("[PLAYER %1 : %2] ")
                            .arg(Player::getConstants()->isTeamBlue() ? "BLUE" : "YELLOW")
                            .arg(_playerID).toStdString(), true) + Text::bold("Thread started."));
}

void Player::loop() {
    // Lock mutex for write and send requests to Actuator Service
    if (getPlayerPos().isinvalid()) {
        if (_idleCount < IDLE_COUNT) {
            setRole(nullptr);
            playerIdle();

            _idleCount++;
        }
    } else {
        _idleCount = 0;

        this->playerKick(0.0f, false);

        _mutexRole.lockForWrite();
        if (_playerRole != nullptr) {
            if (!_playerRole->isInitialized()) {
                _playerRole->initialize(getConstants(), getWorldMap(), getReferee());
            }
            _playerRole->setPlayer(this);
            _playerRole->runRole();
        }

        if (!isPlayerInAvaliableRobots()) {
            getActuatorService()->SetControl(Utils::controlPacket(_playerID, getConstants()->isTeamBlue()));
            spdlog::error(Text::red(QString("[Player %1 : %2] ")
                                    .arg(getPlayerID())
                                    .arg(getConstants()->getTeamColor()).toStdString(), true)
                         + Text::bold("not found in WorldMap."));
        } else {
            getActuatorService()->SetControl(_playerControl);
        }
        _mutexRole.unlock();
    }
}

void Player::finalization() {
    spdlog::info(Text::cyan(QString("[PLAYER %1 : %2] ")
                            .arg(Player::getConstants()->isTeamBlue() ? "BLUE" : "YELLOW")
                            .arg(_playerID).toStdString(), true) + Text::bold("Thread ended."));
}

ActuatorService* Player::getActuatorService() {
    if(_actuatorService == nullptr) {
        spdlog::error(Text::bold("ActuatorService with nullptr value at Player"));
    } else {
        return _actuatorService;
    }
    return nullptr;
}

CoachService* Player::getCoachService() {
    if(_coachService == nullptr) {
        spdlog::error(Text::bold("CoachService with nullptr value at Player"));
    } else {
        return _coachService;
    }
    return nullptr;
}

Constants* Player::getConstants() {
    if(_constants == nullptr) {
        spdlog::error(Text::bold("Constants with nullptr value at Player"));
    } else {
        return _constants;
    }

    return nullptr;
}

WorldMap* Player::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error(Text::bold("WorldMap with nullptr value at Player"));
    } else {
        return _worldMap;
    }

    return nullptr;
}

SSLReferee* Player::getReferee() {
    if(_referee == nullptr) {
        spdlog::error(Text::bold("SSLReferee with nullptr value at Player"));
    } else {
        return _referee;
    }

    return nullptr;
}
