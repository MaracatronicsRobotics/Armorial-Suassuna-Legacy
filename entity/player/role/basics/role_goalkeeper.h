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

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Goalkeeper : public Role
{
private:
    // Behaviours
    Behaviour_Goalkeeper *_bh_gk;
    Behaviour_Penalty_GK *_bh_penaltyGk;

    // Behaviours Enum
    enum{
        BEHAVIOUR_GK,
        BEHAVIOUR_PENALTYGK
    };

    // Inherited functions
    void configure();
    void run();

public:
    Role_Goalkeeper();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_GOALKEEPER_H
