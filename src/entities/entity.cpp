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

#include "entity.h"

#include <src/utils/text/text.h>

int Entity::_entityId = 0;

Entity::Entity() {
    _loopFrequency = 60; // default loop frequency is 60
    _isEnabled = true;   // enabling by default
    _loopEnabled = true; // enabling loop by default
    _entityId++;         // sum the entity id for the next entity created
}

void Entity::run(){
    initialization();

    while(isEnabled()) {
        startTimer();
        if(isLoopEnabled()) {
            loop();
        }
        stopTimer();

        long rest = getRemainingTime();
        if(rest >= 0) {
            msleep(rest);
        }
        else {
            std::cout << Text::yellow(QString("[%1] ").arg(this->name()).toStdString(), true) + Text::bold("Thread timer overextended for " + std::to_string(-rest) + " milliseconds.") + '\n';
        }
    }

    finalization();
}

void Entity::setLoopFrequency(int hz) {
    _mutexLoop.lock();
    _loopFrequency = hz;
    _mutexLoop.unlock();
}

void Entity::enableEntity() {
    _mutexEnabled.lock();
    _isEnabled = true;
    _mutexEnabled.unlock();
}

void Entity::disableLoop() {
    _mutexLoop.lock();
    _loopEnabled = false;
    _mutexLoop.unlock();
}

void Entity::stopEntity() {
    _mutexEnabled.lock();
    _isEnabled = false;
    _mutexEnabled.unlock();
}

int Entity::loopFrequency() {
    _mutexLoop.lock();
    int loopFrequency = _loopFrequency;
    _mutexLoop.unlock();

    return loopFrequency;
}

bool Entity::isEnabled() {
    _mutexEnabled.lock();
    bool isEnabled = _isEnabled;
    _mutexEnabled.unlock();

    return isEnabled;
}

bool Entity::isLoopEnabled() {
    _mutexLoop.lock();
    bool loopEnabled = _loopEnabled;
    _mutexLoop.unlock();

    return loopEnabled;
}

int Entity::entityId() {
    return _entityId;
}

void Entity::startTimer() {
    _entityTimer.start();
}

void Entity::stopTimer() {
    _entityTimer.stop();
}

long Entity::getRemainingTime() {
    long remainingTime = (1000 / _loopFrequency) - _entityTimer.getMiliSeconds();

    return remainingTime;
}
