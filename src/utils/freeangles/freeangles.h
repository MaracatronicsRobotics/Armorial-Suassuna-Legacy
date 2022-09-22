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

#ifndef FREEANGLES_H
#define FREEANGLES_H

#include <src/utils/freeangles/angleinterval/angleinterval.h>
#include <src/utils/freeangles/obstacle/obstacle.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/utils/utils.h>

class FreeAngles
{
public:
    static void setWorldMap(WorldMap* worldMap);

    static QList<Obstacle> getObstacles();
    static QList<Obstacle> getObstacles(const Position &watcher);
    static QList<Obstacle> getObstacles(const Position &watcher, float distanceRadius);

    static QList<AngleInterval> getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos, bool returnObstructed = false);
    static QList<AngleInterval> getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos, const QList<Obstacle> &obstacles, bool returnObstructed = false);
    static QList<AngleInterval> getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, bool returnObstructed = false);
    static QList<AngleInterval> getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, const QList<Obstacle> &obstacles, bool returnObstructed = false);

private:
    static constexpr float twoPi = 6.28318530717958647692;

    static WorldMap* _worldMap;
    static WorldMap* getWorldMap();

    static QList<AngleInterval> _getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, const QList<Obstacle> &obstacles, bool returnObstructed = false);
    static QList<Obstacle> calcObstaclesObstruction(const Position &watcher, const QList<Obstacle> &obstacles);
};

#endif // FREEANGLES_H
