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

#ifndef ROLE_DEFENSIVE_MIDFIELDER_H
#define ROLE_DEFENSIVE_MIDFIELDER_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Defensive_Midfielder : public Role
{
private:
    // Behaviours
    Behaviour_Barrier *_bh_bar;
    Behaviour_MarkBall *_bh_mkBall;

    // Behaviours Enum
    enum{
        BEHAVIOUR_BARRIER,
        BEHAVIOUR_MARKBALL
    };

    // Inherited functions
    void configure();
    void run();

    // Parameters
    quint8 _markId;
    QMutex _markMutex;
    float _minDist;

    // Auxiliar functions
    quint8 getOurPlayerClosestToBall();
    quint8 getTheirPlayerClosestToBall();

public:
    Role_Defensive_Midfielder();
    void initializeBehaviours();
    QString name();

    void setMarkId(quint8 markId);
    quint8 getMarkId();
    // minDist => minimum distance of an allie to the ball to be barrier again
    void setMinDist(float minDist) { _minDist = minDist; }
};

#endif // ROLE_DEFENSIVE_MIDFIELDER_H
