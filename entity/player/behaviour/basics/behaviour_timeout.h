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

#ifndef BEHAVIOUR_TIMEOUT_H
#define BEHAVIOUR_TIMEOUT_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_TimeOut : public Behaviour {
private:
    void configure();
    void run();
    Position _desiredPosition;
    Skill_GoToLookTo *_skill_GoToLookTo;
    double _offsetBall;

public:
    Behaviour_TimeOut();
    QString name();

    void setSkillPosition(Position pos);
};

#endif // BEHAVIOUR_TIMEOUT_H
