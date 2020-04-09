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

#include "navalgorithm.h"
#include <utils/mrctimer/mrctimer.h>

#define NAVALG_PATHRESOLUTION 0.10
#define NAVALG_MAXTIME 5 // ms

NavAlgorithm::NavAlgorithm() {
    _generatePath = true;
}

NavAlgorithm::NavAlgorithm(const NavAlgorithm &copy) {
    _originPos = copy._originPos;
    _originOri = copy._originOri;
    _originVel = copy._originVel;
    _goalPos = copy._goalPos;
    _goalOri = copy._goalOri;
    _loc = copy._loc;

    // Does not generate path for clones
    _generatePath = false;
    _path.clear();
}

NavAlgorithm::~NavAlgorithm() {

}

void NavAlgorithm::initialize(Locations *loc) {
    _loc = loc;
}

void NavAlgorithm::runNavAlgorithm() {
    // Run NavAlgorithm specific implementation
    run();

    // Generate path
/*
    if(_generatePath)
        this->generatePath();
*/
}

void NavAlgorithm::setOrigin(const Position &pos, const Angle &ori, const Velocity &vel) {
    _originPos = pos;
    _originOri = ori;
    _originVel = vel;
}

void NavAlgorithm::setGoal(const Position &pos, const Angle &ori) {
    _goalPos = pos;
    _goalOri = ori;
}

float NavAlgorithm::getDistance() const {
    QMutexLocker locker(&_pathMutex);
    return _distance;
}

QList<Position> NavAlgorithm::getPath() const {
    QMutexLocker locker(&_pathMutex);

    // Return path
    return _path;
}

NavAlgorithm *NavAlgorithm::clone() const {
    return NULL;
}

void NavAlgorithm::generatePath() {
    QMutexLocker locker(&_pathMutex);

    // Clear old path
    _path.clear();
    _distance = 0.0;

    // Generate a copy of the object
    NavAlgorithm *clone = this->clone();
    if(clone==NULL) {
        // Child doesn't implement clone
        _path.append(_originPos);
        _path.append(_goalPos);
        _distance = WR::Utils::distance(_originPos, _goalPos);
        return;
    }

    // Appends the current origin
    _path.append(_originPos);

    // Generate points until reach the goal
    MRCTimer timer(5);
    Position currPos = _originPos;
    while(WR::Utils::distance(currPos, _goalPos)>NAVALG_PATHRESOLUTION && timer.checkTimerEnd()) {

    }

    // Delete clone
    delete clone;

    // Add goal to the path
    _distance += WR::Utils::distance(_path.last(), _goalPos);
    _path.append(_goalPos);
}

