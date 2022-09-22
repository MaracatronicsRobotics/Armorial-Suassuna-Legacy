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

#include "freeangles.h"

WorldMap* FreeAngles::_worldMap = nullptr;

void FreeAngles::setWorldMap(WorldMap *worldMap) {
    FreeAngles::_worldMap = worldMap;
}

WorldMap* FreeAngles::getWorldMap() {
    if(_worldMap == nullptr) {
        spdlog::error(Text::bold("WorldMap with nullptr value at FreeAngles"));
    } else {
        return _worldMap;
    }

    return nullptr;
}

QList<Obstacle> FreeAngles::getObstacles(const Position &watcher, float distanceRadius) {
    if(getWorldMap() == nullptr) {
        spdlog::error(Text::bold("WorldMap with nullptr value at FreeAngles::getObstacles(args)"));
        return QList<Obstacle>();
    }

    QList<Obstacle> obstacles;
    QList<Color> colors;
    colors.append(Utils::getColorObject(false));
    colors.append(Utils::getColorObject(true));

    // Add ball
    Obstacle obstBall;
    Ball ball = getWorldMap()->getBall();
    obstBall = ball;
    obstacles.push_back(obstBall);

    // Add robots
    for(Color c : colors) {
        QList<Robot> robots = getWorldMap()->getRobots(c);
        QList<Robot>::iterator it;

        for(it = robots.begin(); it != robots.end(); it++) {
            Robot robot = (*it);
            const Position rPosition = (*it).robotposition();

            if (!watcher.isinvalid() && watcher == rPosition) {
                continue;
            }

            if (!watcher.isinvalid()) {
                float distToPos = Utils::distance(watcher, rPosition);
                if (distToPos > distanceRadius) {
                    continue;
                }
            }

            Obstacle obst;
            obs = robot;
            obstacles.push_back(obst);
        }
    }

    return obstacles;
}

QList<Obstacle> FreeAngles::getObstacles(const Position &watcher) {
    return FreeAngles::getObstacles(watcher, 1000.0f);
}

QList<Obstacle> FreeAngles::getObstacles() {
    Position invalidPos = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    return FreeAngles::getObstacles(invalidPos, 1000.0f);
}

QList<AngleInterval> FreeAngles::getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos, bool returnObstructed) {
    float minPosAngle = Utils::getAngle(watcher, initialPos);
    float maxPosAngle = Utils::getAngle(watcher, finalPos);
    QList<Obstacle> obstacles = getObstacles(watcher);
    return _getFreeAngles(watcher, minPosAngle, maxPosAngle, obstacles, returnObstructed);
}

QList<AngleInterval> FreeAngles::getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos, const QList<Obstacle> &obstacles, bool returnObstructed) {
    float minPosAngle = Utils::getAngle(watcher, initialPos);
    float maxPosAngle = Utils::getAngle(watcher, finalPos);
    return _getFreeAngles(watcher, minPosAngle, maxPosAngle, obstacles, returnObstructed);
}

QList<AngleInterval> FreeAngles::getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, bool returnObstructed) {
    QList<Obstacle> obstacles = getObstacles(watcher);
    return _getFreeAngles(watcher, initialPosAngle, finalPosAngle, obstacles, returnObstructed);
}

QList<AngleInterval> FreeAngles::getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, const QList<Obstacle> &obstacles, bool returnObstructed) {
    return _getFreeAngles(watcher, initialPosAngle, finalPosAngle, obstacles, returnObstructed);
}

