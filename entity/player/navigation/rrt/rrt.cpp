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

#include "rrt.h"
#include "rrtvertex.h"
#include <const/constants.h>
#include <utils/graph/graph.hh>

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define ROBOT_RADIUS 0.15

RRT::RRT() {
    srand(time(NULL));

    // RRT
    _rrtMaxTime = 5; // ms
    _rrtGrowVectorLength = 0.4;

    // Discrete point generation
    _discreteResolution = 0.05;
    _gridInitialized = false;

    // Path smoothing
    _smoothPathResolution = 0.35;
}

RRT::~RRT() {}

// Reset algorithm
void RRT::reset() {
    _obstacleList.clear();
}

// Add objects
void RRT::addBall(const Position &pos, const Velocity &vel) {
    addObstacles(pos, 2*MRCConstants::_ballRadius);
}

void RRT::addGoalArea(const Position &pos) {

}

void RRT::addOwnRobot(const Position &pos, const Velocity &vel) {
    addObstacles(pos, ROBOT_RADIUS);
}

void RRT::addEnemyRobot(const Position &pos, const Velocity &vel) {
    addObstacles(pos, ROBOT_RADIUS);
}

void RRT::run() {
    _pathMutex.lock();

    if(_path.isEmpty()) {
        // Find RRT path
        _path = findPath(originPos(), goalPos());

        // Optimize path
        minimizePath(0, _path.size()-1);
        smoothPath();

    } else {
        // Update current and goal position on list
        updatePositions();

        // If the current path has been blocked, find a new path, else try to minimize and smooth the current path
        Position breakPoint;

        if(isPathBlocked(&breakPoint)) {
            // Find RRT path to fix blocked path
            QList<Position> fixedPath = findPath(breakPoint, goalPos());

            // Append fixed path
            while(fixedPath.empty()==false)
                _path.prepend(fixedPath.takeLast());

            // Optimize path
            minimizePath(0, _path.size()-1);
            smoothPath();

        } else {
            // Optimize path
            minimizePath(0, _path.size()-2);
            smoothPath();

        }
    }

    // Path following: remove points from path while moving
    float dist = 0;
    do {
        int last = _path.size() - 1;
        Position nextPos = _path.at(last-1);
        dist = WR::Utils::distance(originPos(), nextPos);

        // Remove points when robot passes throgh it
        if(dist < 2.5*ROBOT_RADIUS) {
           if(_path.size() > 2)
               _path.removeAt(last-1);
        }

    } while(dist < 2.0*ROBOT_RADIUS && _path.size() > 2);

    // Path following: analyze segments to get closest segment
    if(_path.size() > 2) {
        float closestSegmentDistance = WR::Utils::distanceToSegment(_path.at(_path.size()-2), _path.at(_path.size()-3), originPos());
        for(int i=_path.size()-3; i>=1; i--) {
            Position pos1 = _path.at(i);
            Position pos2 = _path.at(i-1);

            float dist = WR::Utils::distanceToSegment(pos1, pos2, originPos());
            if(dist < closestSegmentDistance) {
                closestSegmentDistance = dist;

                // Remove all segments before that
                while(_path.size() > i+1)
                    _path.removeAt(i+1);
            }

        }
    }

    _pathMutex.unlock();

    // Return resultant angle
    if(_path.size()>=2) {
        int last = _path.size()-1;
        _resultantAngle = WR::Utils::getAngle(_path.at(last), _path.at(last-1));
    } else {
//        std::cout << "[ERROR] RRT::run(), generated path has only one point!\n";
        _resultantAngle = WR::Utils::getAngle(originPos(), goalPos());
    }
}

// Internal function for add obstacles in the obstacles list
void RRT::addObstacles(const Position &pos, float radius) {
    _obstacleList.append(RRTObstacle(pos, radius));
}

bool RRT::obstacleCloseToOrigin(const RRTObstacle &obst, float extraRadius) {
    float distObstOrigin = WR::Utils::distance(originPos(), obst.getPos());
    return (distObstOrigin < obst.getRadius() + extraRadius);
}

