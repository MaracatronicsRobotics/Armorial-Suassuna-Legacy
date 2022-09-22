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

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <src/entities/player/navigation/navigationalgorithm.h>

class Navigation
{
public:
    Navigation(Player *player, NavigationAlgorithm *navAlgorithm, WorldMap *worldMap, GUI *gui, Constants *constants);
    virtual ~Navigation();

    // Setters
    void setGoal(const Position &destination, const Angle &orientation, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea);
    void setNavigationAlgorithm(NavigationAlgorithm *navAlgorithm);

    // Runner
    void runAlgorithm();

    // Getters
    Angle getDirection() const;
    float getDistance() const;
    QLinkedList<Position> getPath() const;
    QString getNavAlgName() const;

protected:
    Player* getPlayer();
    WorldMap* getWorldMap();
    GUI* getGUI();
    NavigationAlgorithm* getNavAlg();
    Constants* getConstants();

private:
    Player *_player;
    WorldMap *_worldMap;
    GUI *_gui;
    NavigationAlgorithm *_navAlgorithm;
    Constants *_constants;

    QReadWriteLock _mutexNavAlg;
};

#endif // NAVIGATION_H
