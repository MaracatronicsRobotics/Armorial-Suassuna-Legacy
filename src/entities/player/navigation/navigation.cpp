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

#include "navigation.h"

#include <src/entities/player/player.h>

Navigation::Navigation(Player *player, NavigationAlgorithm *navAlgorithm, WorldMap *worldMap, GUI *gui, Constants *constants) {
    _player = player;
    _navAlgorithm = navAlgorithm;
    _worldMap = worldMap;
    _gui = gui;
    _constants = constants;

    if(getNavAlg() != nullptr) {
        _navAlgorithm->initialize(getWorldMap(), getGUI(), getConstants());
    }
}

Navigation::~Navigation() {
    if (getNavAlg() != nullptr) {
        delete _navAlgorithm;
    }
}

void Navigation::setGoal(const Position &destination, const Angle &orientation, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea) {
    QMutexLocker locker(&_mutexNavAlg);

    getNavAlg()->reset();

    getNavAlg()->setOrigin(getPlayer()->getPlayerPos(), getPlayer()->getPlayerOrientation(), getPlayer()->getPlayerVelocity());
    getNavAlg()->setGoal(destination, orientation);

    if(avoidBall) {
        Ball ball = getWorldMap()->getBall();
        getNavAlg()->addBall(ball.ballposition(), ball.ballvelocity());
    }

    if(avoidTeammates) {
        QList<Robot> players = getWorldMap()->getRobots(getConstants()->teamColor());

        for (Robot r : players) {
            if (r.robotidentifier().robotid() == getPlayer()->getPlayerID()) continue;

            getNavAlg()->addTeamRobot(r.robotposition(), r.robotvelocity());
        }
    }

    if(avoidOpponents) {
        QList<Robot> players = getWorldMap()->getRobots(getConstants()->opponentColor());

        for (Robot r : players) {
            getNavAlg()->addEnemyRobot(r.robotposition(), r.robotvelocity());
        }
    }

    if(avoidOurGoalArea || avoidTheirGoalArea) {
        if(Utils::isOutsideField(_player->getPlayerPos())) return;
        if(Utils::isInsideOurArea(_player->getPlayerPos())) return;
        if(Utils::isInsideTheirArea(_player->getPlayerPos())) return;

        if(avoidOurGoalArea) {
            Position far = Utils::projectPointAtSegment(getWorldMap()->getLocations()->ourGoalLeftPost(), getWorldMap()->getLocations()->ourGoalRightPost(), _player->getPlayerPos());
            Position repulsion = Utils::threePoints(far, _player->getPlayerPos(), getWorldMap()->getLocations()->fieldDefenseRadius(), 0.0f);

            _navAlgorithm->addGoalArea(repulsion);
        }

        if(avoidTheirGoalArea) {
            float fieldWidth = getWorldMap()->getLocations()->fieldWidth();
            float goaldWidth = (getWorldMap()->getLocations()->theirSide().isLeft() ? -1 : 1) * getWorldMap()->getLocations()->fieldDefenseWidth();
            float goalHeight = getWorldMap()->getLocations()->fieldDefenseLength();
            Position theirGoal = getWorldMap()->getLocations()->theirGoal();
            Position theirMidGoal = Utils::getPositionObject(theirGoal.x() - goaldWidth/2, theirGoal.y());

            Position cInsideGoal = Utils::getPositionObject(theirMidGoal.x(), -goalHeight/2.0);
            Position dInsideGoal = Utils::getPositionObject(theirMidGoal.x(), goalHeight/2.0);

            _navAlgorithm->addGoalArea(cInsideGoal);
            _navAlgorithm->addGoalArea(dInsideGoal);

            for (float i = -fieldWidth/2.0; i <= fieldWidth/2.0; i += fieldWidth/10.0) {
                Position circle = Utils::getPositionObject(theirGoal.x() + (getWorldMap()->getLocations()->theirSide().isLeft() ? -1.0 : 1.0), i);
                _navAlgorithm->addGoalArea(circle);
            }
        }
    }
}

void Navigation::runNavAlgorithm() {
    if(getNavAlg() != nullptr) {
        getNavAlg()->runNavigationAlgorithm();
    }
}

Angle Navigation::getDirection() const {
    QMutexLocker locker(&_mutexNavAlg);

    return _navAlgorithm->getDirection();
}

float Navigation::getDistance() const {
    QMutexLocker locker(&_mutexNavAlg);

    return _navAlgorithm->getDistance();
}

QLinkedList<Position> Navigation::getPath() const {
    QMutexLocker locker(&_mutexNavAlg);

    return _navAlgorithm->getPath();
}

QString Navigation::getNavAlgName() const {
    return _navAlgorithm->name();
}

Player* Navigation::getPlayer() {
    if(_player == nullptr) {
        spdlog::error(Text::bold("Player with nullptr value at Navigation"));
    } else {
        return _player;
    }

    return nullptr;
}

WorldMap* Navigation::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error(Text::bold("WorldMap with nullptr value at Navigation"));
    } else {
        return _worldMap;
    }

    return nullptr;
}

GUI* Navigation::getGUI() {
    if(_gui == nullptr) {
        spdlog::error(Text::bold("GUI with nullptr value at Navigation"));
    } else {
        return _gui;
    }

    return nullptr;
}

NavigationAlgorithm* Navigation::getNavAlg() {
    if(_navAlgorithm == nullptr) {
        spdlog::error(Text::bold("Navigation Algorithm with nullptr value at Navigation"));
    } else {
        return _navAlgorithm;
    }

    return nullptr;
}

Constants* Navigation::getConstants() {
    if(_constants == nullptr) {
        spdlog::error(Text::bold("Constants with nullptr value at Navigation"));
    } else {
        return _constants;
    }

    return nullptr;
}
