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

#include "suassuna.h"

Suassuna::Suassuna(Constants *constants) {
    // Setting up constants
    _constants = constants;

    // Creating world
    _world = new World(getConstants());

    // Setting GUI as nullptr
    _gui = nullptr;
}

void Suassuna::start(bool useGui) {
    // Creating World Map
    _worldMap = new WorldMap(getConstants());
    _world->addEntity(_worldMap, 0);

    // Creating gand adding referee to world
    _referee = new SSLReferee(getConstants(), _worldMap);
    _world->addEntity(_referee, 0);

    // Set utils
    Utils::setConstants(getConstants());
    Utils::setWorldMap(_worldMap);

    // Setup GUI
    if(useGui) {
        _gui = new GUI();
        _gui->show();
    }

    // Starting world
    _world->start();

    // Disabling world thread (loop() won't run anymore)
    _world->disableLoop();
}

void Suassuna::stop() {
    // Stopping and waiting world
    _world->stopEntity();
    _world->wait();

    // Deleting world (it also delete all other entities added to it)
    delete _world;

    // If gui pointer is not null, close and delete it
    if(_gui != nullptr) {
        _gui->close();
        delete _gui;
    }
}

Constants* Suassuna::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Suassuna") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
