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

#ifndef NAVIGATIONALGORITHM_H
#define NAVIGATIONALGORITHM_H

#define NAVALG_MAXTIME 5 // ms

#include <QTimer> // Check new Armorial lib timer
#include <QReadWriteLock>
#include <QLinkedList>

#include <src/gui/gui.h>
#include <src/constants/constants.h>
#include <src/entities/baseCoach.h>
#include <src/utils/utils.h>

class NavigationAlgorithm
{
public:
    NavigationAlgorithm();
    NavigationAlgorithm(const NavigationAlgorithm &copy);
    virtual ~NavigationAlgorithm();

    virtual QString name() = 0;

    void initialize(WorldMap *worldMap, GUI *gui, Constants *constants);
    void runNavigationAlgorithm();

    virtual void reset() = 0;

    // Setters origin and goal pos
    void setOrigin(const Position &pos, const Angle &ori, const Velocity &vel);
    void setGoal(const Position &pos, const Angle &ori);

    // Insert obstacles
    virtual void addBall(const Position &pos, const Velocity &vel);
    virtual void addGoalArea(const Position &pos) = 0;
    virtual void addTeamRobot(const Position &pos, const Velocity &vel) = 0;
    virtual void addEnemyRobot(const Position &pos, const Velocity &vel) = 0;

    // Getters
    virtual Angle getDirection() const = 0;
    virtual float getDistance() const;
    virtual QLinkedList<Position> getPath() const;

protected:
    WorldMap* getWorldMap();
    GUI* getGUI();
    Constants* getConstants();

    Position originPos() { return _originPos; }
    Angle originOri() { return _originOri; }
    Velocity originVel() { return _originVel; }

    Position goalPos() { return _goalPos; }
    Angle goalOri() { return _goalOri; }

private:
    virtual void run() = 0;

    virtual NavigationAlgorithm* clone() const;

    // Origin
    Position _originPos;
    Angle _originOri;
    Velocity _originVel;

    // Goal
    Position _goalPos;
    Angle _goalOri;

    // Internal
    WorldMap *_worldMap;
    GUI *_gui;
    Constants *_constants;

    // Path gen
    void generatePath();
    bool _generatePath;
    QLinkedList<Position> _path;
    float _distance;

    QReadWriteLock _mutexPath;
};

#endif // NAVIGATIONALGORITHM_H
