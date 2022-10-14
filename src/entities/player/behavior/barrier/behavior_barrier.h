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

#ifndef BEHAVIOR_BARRIER_H
#define BEHAVIOR_BARRIER_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/skill/skills.h>

class Behavior_Barrier : public Behavior
{
public:
    Behavior_Barrier();
    QString name();

    // Setters
    void setTargetPosition(Geometry::Vector2D targetPosition) { _targetPosition = targetPosition; }
    void setEllipseAxis(float ellipseAxis_X, float ellipseAxis_Y) { _ellipseAxis_X = ellipseAxis_X; _ellipseAxis_Y = ellipseAxis_Y; }
    void enableRotation(bool isRotationEnabled) { _isRotationEnabled = isRotationEnabled; }
    void setRadius(float radius) {_radius = radius;}

private:
    // Behavior inherited methods
    void configure();
    void run();

    // Skills enum
    enum {
        SKILL_GOTO,
        SKILL_ROTATETO,
        SKILL_SPIN
    };

    // Skills pointers
    Skill_GoTo *_skill_goTo;
    Skill_RotateTo *_skill_rotateTo;
    Skill_Spin *_skill_spin;

    // v Barrier v

    // Parameters
    Geometry::Vector2D _targetPosition;
    bool _isRotationEnabled;
    float _ellipseAxis_X;
    float _ellipseAxis_Y;


    //default _radius value
    float _radius;

    // Internal functions
    Geometry::Vector2D projectPosOutsideGoalArea(Geometry::Vector2D pos, bool avoidOurArea, bool avoidTheirArea);
    bool spinOrientarion();
};

#endif // BEHAVIOR_BARRIER_H
