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

#ifndef BEHAVIOR_CATCH_H
#define BEHAVIOR_CATCH_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/skill/skills.h>

class Behavior_Catch : public Behavior
{
public:
    Behavior_Catch();

private:
    // Behavior inherited methods
    void configure();
    void run();

    // Skills enum
    enum {
        SKILL_GOTO,
        SKILL_ROTATETO
    };

    enum {
        STATE_STOP,
        STATE_CATCH
    };

    // Skills pointers
    Skill_GoTo *_skill_goTo;
    Skill_RotateTo *_skill_rotateTo;
    int _actualState;
};

#endif // BEHAVIOR_CATCH_H