void RRT::checkGoalInsideObstacle() {
    for(int i=_obstacleList.size()-1; i>=0; i--) {
        RRTObstacle obst = _obstacleList.at(i);

        float distObstGoal = WR::Utils::distance(goalPos(), obst.getPos());
        if(distObstGoal < obst.getRadius() + ROBOT_RADIUS)
            _obstacleList.removeAt(i);
    }
}

// Create a randomic point
Position RRT::generateRandPoint() {
    float xRand, yRand;
    int xIndex, yIndex;
    do {
        // Generate two diferente numbers between 0.0 and 1.0
        float xSeed = (float) rand()/RAND_MAX;
        float ySeed = (float) rand()/RAND_MAX;

        // Generate a randomic x and y inside the field
        xRand = xSeed*loc()->fieldLength() - loc()->fieldLength()/2;
        yRand = ySeed*loc()->fieldWidth() - loc()->fieldWidth()/2;

        // Discretize point
        xIndex = (xRand/_discreteResolution) + (_xMaxIndex)/2;
        yIndex = (yRand/_discreteResolution) + (_yMaxIndex)/2;

    } while(_grid[xIndex][yIndex]);

    // Populate grid
    _grid[xIndex][yIndex] = true;

    return Position(true, xRand, yRand, 0.0);
}

// Checks if two point can connect
bool RRT::canConnect(const Position &posA, const Position &posB, float extraRadius, float extraRadiusIfClose) {

    for(int i=0; i<_obstacleList.size(); i++) {
        RRTObstacle obst = _obstacleList.at(i);

        bool closeToOrigin = obstacleCloseToOrigin(obst, extraRadius);
        float obstRadius = obst.getRadius() + (closeToOrigin? extraRadiusIfClose : extraRadius);

        // Check can connect
        float dist = WR::Utils::distanceToSegment(posA, posB, obst.getPos());
        if(dist < obstRadius)
            return false;
    }

    return true;
}

// Gets a vector in the direction and orientation of two points
Position RRT::getVector(const Position &near, const Position &rand, float vectorLength) {
    float theta = WR::Utils::getAngle(near, rand);
    return Position(true, vectorLength*cos(theta), vectorLength*sin(theta), 0.0);
}

// Minimizes number of points of the list
void RRT::minimizePath(int start, int end) {
    // Check minimal minimize size
    if(end-start <= 2)
        return;

    // Generate Graph to minimize path
    Graph graph;

    QList<RRTVertex*> vertices;

    // Generate vertices
    RRTVertex *origin=NULL;
    RRTVertex *goal=NULL;

    for(int i=start; i<=end; i++) {
        // Create
        RRTVertex *vertex = new RRTVertex(_path.at(i));
        vertices.append(vertex);

        // Add to graph
        graph.addVertex(vertex);

        // Create original path connection
        if(i>start) {
            RRTVertex *last = vertices.at(vertices.size()-2);
            graph.connect(vertex, last, WR::Utils::distance(vertex->getPosition(), last->getPosition()));
        }

        // Store origin and goal
        if(i==start)
            goal = vertex;
        if(i==end)
            origin = vertex;
    }

    // Generate edges
    for(int i=0; i<vertices.size(); i++) {
        RRTVertex *v1 = vertices.at(i);
        Position pos1 = v1->getPosition();

        for(int j=0; j<vertices.size(); j++) {
            RRTVertex *v2 = vertices.at(j);
            Position pos2 = v2->getPosition();

            // Create
            if(canConnect(pos1, pos2, ROBOT_RADIUS, ROBOT_RADIUS)) {
                float weight = WR::Utils::distance(pos1, pos2);
                graph.connect(v1, v2, weight);
            }
        }
    }

    // Get shortest path
    QLinkedList<Vertex*> shortestPath = graph.getShortestPath(goal, origin);

    // Clear old path
    for(int i=start; i<=end; i++)
        _path.removeAt(start);

    // Append minimized path
    int ins = start;
    QLinkedList<Vertex*>::iterator it;
    for(it=shortestPath.begin(); it!=shortestPath.end(); it++) {
        RRTVertex *vertex = static_cast<RRTVertex*>(*it);
        _path.insert(ins, vertex->getPosition());
        ins++;
    }

}

