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


#include "behavior.h"

#include <spdlog/spdlog.h>

Behavior::Behavior() {
    _player = nullptr;
    _worldMap = nullptr;
    _actualSkill = nullptr;
    _initialized = false;
}

Behavior::~Behavior() {
    // Deleting skills
    QList<Skill*> skillList = _skillList.values();
    QList<Skill*>::iterator it;

    for(it = skillList.begin(); it != skillList.end(); it++) {
        if((*it) != nullptr) {
            delete (*it);
        }
    }

    // Cleaning map
    _skillList.clear();
}

bool Behavior::isInitialized() {
    return _initialized;
}

void Behavior::initialize(WorldMap *worldMap) {
    _worldMap = worldMap;
    configure();
    _initialized = true;
}

void Behavior::setPlayer(Player *player) {
    _player = player;
}

void Behavior::runBehavior() {
    // Check if behavior have at least one skill
    if(_skillList.size() == 0) {
        spdlog::error("[{}] No skills set!", name().toStdString());
        return ;
    }

    // Run behavior (implemented by child inherited method)
    run();
}

void Behavior::addSkill(int id, Skill *skill) {
    if(_skillList.contains(id)) {
        spdlog::error("[{}] Trying to add a skill with id '{}' that is already associated.", name().toStdString(), id);
        return ;
    }

    // If is the first skill added, set it as actualSkill
    if(_actualSkill == nullptr) {
        _actualSkill = skill;
    }

    _skillList.insert(id, skill);
}

void Behavior::runSkill(int id) {
    if(!_skillList.contains(id)) {
        spdlog::error("[{}] Trying to set a skill with id '{}' that was not been added.", name().toStdString(), id);
        return ;
    }

    _actualSkill = _skillList.value(id);

    // Check if initialized
    if(!_actualSkill->isInitialized()) {
        _actualSkill->initialize();
    }

    // Run skill
    _actualSkill->setPlayer(player());
    _actualSkill->runSkill();
}

Player* Behavior::player() {
    return _player;
}

WorldMap* Behavior::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error("[{}] WorldMap with nullptr value.", name().toStdString());
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
