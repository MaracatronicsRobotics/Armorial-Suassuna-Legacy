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

#ifndef SKILL_GOTO_H
#define SKILL_GOTO_H

#include <src/entities/player/skill/skill.h>

class Skill_GoTo : public Skill
{
public:
    Skill_GoTo();

    // Target management
    void setTargetPosition(const Geometry::Vector2D& targetPosition);

private:
    // Skill inherited methods
    void configure();
    void run();

    // Internal
    Geometry::Vector2D _targetPosition;
    float _speedFactor;
};

#endif // SKILL_GOTO_H
