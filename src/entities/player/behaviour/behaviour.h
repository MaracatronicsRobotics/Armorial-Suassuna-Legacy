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

#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

//#include <src/entities/coach/basecoach.h>
#include <src/constants/constants.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/entities/player/player.h>
#include <src/entities/player/skill/skill.h>

class Behaviour
{
public:
    Behaviour();
    virtual ~Behaviour();

    // Behavior name (for debug)
    virtual QString name() = 0;

    // Init skill control
    bool isInitialized();
    void initialize(Constants *constants, WorldMap *worldMap);
    void setPlayer(Player *player);

    // Method to run in role
    void runBehaviour();

protected:
    // Skill control methods
    void addSkill(int id, Skill *skill);
    void setSkill(int id);

    // Player and constants getters
    Player* player();
    Constants* getConstants();
    WorldMap* getWorldMap();
    Locations* loc();

private:
    // Virtual implementation in inherited classes
    virtual void configure() = 0;
    virtual void run() = 0;

    // Player access
    Player *_player;

    // Constants
    Constants *_constants;

    // WorldMap
    WorldMap *_worldMap;

    // Skills list
    QMap<int, Skill*> _skillList;
    Skill *_actualSkill;

    // Initialize control
    bool _initialized;
};

#endif // BEHAVIOUR_H
