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

