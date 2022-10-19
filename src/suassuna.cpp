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

#include <src/entities/player/role/default/role_default.h>
#include <src/entities/player/role/goalkeeper/role_goalkeeper.h>
#include <src/entities/player/role/attacker/role_attacker.h>
#include <src/entities/player/role/goalkeeper/role_gk.h>

Suassuna::Suassuna() {
    // Set GUI as nullptr by default
    _gui = nullptr;
    _useSimEnv = false;

    // Start entity manager
    _entityManager = new Threaded::EntityManager();
}

Suassuna::~Suassuna() {
    // If GUI is not null, hide and delete
    if(_gui != nullptr) {
        _gui->hide();
        delete _gui;
    }

    // Delete all entities in EntityManager
    delete _entityManager;
}

bool Suassuna::start(bool useGUI, bool useSimEnv, bool useSwapRoles) {
    // If useGUI is set, create and show the GUI
    if(useGUI) {
        _gui = new GUI();
        _gui->show();
    }

    _useSimEnv = useSimEnv;
    _swapRoles = useSwapRoles;

    // Start worldmap
    _worldMap = new WorldMap(Constants::visionServiceAddress(), Constants::visionServicePort());
    _entityManager->addEntity(_worldMap);

    // Start referee
    _referee = new VSSReferee(_worldMap);
    _entityManager->addEntity(_referee);

    // Start controller
    _controller = new Controller(Constants::actuatorServiceAddress(), Constants::actuatorServicePort());
    _entityManager->addEntity(_controller);

    // Start teams
    if (!_swapRoles) {
        magic_enum::enum_for_each<Common::Enums::Color>([this] (auto color) {
            if(color != Common::Enums::Color::UNDEFINED) {
                _teams.insert(color, new SSLTeam(color));

                for (auto i : Constants::goalkeeperIds()) {
                    if (i != 255) {
                        Player *player = new Player(i, color, _referee, _worldMap, ((color == Constants::teamColor()) ? _controller : nullptr), _useSimEnv);
                        _teams[color]->addPlayer(player);

                        if(color == Constants::teamColor()) {
                            _entityManager->addEntity(player);
                            player->setRole(new Role_Goalkeeper());
                        }
                    }
                }

                for (auto i : Constants::attackerIds()) {
                    if (i != 255) {
                        Player *player = new Player(i, color, _referee, _worldMap, ((color == Constants::teamColor()) ? _controller : nullptr), _useSimEnv);
                        _teams[color]->addPlayer(player);

                        if(color == Constants::teamColor()) {
                            _entityManager->addEntity(player);
                            player->setRole(new Role_Attacker());
                        }
                    }
                }

    //            for (auto i : Constants::supporterIds()) {
    //                if (i != 255) {
    //                    Player *player = new Player(i, color, _worldMap, ((color == Constants::teamColor()) ? _controller : nullptr), _useSimEnv);
    //                    _teams[color]->addPlayer(player);

    //                    if(color == Constants::teamColor()) {
    //                        _entityManager->addEntity(player);
    //                        player->setRole(new Role_Supporter());
    //                    }
    //                }
    //            }
            }
        });
    }
    if (_swapRoles) {
        magic_enum::enum_for_each<Common::Enums::Color>([this] (auto color) {
            if (color != Common::Enums::Color::UNDEFINED) {
                _teams.insert(color, new SSLTeam(color)) {
                    QList<quint8> idList = {0, 1, 2};
                    // Basic configuration
                    for (auto i : idList) {
                        Player *player = new Player(i, color, _referee, _worldMap, ((color == Constants::teamColor()) ? _controller : nullptr), _useSimEnv);
                        _teams[color]->addPlayer(player);
                        if (color == Constants::teamColor()) {
                            _entityManager->addEntity(player);

                            // Roles to be setted after
                        }
                    }

                    // GK to be setted first
                    _teams[color]->_players.at(0)->setRole(new Role_Goalkeeper());

                    // Attacker then
                    _teams[color]->_players.at(0)->setRole(new Role_Attacker());

                    // Supporter then
//                    _teams[color]->_players.at(0)->setRole(new Role_Supporter());
                }
            }
        });
    }

    // Setup teams in WorldMap
    _worldMap->setupTeams(_teams);

    // Start all entities
    _entityManager->startEntities();

    return true;
}

bool Suassuna::stop() {
    // Stop entities registered in EntityManager (this also waits for them to stop)
    _entityManager->disableEntities();

    return true;
}
