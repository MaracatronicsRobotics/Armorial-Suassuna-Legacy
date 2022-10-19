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
#include <src/entities/player/skill/skills.h>
#include "math.h"

class Behavior_Intercept : public Behavior
{
public:
    Behavior_Intercept();
    QString name();

    // Setters
    void setInterceptSegment(Geometry::Vector2D firstPoint, Geometry::Vector2D secondPoint);
    void setBaseSpeed(float baseSpeed) { _baseSpeed = baseSpeed; }
    void setObjectPosition(Geometry::Vector2D objectPos) { _objectPos = objectPos; }
    void setObjectVelocity(Geometry::Vector2D objectVel) { _objectVel = objectVel; }
    void enableSpin(bool spinEnabled) { _spinEnabled = spinEnabled; }
    void setLinearError(float desiredLinearError) { _desiredLinearError = desiredLinearError; }

    // Getters
    Geometry::Vector2D getInterceptPosition() { return _interceptPos; }
    Geometry::Vector2D getInterceptVelocity() { return _interceptVel; }

private:
    // Behavior inherited methods
    void configure();
    void run();

    // Skills enum
    enum {
        SKILL_GOTO,
        SKILL_SPIN
    };

    // Skills pointers
    Skill_GoTo *_skill_goTo;
    Skill_Spin *_skill_spin;

    // Parameters
    Geometry::Vector2D _firstLimitationPoint;
    Geometry::Vector2D _secondLimitationPoint;
    Geometry::Vector2D _objectPos;
    Geometry::Vector2D _objectVel;
    Geometry::Vector2D _interceptPos;
    Geometry::Vector2D _interceptVel;
    float _baseSpeed;
    bool _spinEnabled;
    float _desiredLinearError;

    // Auxiliary Functions
    Geometry::Vector2D getOrthogonalProjection();
    Geometry::Vector2D getInterceptionPosition();
};

#endif // BEHAVIOR_INTERCEPT_H
