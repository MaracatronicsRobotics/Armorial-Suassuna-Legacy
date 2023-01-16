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

#ifndef SUASSUNA_H
#define SUASSUNA_H

#include <Armorial/Threaded/EntityManager/EntityManager.h>

#include <src/common/constants/constants.h>

#include <src/gui/gui.h>
#include <src/entities/vision/vision.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/entities/basestation/basestation.h>

/*!
 * \brief The Suassuna class is responsible to manage the creation, deletion, start and stop of all
 * the core of the Suassuna application, it is, the entities (such as referee and players) and some
 * other modules.
 */
class SuassunaCore
{
public:
    /*!
     * \brief Suassuna constructor.
     */
    SuassunaCore();

    /*!
     * \brief Suassuna destructor.
     */
    ~SuassunaCore();

    /*!
     * \brief Start Suassuna core (entities, players and some other modules).
     * \param useGUI Starts the GUI interface if set as True.
     * \return True if everything went ok and false otherwise.
     */
    [[nodiscard]] bool start(bool useGUI);

    /*!
     * \brief Stop Suassuna core (entities, players and some other modules).
     * \return True if everything went ok and false otherwise.
     */
    [[nodiscard]] bool stop();

private:
    // Modules
    GUI* _gui;
    WorldMap* _worldMap;
    BaseStation* _controller;
    Vision* _vision;

    // Teams
    QMap<Common::Enums::Color, Team*> _teams;

    // Entity manager
    Threaded::EntityManager *_entityManager;

    // Players list
    QList<Player*> _players;
};

#endif // SUASSUNA_H
