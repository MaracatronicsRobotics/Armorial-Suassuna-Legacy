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

#include <src/entities/player/player.h>
#include <src/entities/basestation/basestation.h>
#include <src/entities/worldmap/worldmap.h>

Skill::Skill() {
    _worldMap = nullptr;
    _player = nullptr;
    _initialized = false;
    _nav = nullptr;
    _gui = nullptr;
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

void Skill::setGUI(GUI *gui) {
    _gui = gui;
}

void Skill::runSkill() {
    _nav = new Navigation(_player->identifier().robotid(), _worldMap);
    _nav->setGUI(gui());
    run();
}

Player* Skill::player() {
    return _player;
}

Navigation* Skill::nav() {
    return _nav;
}

GUI* Skill::gui() {
    return _gui;
}

void Skill::setWheelsSpeed(const float &wheelLeft, const float &wheelRight) {
    _player->setWheelsSpeed(wheelLeft, wheelRight);
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
