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

#include "role.h"

Role::Role() {
    _player = nullptr;
    _constants = nullptr;
    _worldMap = nullptr;
    _initialized = false;
    _configureEnabled = true;
    _actualBehaviour = -1;
}

Role::~Role() {
    //Here to clean behaviourList
}

bool Role::isInitialized() {
    return _initialized;
}

void Role::initialize(Constants *constants, WorldMap *worldMap, Referee *referee) {
    _constants = constants;
    _worldMap = worldMap;
    _referee = referee;
    if (_worldMap != nullptr) {
        _loc = _worldMap->getLocations();
    }

    _configureEnabled = true;
    initializedBehaviours();
    configure();
    _configureEnabled = false;

    _initialized = true;
}

void Role::setPlayer(Player *player) {
    _player = player;
}

void Role::runRole() {
    if (/*_behaviourList.size() == 0 && */ getConstants() == nullptr) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Role " + name() + " has no behaviours set!\n").toStdString()));
        return;
    }

    run();
}

Constants* Role::getConstants() {
    if (_constants == nullptr) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Constants with nullptr value at Role " + name() + "!\n").toStdString()));
    } else {
        return _constants;
    }
    return nullptr;
}

WorldMap* Role::getWorldMap() {
    if (_worldMap == nullptr) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("WorldMap with nullptr value at Role " + name() + "!\n").toStdString()));
    } else {
        return _worldMap;
    }
    return nullptr;
}

Referee* Role::getReferee() {
    if (_referee == nullptr) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Referee with nullptr value at Role " + name() + "!\n").toStdString()));
    } else {
        return _referee;
    }
    return nullptr;
}

Locations* Role::getLocations() {
    if (_loc == nullptr) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Locations with nullptr value at Role " + name() + "!\n").toStdString()));
    } else {
        return _loc;
    }
    return nullptr;
}
