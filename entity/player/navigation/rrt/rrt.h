/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
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

#ifndef RRT_HH
#define RRT_HH

#include <QMutexLocker>
#include <entity/player/navigation/navalgorithm.h>
#include <entity/player/navigation/rrt/rrtobstacle.h>
#include <entity/player/navigation/rrt/rrtnode.h>
#include <entity/player/navigation/rrt/rrttree.h>
#include <utils/wrtimer/wrtimer.hh>

class RRT : public NavigationAlgorithm {
public:
    RRT();
    ~RRT();

    // Reset algorithm
    void reset();

    // Add obstacles
    void addBall(const Position &pos, const Velocity &vel);
    void addGoalArea(const Position &pos);
    void addOwnRobot(const Position &pos, const Velocity &vel);
    void addEnemyRobot(const Position &pos, const Velocity &vel);

    // Run
    void run();

    // Return results
    Angle getDirection() const;
    QLinkedList<Position> getPath() const;
    float getDistance() const;

private:
    // RRT
    WRTimer _rrtTimer;
    int   _rrtMaxTime;
    float _rrtGrowVectorLength;

    // Path smoothing
    float _smoothPathResolution;

    // Rand discrete point generation
    float _discreteResolution;
    QVector<QVector<bool> > _grid;
    int _xMaxIndex;
    int _yMaxIndex;
    bool _gridInitialized;

    // List of the path points and obstacles
    QList<Position> _path;
    QList<RRTObstacle> _obstacleList;
    mutable QMutex _pathMutex;

    // Resultant angle
    float _resultantAngle;

    // Create a randomic point
    Position generateRandPoint();

    // Internal addObstacles
    void addObstacles(const Position &pos, float radius);
    bool obstacleCloseToOrigin(const RRTObstacle &obst, float extraRadius);
    void checkGoalInsideObstacle();

    // Checks if two point can connect
    bool canConnect(const Position &posA, const Position &posB, float extraRadius, float extraRadiusIfClose);

    // Gets a unit vector in the direction and orientation of two points
    Position getVector(const Position &near, const Position &rand, float vectorLength);

    // Minimizes number of points of the list
    void minimizePath(int start, int end);

    // Smooths path by add new points at list
    void smoothPath();

    // Finds a path
    QList<Position> findPath(const Position &origin, const Position &goal);

    // Check if path has been blocked
    bool isPathBlocked(Position *breakPoint);

    // Update current and goal position on list
    void updatePositions();
};

#endif // RRT_H
