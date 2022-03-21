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

Skill::Skill() {
    _player = nullptr;
    _constants = nullptr;
    _initialized = false;
}

Skill::~Skill() {

}

bool Skill::isInitialized() {
    return _initialized;
}

void Skill::initialize(Constants *constants) {
    // Init constants
    _constants = constants;

    // Call configure method
    configure();

    // Confirm initialize skill
    _initialized = true;
}

void Skill::setPlayer(Player *player) {
    _player = player;
}

void Skill::runSkill() {
    // Run skill (implemented by child inherited method)
    run();
}

Player* Skill::player() {
    return _player;
}

Constants* Skill::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at " + this->name().toStdString()) + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
