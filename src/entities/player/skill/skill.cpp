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


#include "skill.h"

#include <spdlog/spdlog.h>

Skill::Skill() {
    _worldMap = nullptr;
    _player = nullptr;
    _initialized = false;
}

Skill::~Skill() {

}

bool Skill::isInitialized() {
    return _initialized;
}


void Skill::initialize(WorldMap* worldMap) {
    _worldMap = worldMap;
    configure();
    _initialized = true;
}

void Skill::setPlayer(Player *player) {
    _player = player;
}

void Skill::runSkill() {
    run();
}

Player* Skill::player() {
    return _player;
}

WorldMap* Skill::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error("[{}] WorldMap with nullptr value.", name().toStdString());
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
