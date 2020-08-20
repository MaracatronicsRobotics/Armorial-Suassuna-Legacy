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

#ifndef BEHAVIOUR_TEST_H
#define BEHAVIOUR_TEST_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Test : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    Skill_RotateTo *_skill_rotateTo;
    Skill_GoTo *_skill_goTo;
    Skill_GoToLookTo *_skill_goToLookTo;
    Skill_Kick *_skill_kick;

public:
    Behaviour_Test();
    QString name();
};

#endif // BEHAVIOUR_TEST_H