// Smooths path by add new points at list
void RRT::smoothPath() {
    QList<Position> smoothedPath;

    // Check line smoothing (trivial case)
    if(_path.size()==2 && WR::Utils::distance(_path.first(), _path.last()) > 2*_smoothPathResolution) {

        // Calc num points to add
        int numPoints = WR::Utils::distance(_path.first(), _path.last())/_smoothPathResolution;
        Position vec = getVector(_path.first(), _path.last(), _smoothPathResolution);

        // Add points
        for(int i=0; i<numPoints; i++) {
            Position lastPos = _path.at(_path.size()-2);
            Position newPos(true, lastPos.x() + vec.x(), lastPos.y() + vec.y(), 0.0);
            _path.insert(_path.size()-1, newPos);
        }

        return;
    }

    // Apply the smoothing algorithm
    bool keepRunning = true;
    while(keepRunning) {
        keepRunning = false;

        // Append first position to smoothPath
        smoothedPath.append(_path.first());

        // Go through all point except the extremes
        for(int i=1; i<_path.size()-1; i++) {
            float distA = WR::Utils::distance(_path.at(i), _path.at(i-1));
            float distB = WR::Utils::distance(_path.at(i), _path.at(i+1));

            // If the segments are smaller than the smooth resolution, keep them
            if(distA <= _smoothPathResolution && distB <= _smoothPathResolution) {
                smoothedPath.append(_path.at(i));
                continue;
            }

            // Keep running while still at least one segment larger than smooth path
            keepRunning = true;

            // Apply smoothing on the first segment (i-1 to i)
            if(distA > _smoothPathResolution) {
                // If the segment is too long, limit by smooth resolution, else limit by half of distance
                float smoothDistance = (distA > 2*_smoothPathResolution)? _smoothPathResolution : distA/2;

                // Create new point
                Position vec = getVector(_path.at(i), _path.at(i-1), smoothDistance);
                Position newPoint(true, _path.at(i).x() + vec.x(), _path.at(i).y() + vec.y(), 0.0);

                // Append
                smoothedPath.append(newPoint);
            }

            // If one of the segments are too close, do
            if(distA <= _smoothPathResolution || distB <= _smoothPathResolution)
                smoothedPath.append(_path.at(i));

            // Apply smoothing on the second segment (i to i+1)
            if(distB > _smoothPathResolution) {
                // If the segment is too long, limit by smooth resolution, else limit by half of distance
                float smoothDistance = (distB > 2*_smoothPathResolution)? _smoothPathResolution : distB/2;

                // Create new point
                Position vec = getVector(_path.at(i), _path.at(i+1), smoothDistance);
                Position newPoint(true, _path.at(i).x() + vec.x(), _path.at(i).y() + vec.y(), 0.0);

                // Append
                smoothedPath.append(newPoint);
            }

        }

        // Append the last point to tmp
        smoothedPath.append(_path.last());

        // Remove all points of path
        _path.clear();

        // Copy smooethed path
        for(int i=0;i<smoothedPath.length();i++) {
            _path.append(smoothedPath.at(i));
        }

        // Clear smoothed path
        smoothedPath.clear();
    }

}

