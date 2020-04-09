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

#ifndef FASTPATHPLANNING_H
#define FASTPATHPLANNING_H

#include <entity/player/navigation/navalgorithm.h>
#include <GEARSystem/gearsystem.hh>
#include <const/constants.h>
#include <utils/mrctimer/mrctimer.h>

class FastPathPlanning : public NavAlgorithm
{
public:
    FastPathPlanning();
    ~FastPathPlanning();

    void reset();
    void addBall(const Position &pos, const Velocity &vel);
    void addOwnRobot(const Position &pos, const Velocity &vel);
    void addEnemyRobot(const Position &pos, const Velocity &vel);
    void addGoalArea(const Position &pos);

    void run();
    bool getPaths(QList<Position> &rec, int h, MRCTimer *timer);

    Angle getDirection() const;
    float getDistance() const;
    QList<Position> getPath() const;
    QList<QList<Position>> getAllPath() const;

private:
    std::pair<Position, Position> findPoint(QList<Position> &list, Position colisionPoint,int iteratorPoints);
    Position hasCollisionAtLine(Position pos);
    void addColisionPosition(Position _pos, Boolean _isBall);
    QList<std::pair<Position, Boolean>> _colisionPosition;
    QList<Position> _path;
    QList<QList<Position>> _allPath;

    // Path smoothing
    float _smoothPathResolution;
    QList<Position> _smoothPath;
    void smoothPath(QList<Position> &path);
    Position getVector(const Position &near, const Position &rand, float vectorLength);

    NavAlgorithm* clone() const;
};

#endif // FASTPATHPLANNING_H
