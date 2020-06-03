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

#ifndef BEHAVIOUR_INTERCEPT_H
#define BEHAVIOUR_INTERCEPT_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>
#include <utils/basics/circle.h>

class Behaviour_Intercept : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    Skill_GoToLookTo *_skill_goToLookTo;

    // utilitary functions
    std::pair<Circle, Circle> generateCircles(Point2d& ballPosition, Point2d& kickVector);
    Circle generateRobotCircle(Circle& circle1, Circle& circle2, Point2d& robotPosition);
    Circle robotCircleAux(Point2d& circleCenter, Point2d& robotPosition);
    double pointDistance(Point2d& a, Point2d& b);
    bool isCounterClockwise(Point2d& circleCenterVector, Point2d& testVector);
    bool isClockwise(Point2d& circleCenterVector, Point2d& testVector);

    // setters
    void setDistanceToGoal(double d) { _distanceToGoal = d; }
    void setCircleRadius(double r) { _circleRadius = r; }
public:
    Behaviour_Intercept();
    QString name();

    // parameters
    double _distanceToGoal;
    double _circleRadius;
};

#endif // BEHAVIOUR_INTERCEPT_H
