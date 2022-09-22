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

#ifndef UNIVECTORFIELD_H
#define UNIVECTORFIELD_H

#define OBSTACLE_RADIUS 0.45f

#include <src/entities/player/navigation/navigationalgorithm.h>
#include <src/utils/freeangles/freeangles.h>

class UnivectorField : public NavigationAlgorithm
{
public:
    UnivectorField();
    ~UnivectorField();

    QString name();

    void reset();

    // Add obstacles
    void addBall(const Position &pos, const Velocity &vel);
    void addGoalArea(const Position &pos);
    void addTeamRobot(const Position &pos, const Velocity &vel);
    void addEnemyRobot(const Position &pos, const Velocity &vel);

    // Obstacles radius factor
    static void setObstacleRadiusFactor(float factor);

    void run();

    Angle getDirection() const;

private:
    UnivectorField* clone() const;

    static float _obstacleRadiusFactor;

    void addObstacle(const Position &pos, const float &radius = OBSTACLE_RADIUS);

    Angle _resultantAngle;

    QList<Obstacle> _obstacles;
};

#endif // UNIVECTORFIELD_H
