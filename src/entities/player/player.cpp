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

Player::Player(ActuatorService *actuator, CoachService *coach)
{
    _actuator = actuator;
    _coach = coach;
}

Position Player::getPlayerPos(int ID, bool isBlue) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    Position robotPos = robot.robotposition();
    return robotPos;
}

Angle Player::getPlayerOrientation(int ID, bool isBlue) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    Angle robotOrientation = robot.robotorientation();
    return robotOrientation;
}

AngularSpeed Player::getPlayerAngularSpeed(int ID, bool isBlue){
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    AngularSpeed robotAngularSpeed = robot.robotangularspeed();
    return robotAngularSpeed;
}

Velocity Player::getPlayerVelocity(int ID, bool isBlue) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    Velocity robotSpeed = robot.robotvelocity();
    return robotSpeed;
}

Acceleration Player::getPlayerAcceleration(int ID, bool isBlue) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    Acceleration robotAcceleration = robot.robotacceleration();
    return robotAcceleration;
}

RobotStatus Player::getPlayerStatus(int ID, bool isBlue) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    RobotStatus robotStatus = robot.robotstatus();
    return robotStatus;
}

void Player::playerGoTo(int ID, bool isBlue, Position pos) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    // Configure _playerControl here
    _actuator->SetControl(_playerControl);
}

void Player::playerRotateTo(int ID, bool isBlue, Position pos) {
    RobotIdentifier robotId = Utils::getRobotIdObject(ID, isBlue);
    Robot robot = _coach->getRobot(robotId);
    // Configure _playerControl here
    _actuator->SetControl(_playerControl);
}
