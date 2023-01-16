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

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>

#include <src/entities/basestation/basestation.h>
#include <src/entities/player/role/role.h>

#define MAX_TIME_TO_MARK_AS_IDLE 1.0 // 1 second

Player::Player(const Common::Enums::Color& teamColor, const quint8& robotId, BaseStation* controller, WorldMap *worldMap) : Suassuna::Robot(teamColor, robotId)
{
    // Setup modules
    _controller = controller;
    _worldMap = worldMap;

    // Setup idle control
    _firstIt = false;
    _idleTimer.start();

    // Setup initial role as nullptr
    _playerRole = nullptr;

    // Setup control packet
    Armorial::RobotIdentifier* robotIdentifier = new Armorial::RobotIdentifier();
    robotIdentifier->CopyFrom(identifier());
    _controlPacket.set_allocated_robotidentifier(robotIdentifier);
}

QString Player::roleName() {
    _mutexRole.lock();
    QString roleName = (_playerRole == nullptr) ? "No Role" : _playerRole->name();
    _mutexRole.unlock();

    return roleName;
}

QString Player::behaviorName() {
    _mutexRole.lock();
    QString behaviorName = (_playerRole == nullptr) ? "No Behavior" : _playerRole->actualBehaviorName();
    _mutexRole.unlock();

    return behaviorName;
}

void Player::setRole(Role *role) {
    _mutexRole.lock();
    _playerRole = role;
    _mutexRole.unlock();
}

void Player::updatePlayer(Common::Types::Object playerData) {
    setPosition(playerData.getPosition());
    setVelocity(playerData.getVelocity());
    setAcceleration(playerData.getAcceleration());
    setOrientation(playerData.getOrientation());
    setAngularSpeed(playerData.getAngularSpeed());

    _idleTimer.start();
}

void Player::idle() {
    _controlPacket.set_w1(0.0);
    _controlPacket.set_w2(0.0);
}

void Player::setWheelsSpeed(const float &wheelLeft, const float &wheelRight) {
    _controlPacket.set_w1(wheelLeft);
    _controlPacket.set_w2(wheelRight);
}

BaseStation* Player::controller() {
    return _controller;
}

void Player::initialization() {
    spdlog::info("[{}] Thread started.", fmt::format(fmt::fg(teamColor() == Common::Enums::Color::BLUE ? fmt::terminal_color::blue : fmt::terminal_color::yellow) | fmt::emphasis::bold, entityName().toStdString() + " " + std::to_string(robotId())));
}

void Player::loop() {
    // Check if player is idle (not received updated data from him for a long time)
    if(_idleTimer.getSeconds() >= MAX_TIME_TO_MARK_AS_IDLE || _firstIt) {
        _firstIt = false;
        idle();
    }
    else {
        _mutexRole.lock();
        if(_playerRole != nullptr) {
            if(!_playerRole->isInitialized()) {
                _playerRole->initialize(_worldMap);
            }
            _playerRole->setPlayer(this);
            _playerRole->runRole();
        }
        else {
            idle();
        }
        _mutexRole.unlock();
    }

    controller()->sendData(_controlPacket);
}

void Player::finalization() {
    spdlog::info("[{}] Thread ended.", fmt::format(fmt::fg(teamColor() == Common::Enums::Color::BLUE ? fmt::terminal_color::blue : fmt::terminal_color::yellow) | fmt::emphasis::bold, entityName().toStdString() + " " + std::to_string(robotId())));
}
