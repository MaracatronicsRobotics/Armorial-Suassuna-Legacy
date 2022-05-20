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

#include "behaviour.h"

Behaviour::Behaviour() {
    _player = nullptr;
    _worldMap = nullptr;
    _constants = nullptr;
    _actualSkill = nullptr;
    _initialized = false;
}

Behaviour::~Behaviour() {
    // Deleting skills
    QList<Skill*> skillList = _skillList.values();
    QList<Skill*>::iterator it;

    for(it = skillList.begin(); it != skillList.end(); it++) {
        delete *it;
    }

    // Cleaning map
    _skillList.clear();
}

bool Behaviour::isInitialized() {
    return _initialized;
}

void Behaviour::initialize(Constants *constants, WorldMap *worldMap) {
    _constants = constants;
    _worldMap = worldMap;
    configure();
    _initialized = true;
}

void Behaviour::setPlayer(Player *player) {
    _player = player;
}

void Behaviour::runBehaviour() {
    // Check if Behaviour have at least one skill
    if(_skillList.size() == 0) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold(this->name().toStdString() + ":Has no skills set!.") + '\n';
        return ;
    }

    // Run Behaviour (implemented by child inherited method)
    run();
}

void Behaviour::addSkill(int id, Skill *skill) {
    if(_skillList.contains(id)) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold(this->name().toStdString() + ":Trying to add a skill with an id already associated.") + '\n';
        return ;
    }

    // If is the first skill added, set it as actualSkill
    if(_actualSkill == nullptr) {
        _actualSkill = skill;
    }

    _skillList.insert(id, skill);
}

void Behaviour::setSkill(int id) {
    if(!_skillList.contains(id)) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold(this->name().toStdString() + ":Trying to set an skill that has not added previously.") + '\n';
        return ;
    }

    _actualSkill = _skillList.value(id);

    // Check if initialized
    if(!_actualSkill->isInitialized()) {
        _actualSkill->initialize(getConstants());
    }

    // Run skill
    _actualSkill->setPlayer(player());
    _actualSkill->runSkill();
}

Player* Behaviour::player() {
    return _player;
}

Constants* Behaviour::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at " + this->name().toStdString()) + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

WorldMap* Behaviour::getWorldMap() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("WorldMap with nullptr value at " + this->name().toStdString()) + '\n';
    }
    else {
        return _worldMap;
    }

    return nullptr;
}

Locations* Behaviour::loc() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Locations with nullptr value at " + this->name().toStdString()) + '\n';
    }
    else {
        return getWorldMap()->getLocations();
    }

    return nullptr;
}
