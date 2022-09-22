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

#include "univectorfield.h"

float UnivectorField::_obstacleRadiusFactor = 1.0f;

UnivectorField::UnivectorField() {

}

UnivectorField::~UnivectorField() {

}

QString UnivectorField::name() {
    return "UnivectorField";
}

void UnivectorField::reset() {
    _obstacles.clear();
}

void UnivectorField::addBall(const Position &pos, const Velocity &vel) {
    addObstacle(pos);
}

void UnivectorField::addGoalArea(const Position &pos) {
    addObstacle(pos, 2.0*OBSTACLE_RADIUS);
}

void UnivectorField::addTeamRobot(const Position &pos, const Velocity &vel) {
    addObstacle(pos);
}

void UnivectorField::addEnemyRobot(const Position &pos, const Velocity &vel) {
    addObstacle(pos);
}

void UnivectorField::setObstacleRadiusFactor(float factor) {
    _obstacleRadiusFactor = factor;
}

void UnivectorField::run() {
    // Algorithm comes here
}

void UnivectorField::addObstacle(const Position &pos, const float &radius) {
    Obstacle obst;
    obst.setPosition(pos);
    obst.setRadius(radius * _obstacleRadiusFactor);

    _obstacles.push_back(obst);
}

Angle UnivectorField::getDirection() const {
    return _resultantAngle;
}

UnivectorField *UnivectorField::clone() const {
    return new UnivectorField(*this);
}
