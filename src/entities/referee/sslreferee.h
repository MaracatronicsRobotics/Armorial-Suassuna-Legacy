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

#ifndef VSSREFEREE_H
#define VSSREFEREE_H

#include <Armorial/Base/Client/Client.h>
#include <Armorial/Threaded/Entity/Entity.h>
#include <Armorial/Geometry/Vector2D/Vector2D.h>

#include <src/entities/worldmap/worldmap.h>

/*!
 * \brief The VSSReferee class is responsible to receive the upcoming data from Referee
 * and make it available for the team.
 */
class VSSReferee : public Threaded::Entity, public Base::UDP::Client
{
public:
    /*!
     * \brief VSSReferee constructor.
     */
    VSSReferee(WorldMap *worldMap);

    /*!
     * \brief VSSReferee destructor.
     */
    ~VSSReferee();

    bool stopRobots() { return _stopRobots; }

private:
    void initialization();
    void loop();
    void finalization();

    // Internal
    WorldMap *_worldMap;
    bool _stopRobots;
};

#endif // SSLREFEREE_H
