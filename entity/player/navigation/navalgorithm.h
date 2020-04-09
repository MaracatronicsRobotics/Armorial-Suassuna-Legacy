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

#ifndef NAVALGORITHM_H
#define NAVALGORITHM_H

#include <GEARSystem/Types/types.hh>
#include<entity/locations.h>
#include <utils/utils.hh>

class NavAlgorithm {
public:
    NavAlgorithm();
    NavAlgorithm(const NavAlgorithm &copy);
    virtual ~NavAlgorithm();

    // Access to Navigation
    void initialize(Locations *loc);
    void runNavAlgorithm();

    // Reset algorithm
    virtual void reset() = 0;

    // Set origin and goal
    void setOrigin(const Position &pos, const Angle &ori, const Velocity &vel);
    void setGoal(const Position &pos, const Angle &ori);

    // Add obstacles
    virtual void addBall(const Position &pos, const Velocity &vel) = 0;
    virtual void addGoalArea(const Position &pos) = 0;
    virtual void addOwnRobot(const Position &pos, const Velocity &vel) = 0;
    virtual void addEnemyRobot(const Position &pos, const Velocity &vel) = 0;

    // Return results
    virtual Angle getDirection() const = 0;
    virtual float getDistance() const;
    virtual QList<Position> getPath() const;

protected:
    Locations* loc() { return _loc; }

    // Origin access
    Position originPos() const { return _originPos; }
    Angle originOri() const { return _originOri; }
    Velocity originVel() const { return _originVel; }

    // Goal access
    Position goalPos() const { return _goalPos; }
    Angle goalOri() const { return _goalOri; }

private:
    // Execute algorithm
    virtual void run() = 0;

    // Copy
    virtual NavAlgorithm* clone() const;

    // Locations access
    Locations *_loc;

    // NavAlgorithm positions
    // Origin
    Position _originPos;
    Angle _originOri;
    Velocity _originVel;
    // Goal
    Position _goalPos;
    Angle _goalOri;

    // Path generation
    void generatePath();
    bool _generatePath;
    QList<Position> _path;
    mutable QMutex _pathMutex;

    // Distance
    float _distance;
};

#endif // NAVALGORITHM_H
