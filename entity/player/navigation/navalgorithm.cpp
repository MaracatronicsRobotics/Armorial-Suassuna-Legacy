/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
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
#include <utils/wrtimer/wrtimer.hh>

#define NAVALG_MAXTIME 5 // ms

NavigationAlgorithm::NavigationAlgorithm() {
    _generatePath = true;
}

NavigationAlgorithm::NavigationAlgorithm(const NavigationAlgorithm &copy) {
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

NavigationAlgorithm::~NavigationAlgorithm() {

}

QString NavigationAlgorithm::name() {
    return "Not defined";
}

void NavigationAlgorithm::initialize(Locations *loc) {
    _loc = loc;
}

void NavigationAlgorithm::runNavigationAlgorithm() {
    // Run NavigationAlgorithm specific implementation
    run();

    // Generate path
    if(_generatePath)
        this->generatePath();
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
    QMutexLocker locker(&_pathMutex);
    return _distance;
}

QLinkedList<Position> NavigationAlgorithm::getPath() const {
    QMutexLocker locker(&_pathMutex);

    // Return path
    return _path;
}

NavigationAlgorithm *NavigationAlgorithm::clone() const {
    return NULL;
}

void NavigationAlgorithm::generatePath() {
    QMutexLocker locker(&_pathMutex);

    // Clear old path
    _path.clear();
    _distance = 0.0;

    // Generate a copy of the object
    NavigationAlgorithm *clone = this->clone();
    if(clone==NULL) {
        // Child doesn't implement clone
        _path.append(_originPos);
        _path.append(_goalPos);
        _distance = WR::Utils::distance(_originPos, _goalPos);
        return;
    }

    // Apends the current origin
    _path.append(_originPos);

    // Generate points until reach the goal
    WRTimer timer;
    Position currPos = _originPos;
    float navAlgPathResolution = 0.03f;
    while(WR::Utils::distance(currPos, _goalPos)>4*navAlgPathResolution && timer.timemsec()<=NAVALG_MAXTIME) {

        // Run algorithm
        clone->runNavigationAlgorithm();

        // Gets the direction for the goal
        Angle direction = clone->getDirection();

        // Calcuates the new dx and dy whose will be added to the actual position
        float dx = navAlgPathResolution*cos(direction.value());
        float dy = navAlgPathResolution*sin(direction.value());
        currPos.setPosition(currPos.x() + dx, currPos.y() + dy, 0.0);

        // Update distance
        _distance += WR::Utils::distance(_path.last(), currPos);

        // Add to the path
        _path.append(currPos);

        // Set new origins
        clone->setOrigin(currPos, Angle(), Velocity());

        timer.stop();
    }

    // Delete clone
    delete clone;

    // Add goal to the path
    _distance += WR::Utils::distance(_path.last(), _goalPos);
    _path.append(_goalPos);
}
