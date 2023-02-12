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

#ifndef ROLE_GOALKEEPER_H
#define ROLE_GOALKEEPER_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/move/behavior_move.h>
#include <src/entities/player/behavior/intercept/behavior_intercept.h>

class Role_Goalkeeper : public Role
{
public:
    Role_Goalkeeper();

private:
    // Role inherited methods
    void configure();
    void run();

    // Behaviors enum
    enum {
        BEHAVIOR_MOVE,
        BEHAVIOR_INTERCEPT
    };

    // Behavior pointers
    Behavior_Move *_behavior_move;
    Behavior_Intercept *_behavior_intercept;

    // State Machine
    uint _state;
    enum {
        ROTATE,
        INTERCEPT
    };

    // Parameters
    bool _gkOverlap;

    // Auxiliary methods
    float getIntersectionAccuracy(Geometry::LineSegment interceptionSegment);
};

#endif // ROLE_GOALKEEPER_H
