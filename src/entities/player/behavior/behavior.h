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


#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <src/entities/basesuassuna.h>

class Behavior
{
public:
    Behavior();
    virtual ~Behavior();

    // Name
    virtual inline QString name() {
        return NAMEOF_TYPE_RTTI(*this).data();
    }

    // Init skill control
    bool isInitialized();
    void initialize(WorldMap* worldMap);
    void setPlayer(Player* player);

    // Method to run in role
    void runBehavior();

protected:
    // Skill control methods
    void addSkill(int id, Skill *skill);
    void runSkill(int id);

    // Player and constants getters
    WorldMap* getWorldMap();
    Player* player();

private:

    // Virtual implementation in inherited classes
    virtual void configure() = 0;
    virtual void run() = 0;

    // Player access
    Player *_player;

    // WorldMap
    WorldMap *_worldMap;

    // Skills list
    QMap<int, Skill*> _skillList;
    Skill *_actualSkill;

    // Initialize control
    bool _initialized;
};

#endif // BEHAVIOR_H
