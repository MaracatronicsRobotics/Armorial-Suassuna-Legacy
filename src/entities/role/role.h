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

#include <QObject>

#include <src/constants/constants.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/entities/referee/referee.h>
#include <src/entities/player/player.h>
#include <src/entities/player/behaviour/behaviour.h>

class Role : public QObject {
    Q_OBJECT
public:
    Role();
    virtual ~Role();

    bool isInitialized();
    void initialize(Constants *constants, WorldMap *worldMap, SSLReferee *referee);
    void setPlayer(Player *player);

    void runRole();

    virtual QString name() = 0;

    Behaviour* getActualBehaviour();
    virtual void initializeBehaviours() = 0;
    QHash<int, Behaviour*> getBehaviours();
    void setBehaviour(int behaviourID);

    Player* player();

protected:
    void addBehaviour(int behaviourID, Behaviour *behaviour);

    Constants* getConstants();
    WorldMap* getWorldMap();
    SSLReferee* getReferee();
    Locations* getLocations();

private:
    virtual void configure() = 0;
    virtual void run() = 0;

    Player *_player;

    Constants *_constants;
    WorldMap *_worldMap;
    SSLReferee *_referee;
    Locations *_loc;

    // Behaviours for game status control
    Behaviour *_goToLookTo; // Future changes: Behaviour -> GoToLookTo (wich extends behaviour)
    bool _isHalted;

    // Behaviour list
    QHash<int, Behaviour*> _behaviourList;
    Behaviour *_behaviour;
    Behaviour *_actualBehaviour;

    bool _initialized;
    bool _configureEnabled;
};

#endif // ROLE_H
