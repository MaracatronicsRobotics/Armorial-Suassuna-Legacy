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

Player::Player(int playerID, WorldMap *worldMap, SSLReferee *referee, Constants *constants) {
    _constants = constants;
    _playerID = playerID;
    _actuatorService = nullptr;
    _coachService = nullptr;
    _playerControl = nullptr;
    _worldMap = worldMap;
    _referee = referee;
    _isDribbling = false;
    _dest.set_x(-4.3f);
    _dest.set_y(0.0f);
    _dest.set_z(0.0f);
    _dest.set_isinvalid(false);
    _lookTo.set_x(0.0f);
    _lookTo.set_y(2.0f);
    _lookTo.set_z(0.0f);
    _lookTo.set_isinvalid(false);
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

float Player::getPlayerAngleTo(Position targetPos) {
    return atan2(targetPos.y() - Player::getPlayerPos().y(), targetPos.x() - Player::getPlayerPos().x());
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

QString Player::roleName() {
    QString roleName = "Still needs the Role Class";
    return roleName;
}

QString Player::behaviorName() {
    QString behaviorName = "Still needs the Behavior Class";
    return behaviorName;
}

void Player::playerGoTo(Position pos) {
    // Here use pid output
    // For now, lets just go straight to pos without limits
    if (_playerControl == nullptr) {
        spdlog::error(Text::bold("Player ControlPacket with nullptr value at Player::playerGoTo"));
        _playerControl->CopyFrom(Utils::controlPacket(_playerID, Player::getConstants()->isTeamBlue()));
        _actuatorService->SetControl(*_playerControl);
        spdlog::warn(Text::bold(QString("Sending zero packet to Robot: %1 of Team: %2")
                                .arg(_playerID)
                                .arg(Player::getConstants()->isTeamBlue() ? "Blue" : "Yellow").toStdString()));
    } else {
        Position playerPos = Player::getPlayerPos();
        float dx = (playerPos.x() - pos.x());
        float dy = (playerPos.y() - pos.y());

        // Getting halfway vectors trying to avoid enormous velocities
        // This should be fixed after implementing PID

        float vx = (dx * cos(getPlayerOrientation().value()) + dy * sin(getPlayerOrientation().value()));
        float vy = (dy * cos(getPlayerOrientation().value()) + dx * sin(getPlayerOrientation().value()));

        _playerControl = _actuatorService->setVelocity(_playerID, Player::getConstants()->isTeamBlue(), -vx/2, -vy/2, 0.0f);
        _playerControls.push_back(*_playerControl);
    }
}

void Player::playerRotateTo(Position pos, Position referencePos) {
    if (_playerControl == nullptr) {
        spdlog::error(Text::bold("Player ControlPacket with nullptr value at Player::playerRotateTo"));
        _playerControl->CopyFrom(Utils::controlPacket(_playerID, Player::getConstants()->isTeamBlue()));
        _actuatorService->SetControl(*_playerControl);
        spdlog::warn(Text::bold(QString("Sending zero packet to Robot: %1 of Team: %2")
                                .arg(_playerID)
                                .arg(Player::getConstants()->isTeamBlue() ? "Blue" : "Yellow").toStdString()));
    } else {
        // If Pos is invalid, use playerPos
        if (referencePos.isinvalid()) {
            referencePos = Player::getPlayerPos();
        }

        float angleRobotToObjective = Player::getRotationAngleTo(pos, referencePos);
        float ori = Player::getPlayerOrientation().value();

        if (ori > M_PI) ori -= 2.0 * M_PI;
        if (ori < -M_PI) ori += 2.0 * M_PI;

        float angleRobotToTarget = ori + angleRobotToObjective;

        float vw = (ori - angleRobotToTarget)/2;

        _playerControl = _actuatorService->setAngularSpeed(_playerID, Player::getConstants()->isTeamBlue(), vw, false);
        _playerControls.push_back(*_playerControl);
    }
}



void Player::initialization() {
    // Create Actuator service pointers
    _actuatorService = new ActuatorService(getConstants());
    _coachService = new CoachService(getConstants());

    // Create Control Packet pointer
    _playerControl = new ControlPacket();

    // Log
    spdlog::info(Text::cyan(QString("[PLAYER %1 : %2] ")
                            .arg(Player::getConstants()->isTeamBlue() ? "BLUE" : "YELLOW")
                            .arg(_playerID).toStdString(), true) + Text::bold("Thread started."));
}

void Player::loop() {
    // Lock mutex for write and send requests to Actuator Service
    _mutex.lockForWrite();


    if (getPlayerPos().x() >= 4.0f) {
        _dest.set_x(-4.0f - 0.2f);
        _dest.set_y(0.0f);
        _dest.set_z(0.0f);
        _dest.set_isinvalid(false);
        _lookTo.set_x(0.0f);
        _lookTo.set_y(2.0f);
        _lookTo.set_z(0.0f);
        _lookTo.set_isinvalid(false);
    } else if (getPlayerPos().x() <= -4.0f) {
        _dest.set_x(4.0f + 0.2f);
        _dest.set_y(0.0f);
        _dest.set_z(0.0f);
        _dest.set_isinvalid(false);
        _lookTo.set_x(0.0f);
        _lookTo.set_y(-2.0f);
        _lookTo.set_z(0.0f);
        _lookTo.set_isinvalid(false);
    }

    if(Utils::distance(getPlayerPos(), _dest) >= 0.0f) {
        playerGoTo(_dest);
        //playerRotateTo(_lookTo);
    }

    // Send ControlPackets
    if (_playerControls.size() > 0) {
        getActuatorService()->SetControls(_playerControls);
    }

    // Test

    spdlog::info(Text::cyan(QString("[PLAYER %1 : %2] ")
                            .arg("YELLOW")
                            .arg(getPlayerID()).toStdString(), true)
                 + Text::bold(QString("Position: (%1,%2,%3).")
                            .arg(getPlayerPos().x())
                            .arg(getPlayerPos().y())
                            .arg(getPlayerPos().z()).toStdString()));

    spdlog::info(Text::cyan("[BALL] ", true)
                 + Text::bold(QString("Position: (%1,%2,%3).")
                            .arg(getWorld()->getBall().ballposition().x())
                            .arg(getWorld()->getBall().ballposition().y())
                            .arg(getWorld()->getBall().ballposition().z()).toStdString()));


    _playerControls.clear();

    // Unlock mutex
    _mutex.unlock();
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

WorldMap* Player::getWorld() {
    if(_worldMap == nullptr) {
        spdlog::error(Text::bold("WorldMap with nullptr value at Player"));
    } else {
        return _worldMap;
    }

    return nullptr;
}
