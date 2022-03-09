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


#include "worldmap.h"

#include <src/utils/text/text.h>

WorldMap::WorldMap(Constants *constants) {
    _constants = constants;
    _coachService = nullptr;
    _locations = nullptr;
}

QString WorldMap::name() {
    return "WorldMap";
}

Robot WorldMap::getRobot(RobotIdentifier identifier) {
    // Lock mutex for read and get the list for the identifier color
    _mutex.lockForRead();
    QList<Robot> listToSearch = _robots.value(identifier.robotcolor().isblue());
    _mutex.unlock();

    // Try to search in the list an robot that matches the required id
    // and return if found
    for(int i = 0; i < listToSearch.size(); i++) {
        Robot robot = listToSearch.at(i);
        if(robot.robotidentifier().robotid() == identifier.robotid()) {
            return robot;
        }
    }

    // If not found the required robot, return an robot with invalid id
    Robot robot = Robot();

    // Create invalid identifier
    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    identifier.set_robotid(ROBOT_INVALID_ID);
    robot.set_allocated_robotidentifier(robotIdentifier);

    // Return the invalid robot
    return robot;
}

QList<Robot> WorldMap::getRobots(Color color) {
    // Lock mutex for read and get the list for the identifier color
    _mutex.lockForRead();
    QList<Robot> listToSearch = _robots.value(color.isblue());
    _mutex.unlock();

    // Return the list
    return listToSearch;
}

Field WorldMap::getField() {
    // Lock mutex for read and take the field var
    _mutex.lockForRead();
    Field field = _field;
    _mutex.unlock();

    return field;
}

Ball WorldMap::getBall() {
    // Lock mutex for read and take the ball var
    _mutex.lockForRead();
    Ball ball = _ball;
    _mutex.unlock();

    return ball;
}

void WorldMap::initialization() {
    // Create Coach Service pointer
    _coachService = new CoachService(getConstants());

    // Create Locations pointer
    _locations = new Locations(getConstants());

    // Debug
    std::cout << Text::blue("[WORLDMAP] ", true) + Text::bold("Thread started.\n");
}

void WorldMap::loop() {
    // Send requests to coach service and lock QReadWriteLock for write
    _mutex.lockForWrite();

    // Request ball object
    _ball = getService()->getBall();

    // Request field object
    _field = getService()->getField();

    // Fill data in locations
    _locations->updateFieldData(_field);

    // Request robots from team yellow
    Color yellow;
    yellow.set_isblue(false);
    QList<Robot> yellowRobots = getService()->getRobots(yellow);

    // Request robots from team blue
    Color blue;
    blue.set_isblue(true);
    QList<Robot> blueRobots = getService()->getRobots(blue);

    // Update robots in map
    _robots.clear();
    _robots.insert(YELLOW, yellowRobots);
    _robots.insert(BLUE, blueRobots);

    // Unlock mutex
    _mutex.unlock();
}

void WorldMap::finalization() {
    // Delete Coach Service pointer
    delete _coachService;

    // Delete Locations pointer
    delete _locations;

    // Debug
    std::cout << Text::blue("[WORLDMAP] ", true) + Text::bold("Thread ended.\n");
}

Locations* WorldMap::getLocations() {
    if(_locations == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Locations with nullptr value at WorldMap") + '\n';
    }
    else {
        return _locations;
    }

    return nullptr;
}

CoachService* WorldMap::getService() {
    if(_coachService == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("CoachService with nullptr value at WorldMap") + '\n';
    }
    else {
        return _coachService;
    }

    return nullptr;
}

Constants* WorldMap::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at WorldMap") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