QList<AngleInterval> FreeAngles::_getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, const QList<Obstacle> &obstacles, bool returnObstructed) {
    QMap<float, quint8> freeAngles;

    Utils::angleLimitZeroTwoPi(&initialPosAngle);
    Utils::angleLimitZeroTwoPi(&finalPosAngle);

    if (initialPosAngle == finalPosAngle) {
        return QList<AngleInterval>();
    } else if (initialPosAngle < finalPosAngle) {
        freeAngles.insert(initialPosAngle, 0);
        freeAngles.insert(finalPosAngle, 0);
    } else {
        if (finalPosAngle == 0) {
            finalPosAngle = twoPi;
            freeAngles.insert(initialPosAngle, 0);
            freeAngles.insert(finalPosAngle, 0);
        } else {
            freeAngles.insert(0, 0);
            freeAngles.insert(finalPosAngle, 0);
            freeAngles.insert(initialPosAngle, 0);
            freeAngles.insert(twoPi, 0);
        }
    }

    QList<Obstacle> tmpObstacles = calcObstaclesObstruction(watcher, obstacles);

    for (int i = 0; i < tmpObstacles.size(); i++) {
        Obstacle obst = tmpObstacles.at(i);
        Angle obstInitialAngle = obst.initialAngle();
        Angle obstFinalAngle = obst.finalAngle();

        if (obstInitialAngle > obstFinalAngle) {
            Obstacle newObst1 = obst;
            newObst1.setInitialAngle(Utils::getAngleObject(0.0f, false, false));
            newObst1.setFinalAngle(obst.finalAngle());

            Obstacle newObst2 = obst;
            newObst2.setInitialAngle(obst.initialAngle());
            newObst2.setFinalAngle(Utils::getAngleObject(twoPi, false, false));

            obst = newObst1;
            tmpObstacles.push_back(newObst2);
        }

        QMap<float, quint8> tmpFreeAngles(freeAngles);
        int size = tmpFreeAngles.size();

        if (size % 2 == 0) {
            for (int fa = 0; fa < size; fa += 2) {
                const float initAngle = tmpFreeAngles.keys().at(fa);
                const float finalAngle = tmpFreeAngles.keys().at(fa+1);

                if (obst.initialAngle() < initAngle && obst.finalAngle() <= initAngle) {
                    // ok
                } else if (obst.initialAngle() <= initAngle && obst.finalAngle() > initAngle && obst.finalAngle() < finalAngle) {
                    freeAngles.remove(initAngle);
                    freeAngles.insert(obst.finalAngle().value(), 0);
                } else if (obst.initialAngle() > initAngle && obst.initialAngle() < finalAngle && obst.finalAngle() > initAngle && obst.finalAngle() < finalAngle) {
                    freeAngles.insert(obst.initialAngle().value(), 0);
                    freeAngles.insert(obst.finalAngle().value(), 0);
                } else if (obst.initialAngle() > initAngle && obst.initialAngle() < finalAngle && obst.finalAngle() >= finalAngle) {
                    freeAngles.remove(finalAngle);
                    freeAngles.insert(obst.initialAngle().value(), 0);
                } else if (obst.initialAngle() >= finalAngle && obst.finalAngle() > finalAngle) {
                    // ok
                } else if (obst.initialAngle() <= initAngle && obst.finalAngle() >= finalAngle) {
                    freeAngles.remove(initAngle);
                    freeAngles.remove(finalAngle);
                }
            }
        }
    }

    QList<AngleInterval> retn;
    for (int i = 0; i < freeAngles.size(); i++) {
        int nextI = i+1;
        if (nextI >= freeAngles.size()) {
            nextI = 0;
        }

        float initAngle = freeAngles.keys().at(i);
        float finalAngle = freeAngles.keys().at(nextI);
        bool obstructed = (i % 2) != 0;

        if (!obstructed || (obstructed && returnObstructed)) {
            if (initAngle == twoPi && finalAngle == 0.0f) {
                continue;
            }

            retn.push_back(AngleInterval(initAngle, finalAngle, obstructed));
        }
    }

    if (!retn.empty()) {
        int size = retn.size();
        int i = 0;
        for (int c = 0; c < size + 1; c++) {
            int nextI = i + 1;
            if (nextI >= retn.size()) {
                nextI = 0;
            }

            AngleInterval curr = retn.at(i);
            AngleInterval next = retn.at(nextI);
            if (((curr.getFinalAngle() == next.getInitialAngle()) || (next.getInitialAngle() == (curr.getFinalAngle() - twoPi))) && curr.isObstructed() == next.isObstructed()) {
                retn.removeAt(i);
                retn.removeAt(nextI);
                retn.insert(i, AngleInterval(curr.getInitialAngle(), next.getFinalAngle(), curr.isObstructed()));
            }
            i++;
            if (i >= retn.size()) {
                i = 0;
            }
        }
    }

    return retn;
}

QList<Obstacle> FreeAngles::calcObstaclesObstruction(const Position &watcher, const QList<Obstacle> &obstacles) {
    QList<Obstacle> retnObstacles;
    for (int i = 0; i < obstacles.size(); i++) {
        Obstacle obst = obstacles.at(i);
        if (watcher == obst.position()) {
            continue;
        }

        obst.calcAnglesFrom(watcher);
        retnObstacles.push_back(obst);
    }

    return retnObstacles;
}
