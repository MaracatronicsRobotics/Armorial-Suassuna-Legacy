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
    _behaviour = nullptr;
    _goToLookTo = nullptr;
    _actualBehaviour = nullptr;
    _initialized = false;
    _configureEnabled = true;
    _isHalted = false;
}

Role::~Role() {
    QHash<int, Behaviour*>:: iterator it;
    for (it = _behaviourList.begin(); it != _behaviourList.end(); it++) {
        if ( (*it) == nullptr) {
            continue;
        } else {
            delete *it;
        }
    }
    _behaviourList.clear();

    if (_goToLookTo != nullptr) {
        delete _goToLookTo;
    }
}

bool Role::isInitialized() {
    return _initialized;
}

void Role::initialize(Constants *constants, WorldMap *worldMap, SSLReferee *referee) {
    _constants = constants;
    _worldMap = worldMap;
    _referee = referee;
    if (_worldMap != nullptr) {
        _loc = _worldMap->getLocations();
    }

    _configureEnabled = true;
    //_goToLookTo = new Behaviour_GoToLookTo();
    initializeBehaviours();

    if(getReferee()->getGameInfo()->timeOut()) {
        _isHalted = true;
    }

    configure();
    _configureEnabled = false;

    _initialized = true;
}

void Role::setPlayer(Player *player) {
    _player = player;
}

void Role::runRole() {
    if (_behaviourList.size() == 0) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Role " + name() + " has no behaviours set!\n").toStdString()));
        player()->playerIdle();
        return;
    }

    if (!getReferee()->getGameInfo()->canMove()) {
        player()->playerIdle();
        return;
    }

    if (getReferee()->getGameInfo()->timeOut()) {
        if (_isHalted) {
            player()->playerIdle();
            return;
        }

        Locations *loc = getWorldMap()->getLocations();
        Color teamColor;
        teamColor.set_isblue(getConstants()->isTeamBlue());
        QList<int> availablePlayers = getWorldMap()->getRobotsIDs(teamColor);
        // Maybe sort here (but not sure)

        Position desiredPosition = Position();
        desiredPosition.set_isinvalid(false);
        desiredPosition.set_x(loc->ourGoal().x() + (loc->ourSide().isLeft() ? loc->fieldDefenseWidth() /*+ getConstants().robotRadius()*/ : -loc->fieldDefenseWidth() /*- getConstants().robotRadius()*/));
        desiredPosition.set_y(loc->fieldDefenseWidth() - (availablePlayers.indexOf(player()->getPlayerID()) * (2*loc->fieldDefenseWidth() / std::max(1, (availablePlayers.size() - 1)))));
        desiredPosition.set_z(0.0f);

        Position lookTo = Position();
        lookTo.set_isinvalid(false);
        lookTo.set_x(-(loc->ourGoal().x() + (loc->ourSide().isLeft() ? loc->fieldDefenseWidth() /*+ getConstants()->robotRadius()*/ : -loc->fieldDefenseWidth() /*- getConstants()->robotRadius()*/)));
        lookTo.set_y(loc->fieldDefenseWidth() - (availablePlayers.indexOf(player()->getPlayerID()) * (2*loc->fieldDefenseWidth() / std::max(1, (availablePlayers.size() - 1)))));
        lookTo.set_z(0.0f);

        if (player()->getPlayerDistanceTo(desiredPosition) <= player()->getLinearError()) {
            _isHalted = true;
        } else {
            _goToLookTo->setPlayer(player());
            // Here comes GoToLookTo setters
            _goToLookTo->runBehaviour();
        }
        return;
    } else {
        _isHalted = false;
    }

    run();

    if (!_actualBehaviour->isInitialized()) {
        _actualBehaviour->initialize(getConstants(), getWorldMap());
    }

    _actualBehaviour->setPlayer(player());
    _actualBehaviour->runBehaviour();
}

Behaviour* Role::getActualBehaviour() {
    return _actualBehaviour;
}

void Role::setBehaviour(int behaviourID) {
    if (!_behaviourList.contains(behaviourID)) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Behaviour ID not found at Role " + name() + "'s behaviour list!\n").toStdString()));
        return;
    }

    _actualBehaviour = _behaviourList.value(behaviourID);
}

Player* Role::player() {
    if (_player == nullptr) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Player with nullptr value at Role " + name() + "!\n").toStdString()));
    } else {
        return _player;
    }

    return nullptr;
}

void Role::addBehaviour(int behaviourID, Behaviour *behaviour) {
    if (_behaviourList.contains(behaviourID)) {
        spdlog::error(Text::red("[ERROR] ", true) + Text::bold(QString("Behaviour ID already found at Role " + name() + "'s behaviour list!\n").toStdString()));
        return ;
    }

    _behaviourList.insert(behaviourID, behaviour);
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

SSLReferee* Role::getReferee() {
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
