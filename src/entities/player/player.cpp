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

#define MAX_TIME_TO_MARK_AS_IDLE 1.0 // 1 second

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>

#include <math.h>
#include <algorithm>

#include <src/entities/player/role/role.h>

Player::Player(const quint8 playerId, const Common::Enums::Color& teamColor, WorldMap *worldMap, Controller *controller) {
    _playerId = playerId;
    _teamColor = teamColor;
    _worldMap = worldMap;
    _controller = controller;

    // Start PIDs
//    _vxPID = new PID(2.0, 0.1, 0.1);
//    _vxPID->setOutputLimits(2.5);
//    _vxPID->setOutputRampRate(0.2);

//    _vyPID = new PID(2.0, 0.1, 0.1);
//    _vyPID->setOutputLimits(2.5);
//    _vyPID->setOutputRampRate(0.2);

    _playerRole = nullptr;

    _vwPID = new AnglePID(5.0, 0.1, 0.2, 20.0, 1.0/60.0);
}

Common::Enums::Color Player::teamColor() {
    return _teamColor;
}

quint8 Player::playerId() {
    return _playerId;
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

void Player::goTo(const Geometry::Vector2D &target) {
    float linearSpeed = 0.5f; // 0.5m/s
    float L = 0.075;
    float r = 0.0325/2.0;

    float angleReferenceToTarget = (target - getPosition()).angle();
    float vwOut = _vwPID->getOutput(angleReferenceToTarget, this->getOrientation().value());

    float wl = ((2.0*linearSpeed) - (L*vwOut)) / (2.0 * r);
    float wr = ((2.0*linearSpeed) + (L*vwOut)) / (2.0 * r);

    if(isnanf(wl) || isnanf(wr)) {
        return ;
    }

    bool isNegL = wl < 0.0;
    bool isNegR = wr < 0.0;
    wl = fabs(wl);
    wr = fabs(wr);

    // Descomentar para rodar robo fisico
//    {
//        wl = int((std::min(std::max(wl, 30.0f), 100.0f) / 100.0f) * 255);
//        wr = int((std::min(std::max(wr, 30.0f), 100.0f) / 100.0f) * 255);
//        _controller->setWheelsSpeed(playerId(), wl * (isNegL ? (1) : -1), wr * (isNegR ? (1) : -1));
//    }

    // Descomentar para rodar robo simulado
    {
        _controller->setWheelsSpeed(playerId(), wl * (isNegL ? (-1) : 1), wr * (isNegR ? (-1) : 1));
    }

    spdlog::info("{} {} {}", playerId(), wl, wr);
}

void Player::rotateTo(const Geometry::Angle &targetAngle) {
    if(isnanf(targetAngle.value())) {
        return ;
    }

    float L = 0.075;
    float r = 0.0325/2.0;

    float vwOut = _vwPID->getOutput(targetAngle.value(), this->getOrientation().value());
    float wl = -((L*vwOut) / (2.0 * r));
    float wr = ((L*vwOut) / (2.0 * r));

    if(isnanf(wl) || isnanf(wr)) {
        return ;
    }

    bool isNegL = wl < 0.0;
    bool isNegR = wr < 0.0;
    wl = fabs(wl);
    wr = fabs(wr);

    // Descomentar para rodar robo fisico
//    {
//        wl = int((std::min(std::max(wl, 30.0f), 100.0f) / 100.0f) * 255);
//        wr = int((std::min(std::max(wr, 30.0f), 100.0f) / 100.0f) * 255);
//        _controller->setWheelsSpeed(playerId(), wl * (isNegL ? (1) : -1), wr * (isNegR ? (1) : -1));
//    }

    // Descomentar para rodar robo simulado
    {
        _controller->setWheelsSpeed(playerId(), wl * (isNegL ? (-1) : 1), wr * (isNegR ? (-1) : 1));
    }

    spdlog::info("{} {} {}", playerId(), wl, wr);
}

void Player::spin(bool isClockWise) {
    if (isClockWise) {
        _controller->setWheelsSpeed(playerId(), 255, -255);
    } else {
        _controller->setWheelsSpeed(playerId(), -255, 255);
    }
}

void Player::kick(const float &kickSpeed, const float &chipKickAngle, const float &kickAngle) {
    _controller->setKick(playerId(), kickSpeed, chipKickAngle, kickAngle);
}

void Player::dribble(const bool &dribbling) {
    _controller->setDribble(playerId(), dribbling);
}

void Player::idle() {
    _controller->setWheelsSpeed(playerId(), 0.0f, 0.0f);
}

void Player::initialization() {
    spdlog::info("[{}] Thread started.", fmt::format(fmt::fg(teamColor() == Common::Enums::Color::BLUE ? fmt::terminal_color::blue : fmt::terminal_color::yellow) | fmt::emphasis::bold, entityName().toStdString() + " " + std::to_string(playerId())));
}

void Player::loop() {
    // Check if player is idle (not received updated data from him for a long time)
    if(_idleTimer.getSeconds() >= MAX_TIME_TO_MARK_AS_IDLE) {
        setRole(nullptr);
        idle();
    }
    else {
        /// TODO: cast role
        //_controller->setWheelsSpeed(playerId(), 255, -255);
        _mutexRole.lock();
        if(_playerRole != nullptr) {
            if(!_playerRole->isInitialized()) {
                _playerRole->initialize(_worldMap);
            }
            _playerRole->setPlayer(this);
            _playerRole->runRole();
        }
        _mutexRole.unlock();
    }
}

void Player::finalization() {
    spdlog::info("[{}] Thread ended.", fmt::format(fmt::fg(teamColor() == Common::Enums::Color::BLUE ? fmt::terminal_color::blue : fmt::terminal_color::yellow) | fmt::emphasis::bold, entityName().toStdString() + " " + std::to_string(playerId())));
}
