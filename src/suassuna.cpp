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

#include <src/entities/basestation/sim/simstation.h>
#include <src/entities/coach/team/team.h>
#include <src/entities/player/player.h>

#include <src/entities/player/role/default/role_default.h> // TODO: remove
#include <src/entities/player/role/goalkeeper/role_goalkeeper.h> // TODO: remove
#include <src/entities/player/role/attacker/role_attacker.h>

SuassunaCore::SuassunaCore() {
    // Set GUI as nullptr by default
    _gui = nullptr;

    // Start entity manager
    _entityManager = new Threaded::EntityManager();
}

SuassunaCore::~SuassunaCore() {
    // If GUI is not null, hide and delete
    if(_gui != nullptr) {
        _gui->hide();
        delete _gui;
    }

    // Delete all entities in EntityManager
    delete _entityManager;
}

bool SuassunaCore::start(bool useGUI) {
    _gui = nullptr;

    // Start worldmap
    _worldMap = new WorldMap();

    // Start controller
    _controller = new SimStation();
    _entityManager->addEntity(_controller);

    // Start vision
    _vision = new Vision();
    _entityManager->addEntity(_vision);

    // If useGUI is set, create and show the GUI
    if(useGUI) {
        _gui = new GUI();
        _gui->show();

        QObject::connect(_vision, &Vision::sendRobots, _gui, &GUI::updateRobots);
        QObject::connect(_vision, &Vision::sendBalls, _gui, &GUI::updateBalls);
        QObject::connect(_vision, &Vision::sendField, _gui, &GUI::updateFieldGeometry);
    }

    // Start teams
    magic_enum::enum_for_each<Common::Enums::Color>([this] (auto color) {
        if(color != Common::Enums::Color::UNDEFINED) {
            _teams.insert(color, new Team(color));

            for(int i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
                Player *player = new Player(color, i, ((color == Suassuna::Constants::teamColor()) ? _controller : nullptr), _worldMap);
                _teams[color]->addPlayer(player);

                if(color == Suassuna::Constants::teamColor()) {
                    _entityManager->addEntity(player);
                    player->setGUI(_gui);
                    if (i == 0) {
                        player->setRole(new Role_Goalkeeper());
                    } else if(i == 1) {
                        player->setRole(new Role_Attacker());
                    } else {
                        player->setRole(new Role_Default());
                    }                       
                }
            }
        }
    });

    // Make connections between modules
    QObject::connect(_vision, &Vision::sendRobots, _worldMap, &WorldMap::updatePlayers);
    QObject::connect(_vision, &Vision::sendBalls, _worldMap, &WorldMap::updateBall);
    QObject::connect(_vision, &Vision::sendField, _worldMap, &WorldMap::updateField);

    // Setup teams in WorldMap
    _worldMap->setupTeams(_teams);

    // Start all entities
    _entityManager->startEntities();

    return true;
}

bool SuassunaCore::stop() {
    // Stop entities registered in EntityManager (this also waits for them to stop)
    _entityManager->disableEntities();

    return true;
}
