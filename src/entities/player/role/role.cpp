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

#include <spdlog/spdlog.h>

#include <src/entities/player/player.h>
#include <src/entities/player/behavior/behavior.h>

Role::Role() {
    _player = nullptr;
    _worldMap = nullptr;
    _actualBehavior = nullptr;
    _initialized = false;
}

Role::~Role() {
    // Deleting skills
    QList<Behavior*> behaviorList = _behaviorList.values();
    QList<Behavior*>::iterator it;

    for(it = behaviorList.begin(); it != behaviorList.end(); it++) {
        if((*it) != nullptr) {
            delete (*it);
        }
    }

    // Cleaning map
    _behaviorList.clear();
}

bool Role::isInitialized() {
    return _initialized;
}

void Role::initialize(WorldMap *worldMap) {
    _worldMap = worldMap;

    configure();
    _initialized = true;
}

void Role::setPlayer(Player *player) {
    _player = player;
}

void Role::setGUI(GUI *gui) {
    _gui = gui;
}

QString Role::actualBehaviorName() {
    if(_actualBehavior == nullptr) {
        return "NONE";
    }
    else {
        return _actualBehavior->name();
    }
}

void Role::runRole() {
    // Check if role have at least one behavior
    if(_behaviorList.size() == 0) {
        spdlog::error("[{}] No behaviors set.", name().toStdString());
        player()->idle();
        return ;
    }

    // Run role (implemented by child inherited method)
    run();

    // Set player in behavior
    _actualBehavior->setPlayer(player());

    // Check if initialized
    if(!_actualBehavior->isInitialized()) {
        _actualBehavior->initialize(getWorldMap());
    }

    // Run skill
    _actualBehavior->runBehavior();
}

void Role::addBehavior(int id, Behavior *behavior) {
    if(_behaviorList.contains(id)) {
        spdlog::error("[{}] Trying to add a behavior with id '{}' that is already associated.", name().toStdString(), id);
        return ;
    }

    // If is the first skill added, set it as actualSkill
    if(_actualBehavior == nullptr) {
        _actualBehavior = behavior;
    }

    _behaviorList.insert(id, behavior);
}

void Role::setBehavior(int id) {
    if(!_behaviorList.contains(id)) {
        spdlog::error("[{}] Trying to set an behavior that has not been added previously.", name().toStdString());
        return ;
    }

    _actualBehavior = _behaviorList.value(id);
}

Player* Role::player() {
    return _player;
}

WorldMap* Role::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error("[{}] WorldMap with nullptr value.", name().toStdString());
    }
    else {
        return _worldMap;
    }

    return nullptr;
}

GUI* Role::gui() {
    return _gui;
}
