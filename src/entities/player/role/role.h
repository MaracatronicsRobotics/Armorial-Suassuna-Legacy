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


#ifndef ROLE_H
#define ROLE_H

#include <src/entities/basesuassuna.h>

class Role : public QObject
{
    Q_OBJECT
public:
    Role();
    virtual ~Role();

    // Name
    virtual inline QString name() {
        return NAMEOF_TYPE_RTTI(*this).data();
    }

    // Init role control
    bool isInitialized();
    void initialize(WorldMap *worldMap);
    void setPlayer(Player *player);

    // Actual Behavior name
    QString actualBehaviorName();

    // Method to run in playbook
    void runRole();

protected:
    // Role control methods
    void addBehavior(int id, Behavior *behavior);
    void setBehavior(int id);

    // Player and constants getters
    Player* player();
    WorldMap* getWorldMap();

private:
    // Virtual implementation in inherited classes
    virtual void configure() = 0;
    virtual void run() = 0;

    // Player access
    Player *_player;

    // Worldmap
    WorldMap *_worldMap;

    // Behavior list
    QMap<int, Behavior*> _behaviorList;
    Behavior *_actualBehavior;

    // Initialize control
    bool _initialized;
};

#endif // ROLE_H
