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

#ifndef SKILL_ROTATETO_H
#define SKILL_ROTATETO_H

#include <src/entities/player/skill/skill.h>

class Skill_RotateTo : public Skill
{
public:
    Skill_RotateTo();
    void setTargetAngle(const float& targetAngle);
    void setSimulationGame(const bool& isSimulation);

private:
    void configure();
    void run();

    // Internal
    float _targetAngle;
    bool _isSimulation;
};

#endif // SKILL_ROTATETO_H
