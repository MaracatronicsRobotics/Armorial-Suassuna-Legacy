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

#ifndef ROLE_ATTACKER_H
#define ROLE_ATTACKER_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/behaviors.h>
#include <Armorial/Geometry/Arc/Arc.h>

class Role_Attacker : public Role
{
public:
    Role_Attacker(bool supporter = false);

private:
    void configure();
    void run();

    bool alignedToTheirGoal();
    bool hasPossession(Geometry::Vector2D ballPos);
    Geometry::Vector2D getSecondAttackerPos();

    enum {
        BEHAVIOR_MOVETO,
        BEHAVIOR_CHASER
    };

    Behavior_MoveTo *_behavior_moveTo;
    Behavior_Chaser *_behavior_chaser;

    enum State {
        STATE_CHASE,
        STATE_CHARGE,
        STATE_SUPPORT
    };

    State _currState;
    bool _supporter;
};

#endif // ROLE_ATTACKER_H
