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

#ifndef ROLE_DEFAULT_H
#define ROLE_DEFAULT_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/behaviors.h>

class Role_Default : public Role
{
public:
    Role_Default();

private:
    // Role inherited methods
    void configure();
    void run();

    // Behaviors enum
    enum {
        BEHAVIOR_DEFAULT
    };

    // Behaviors pointers
    Behavior_Default *_behavior_default;
};

#endif // ROLE_DEFAULT_H
