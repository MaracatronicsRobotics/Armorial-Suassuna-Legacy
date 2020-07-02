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
    Skill_GoToLookTo *_skill_GoToLookTo;
    Skill_DoNothing  *_skill_doNothing;

    // Control halt
    bool _alreadyReachedPosition;
    Position _desiredPosition;

    enum{
        ENABLE_HALT,
        ENABLE_POS
    };

public:
    Behaviour_TimeOut();
    QString name();

    // Control halt
    void reset();
    void getPlayerTimeOutId();
};

#endif // BEHAVIOUR_TIMEOUT_H
