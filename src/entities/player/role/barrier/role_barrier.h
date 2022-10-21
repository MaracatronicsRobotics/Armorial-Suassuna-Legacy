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

#ifndef ROLE_BARRIER_H
#define ROLE_BARRIER_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/behaviors.h>

class Role_Barrier : public Role
{
public:
    Role_Barrier();

private:
    void configure();
    void run();

    enum {
        BEHAVIOR_CHASER
    };

    Behavior_Chaser *_behavior_chaser;

    enum State {
        STATE_TOWER,
        STATE_BLOCK,
        STATE_DEFEND
    };

    State _currState;
    float _fixedTowerX;

    // Functions
    bool isInTowerArea(Geometry::Vector2D target);
};

#endif // ROLE_BARRIER_H