QList<Position> RRT::findPath(const Position &origin, const Position &goal) {
    // Check trivial case
    if(canConnect(origin, goal, ROBOT_RADIUS, ROBOT_RADIUS)) {

        // Generate path
        QList<Position> path;
        path.append(goal);
        path.append(origin);

        return path;
    }

    // Check goal inside obstacle area
    checkGoalInsideObstacle();

    if(_gridInitialized==false) {
        // Initialize lenghts of discrete grid matrix
        _xMaxIndex = (int) (loc()->fieldLength() / _discreteResolution) + 1;
        _yMaxIndex = (int) (loc()->fieldWidth()  / _discreteResolution) + 1;
        _grid.resize(_xMaxIndex);
        _gridInitialized = true;
    }

    // Reset grid
    for(int i=0; i<_grid.size(); i++)
        _grid[i].fill(false, _yMaxIndex);

    // Initialize tree
    RRTTree* tree = new RRTTree();

    // Append the origin to the tree
    tree->append(new RRTNode(origin));

    // Starts timer for check exploration time limit
    _rrtTimer.start();

    // Creates random points until get max iteration or founds a path through plan to goal
    bool foundPath = false;
    while(foundPath==false) {
        // Generate a discrete rand point
        Position randPosition = generateRandPoint();

        // Get nearest point
        RRTNode* nearNode = tree->getNN(randPosition);

        // Find a vector Near-Rand
        Position unitVec = getVector(nearNode->getPosition(), randPosition, _rrtGrowVectorLength);

        // Sum vector to the near point to found the new point
        Position newPos(true, nearNode->getPosition().x() + unitVec.x(), nearNode->getPosition().y() + unitVec.y(), 0.0);

        // Check if can connect to new point
        if(canConnect(nearNode->getPosition(), newPos, ROBOT_RADIUS, 0.0)) {

            // Append new point to the tree
            RRTNode* leaf = new RRTNode(newPos);
            leaf->setNodeNN(nearNode);
            tree->append(leaf);

            // Check if found a path
            if(canConnect(newPos, goal, ROBOT_RADIUS, ROBOT_RADIUS))
                foundPath = true;

        }

        // Check max time
        _rrtTimer.stop();
        if(_rrtTimer.timemsec() >= _rrtMaxTime)
            break;
    }

//    std::cout << "RRT find path time: " << _rrtTimer.timemsec() << "\n";

    QList<Position> path;
    if(foundPath) {
        // Copy the generated path from tree to path list
        path = tree->getPath();

        // Prepend goal
        path.prepend(goal);

    } else {
//        std::cout << "[WARNING] RRT::findPath(), path not found!\n";
        path.append(goal);
        path.append(origin);
    }

    // Delete tree
    delete tree;

    return path;
}

// Check if path has been blocked
bool RRT::isPathBlocked(Position *breakPoint) {
    bool blocked = false;

    for(int i=1; i<_path.size(); i++) {
        // If you found a obstacle in the path, copy the last point before the obstacle and
        // remove all point starting from obstacle
        if(canConnect(_path.at(i-1), _path.at(i), ROBOT_RADIUS/3, 0.0)==false) {

            // Store break point
            *breakPoint = (i==_path.size()-1)? _path.at(i) : _path.at(i+1);

            // Remove sub path
            int remove = i;
            for(int r=0; r<=remove; r++)
                _path.takeFirst();
            i = 0; // reset to start of the path

            blocked = true;
        }
    }

    return blocked;
}

// Update current and goal position on list
void RRT::updatePositions() {
    // Update current position on list
    _path.removeLast();
    _path.push_back(originPos());

    // Update goal position on list
    _path.removeFirst();
    _path.push_front(goalPos());
}

// Return results
Angle RRT::getDirection() const {
    return Angle(true, _resultantAngle);
}

QLinkedList<Position> RRT::getPath() const {
    QMutexLocker locker(&_pathMutex);

    QLinkedList<Position> path;
    for(int i=0; i<_path.size(); i++)
        path.append(_path.at(i));
    return path;
}

float RRT::getDistance() const {
    QMutexLocker locker(&_pathMutex);

    float dist = 0;
    for(int i=0; i<_path.size()-1; i++)
        dist += WR::Utils::distance(_path.at(i), _path.at(i+1));
    return dist;
}
