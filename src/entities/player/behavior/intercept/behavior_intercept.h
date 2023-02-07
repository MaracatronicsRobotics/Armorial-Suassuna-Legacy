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

#ifndef BEHAVIOR_INTERCEPT_H
#define BEHAVIOR_INTERCEPT_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/behavior/move/behavior_move.h>

class Behavior_Intercept : public Behavior
{
public:
    Behavior_Intercept();

    //Setters
    void setInterceptSegment(Geometry::LineSegment interceptSegment) { _interceptSegment = interceptSegment; }
    void setObjectPosition(Geometry::Vector2D objectPosition) { _objectPosition = objectPosition; }
    void setObjectVelocity(Geometry::Vector2D objectVelocity) { _objectVelocity = objectVelocity; }
    void setIntersectionAccuracy(float intersectionAccuracy);

private:
    // Behavior inherited methods
    void configure();
    void run();

    // Skills pointers
    Skill_GoTo *_skill_goTo;

    // Skills enum
    enum {
        SKILL_GOTO
    };

    Geometry::LineSegment _interceptSegment;
    Geometry::Vector2D _objectPosition;
    Geometry::Vector2D _objectVelocity;
    Geometry::Vector2D _interceptPosition;
    float _intersectionAccuracy;
};

#endif // BEHAVIOR_INTERCEPT_H
