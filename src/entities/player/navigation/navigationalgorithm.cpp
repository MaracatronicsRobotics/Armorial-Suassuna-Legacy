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

#include "navigationalgorithm.h"

#include <src/utils/timer/timer.h>

NavigationAlgorithm::NavigationAlgorithm() {
    _generatePath = true;
}

NavigationAlgorithm::NavigationAlgorithm(const NavigationAlgorithm &copy) {
    _originPos = copy._originPos;
    _originOri = copy._originOri;
    _originVel = copy._originVel;
    _goalPos = copy._goalPos;
    _goalOri = copy._goalOri;
    _worldMap = copy._worldMap;
    _constants = copy._constants;

    _generatePath = false;
    _path.clear();
}

NavigationAlgorithm::~NavigationAlgorithm() {

}

void NavigationAlgorithm::initialize(WorldMap *worldMap, GUI *gui, Constants *constants) {
    _worldMap = worldMap;
    _gui = gui;
    _constants = constants;
}

void NavigationAlgorithm::runNavigationAlgorithm() {
    run();

    if (_generatePath) {
        generatePath();
    }
}

void NavigationAlgorithm::setOrigin(const Position &pos, const Angle &ori, const Velocity &vel) {
    _originPos = pos;
    _originOri = ori;
    _originVel = vel;
}

void NavigationAlgorithm::setGoal(const Position &pos, const Angle &ori) {
    _goalPos = pos;
    _goalOri = ori;
}

float NavigationAlgorithm::getDistance() const {
    QMutexLocker locker(&_mutexPath);
    return _distance;
}

QLinkedList<Position> NavigationAlgorithm::getPath() const {
    QMutexLocker locker(&_mutexPath);
    return _path;
}

void NavigationAlgorithm::generatePath() {
    QMutexLocker locker(&_mutexPath);

    _path.clear();
    _distance = 0.0f;

    NavigationAlgorithm *clone = this->clone();
    if (clone == nullptr) {
        _path.append(_originPos);
        _path.append(_goalPos);
        _distance = Utils::distance(_originPos, _goalPos);
        return;
    }

    _path.append(_originPos);

    Timer timer;
    Position currPos = _originPos;
    float navAlgPathStep = 0.03f; // Check usage

    timer.start();
    while (Utils::distance(currPos, _goalPos) > 4*navAlgPathStep && timer.getMiliSeconds() <= NAVALG_MAXTIME) {
        clone->runNavigationAlgorithm();

        Angle direction = clone->getDirection();

        float dx = navAlgPathStep * cos(direction.value());
        float dy = navAlgPathStep * sin(direction.value());
        currPos.set_x(currPos.x() + dx);
        currPos.set_y(currPos.y() + dy);

        _distance += Utils::distance(_path.last(), currPos);

        _path.append(currPos);

        // Still without player's speed
        clone->setOrigin(currPosition, direction, _originVel);

        timer.stop();
    }

    _distance += Utils::distance(_path.last(), _goalPos);
    _path.append(_goalPos);
}

NavigationAlgorithm* NavigationAlgorithm::clone() const {
    return nullptr;
}

WorldMap* NavigationAlgorithm::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error(Text::bold("WorldMap with nullptr value at Navigation Algorithm"));
    } else {
        return _worldMap;
    }

    return nullptr;
}

GUI* NavigationAlgorithm::getGUI() {
    if(_gui == nullptr) {
        spdlog::error(Text::bold("GUI with nullptr value at Navigation Algorithm"));
    } else {
        return _gui;
    }

    return nullptr;
}

Constants* NavigationAlgorithm::getConstants() {
    if(_constants == nullptr) {
        spdlog::error(Text::bold("Constants with nullptr value at Navigation Algorithm"));
    } else {
        return _constants;
    }

    return nullptr;
}
