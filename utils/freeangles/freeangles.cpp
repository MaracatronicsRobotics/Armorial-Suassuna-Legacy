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
#include <iostream>
#include <GEARSystem/gearsystem.hh>
#include <entity/contromodule/mrcteam.h>
#include <entity/player/player.h>
#include <utils/utils.hh>

MRCTeam* FreeAngles::_ourTeam = NULL;
MRCTeam* FreeAngles::_opTeam = NULL;

QList<Obstacle> FreeAngles::getObstacles(const Position &watcher, float distanceRadius) {
    if(_ourTeam==NULL || _opTeam==NULL) {
        std::cout << "[ERROR] FreeAngles::getObstacles() cannot be called with _ourTeam==NULL || _opTeam==NULL!\n";
        return QList<Obstacle>();
    }

    QList<Obstacle> obstacles;

    // Fill obstacles
    for(int team=0; team<2; team++) {
        MRCTeam *wrTeam = (team==0? _ourTeam : _opTeam);

        const QList<Player*> players = wrTeam->avPlayers().values();
        QList<Player*>::const_iterator it;
        for(it=players.constBegin(); it!=players.constEnd(); it++) {
            const Player *player = *it;
            const Position posPlayer = player->position();

            // Check if watcher==pos
            if(watcher.isUnknown()==false && watcher.x()==posPlayer.x() && watcher.y()==posPlayer.y())
                continue;

            // Check radius
            if(watcher.isUnknown()==false) {
                float distPos = WR::Utils::distance(watcher, posPlayer);
                if(distPos > distanceRadius)
                    continue;
            }

            // Update info
            Obstacle obst;
            obst.id() = player->playerId();
            obst.team() = player->teamId();
            obst.position() = player->position();
            obst.radius() = 0.09;

            // Push to list
            obstacles.push_back(obst);
        }
    }

    // DEBUG PRINT OBSTACLES
//    std::cout << "Obstacles:";
//    for(int i=0; i<obstacles.size(); i++) {
//        Obstacle obst = obstacles.at(i);
//        std::cout << " " << (int)obst.team() << ":" << (int)obst.id();
//    }
//    std::cout << std::endl;

    return obstacles;
}

QList<FreeAngles::Interval> FreeAngles::getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos, bool returnObstructed) {
    float minPosAngle = WR::Utils::getAngle(watcher, initialPos);
    float maxPosAngle = WR::Utils::getAngle(watcher, finalPos);
    QList<Obstacle> obstacles = getObstacles(watcher);
    return _getFreeAngles(watcher, minPosAngle, maxPosAngle, obstacles, returnObstructed);
}

QList<FreeAngles::Interval> FreeAngles::getFreeAngles(const Position &watcher, const Position &initialPos, const Position &finalPos, const QList<Obstacle> &obstacles, bool returnObstructed) {
    float minPosAngle = WR::Utils::getAngle(watcher, initialPos);
    float maxPosAngle = WR::Utils::getAngle(watcher, finalPos);
    return _getFreeAngles(watcher, minPosAngle, maxPosAngle, obstacles, returnObstructed);
}

QList<FreeAngles::Interval> FreeAngles::getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, bool returnObstructed) {
    QList<Obstacle> obstacles = getObstacles(watcher);
    return _getFreeAngles(watcher, initialPosAngle, finalPosAngle, obstacles, returnObstructed);
}

QList<FreeAngles::Interval> FreeAngles::getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, const QList<Obstacle> &obstacles, bool returnObstructed) {
    return _getFreeAngles(watcher, initialPosAngle, finalPosAngle, obstacles, returnObstructed);
}

QList<FreeAngles::Interval> FreeAngles::_getFreeAngles(const Position &watcher, float initialPosAngle, float finalPosAngle, const QList<Obstacle> &obstacles, bool returnObstructed) {
    QMap<float,quint8> freeAngles;

    // Ensure min/max-PosAngle are in 0-360 range
    WR::Utils::angleLimitZeroTwoPi(&initialPosAngle);
    WR::Utils::angleLimitZeroTwoPi(&finalPosAngle);

//    std::cout << "initialPosAngle: " << GEARSystem::Angle::toDegrees(initialPosAngle) << "\n";
//    std::cout << "finalPosAngle: " << GEARSystem::Angle::toDegrees(finalPosAngle) << "\n";

    // Check I==F
    if(initialPosAngle==finalPosAngle)
        return QList<Interval>();

    // I > F
    if(initialPosAngle>finalPosAngle) {
        if(finalPosAngle==0) {
            finalPosAngle = GEARSystem::Angle::twoPi;
            freeAngles.insert(initialPosAngle, 0);
            freeAngles.insert(finalPosAngle, 0);
        } else {
            freeAngles.insert(0, 0);
            freeAngles.insert(finalPosAngle, 0);
            freeAngles.insert(initialPosAngle, 0);
            freeAngles.insert(GEARSystem::Angle::twoPi, 0);
        }

    // F > I
    } else {
        freeAngles.insert(initialPosAngle, 0);
        freeAngles.insert(finalPosAngle, 0);
    }

    /// DEBUG PRINT FREE ANGLES
//    std::cout << "***\nFree angles 1 (" << freeAngles.size()/2 << "):";
//    for(int i=0; i<freeAngles.size(); i+=2) {
//        float angI = freeAngles.keys().at(i);
//        float angF = freeAngles.keys().at(i+1);
//        std::cout << " " << GEARSystem::Angle::toDegrees(angI) << ":" << GEARSystem::Angle::toDegrees(angF);
//    }
//    std::cout << "\n";

    // Calc obstacles obstruction angles
    QList<Obstacle> tmpObstacles = calcObstaclesObstruction(watcher, obstacles);

    // For each obstacle
    for(int i=0; i<tmpObstacles.size(); i++) {
        Obstacle obst = tmpObstacles.at(i);

//        std::cout << "obstacle #" << i << ", initial=" << GEARSystem::Angle::toDegrees(obst.initialAngle()) << ", final=" << GEARSystem::Angle::toDegrees(obst.finalAngle()) << "\n";

        // I > F
        if(obst.initialAngle()>obst.finalAngle()) {
            Obstacle newObst1 = obst;
            newObst1.initialAngle() = 0;
            newObst1.finalAngle() = obst.finalAngle();

            Obstacle newObst2 = obst;
            newObst2.initialAngle() = obst.initialAngle();
            newObst2.finalAngle() = GEARSystem::Angle::twoPi;

            obst = newObst1;
            tmpObstacles.push_back(newObst2);
        }

        // Runs in freeAngles to add obstruction
        QMap<float,quint8> tmpFreeAngles(freeAngles);
        int size = tmpFreeAngles.size();
        if(size%2==0) {
            for(int fa=0; fa<size; fa+=2) {
                const float angI = tmpFreeAngles.keys().at(fa);
                const float angF = tmpFreeAngles.keys().at(fa+1);

//                std::cout << "testing with interval, angI=" << GEARSystem::Angle::toDegrees(angI) << ", angF=" << GEARSystem::Angle::toDegrees(angF) << "\n";

                if(obst.initialAngle()<angI && obst.finalAngle()<=angI) {
//                    std::cout << "caso 1\n";

                } else if(obst.initialAngle()<=angI && obst.finalAngle()>angI && obst.finalAngle()<angF) {
//                    std::cout << "caso 2\n";
                    freeAngles.remove(angI);
                    freeAngles.insert(obst.finalAngle(), 0);
                } else if(obst.initialAngle()>angI && obst.initialAngle()<angF && obst.finalAngle()>angI && obst.finalAngle()<angF) {
//                    std::cout << "caso 3\n";
                    freeAngles.insert(obst.initialAngle(), 0);
                    freeAngles.insert(obst.finalAngle(), 0);
                } else if(obst.initialAngle()>angI && obst.initialAngle()<angF && obst.finalAngle()>=angF) {
//                    std::cout << "caso 4\n";
                    freeAngles.remove(angF);
                    freeAngles.insert(obst.initialAngle(), 0);
                } else if(obst.initialAngle()>=angF && obst.finalAngle()>angF) {
//                    std::cout << "caso 5\n";

                } else if(obst.initialAngle()<=angI && obst.finalAngle()>=angF) {
//                    std::cout << "caso 6\n";
                    freeAngles.remove(angI);
                    freeAngles.remove(angF);
                } else {
//                    std::cout << "caso da morte\n";

                }
            }
        }
    }

    /// DEBUG PRINT FREE ANGLES
//    std::cout << "***\nFree angles 2 (" << freeAngles.size()/2 << "):";
//    for(int i=0; i<freeAngles.size(); i+=2) {
//        float angI = freeAngles.keys().at(i);
//        float angF = freeAngles.keys().at(i+1);
//        std::cout << " " << GEARSystem::Angle::toDegrees(angI) << ":" << GEARSystem::Angle::toDegrees(angF);
//    }
//    std::cout << "\n";

    // Construct return
    QList<FreeAngles::Interval> retn;
    for(int i=0; i<freeAngles.size(); i++) {
        int nextI = i+1;
        if(nextI>=freeAngles.size())
            nextI = 0;

        // Get info
        float angI = freeAngles.keys().at(i);
        float angF = freeAngles.keys().at(nextI);
        bool obstructed = (i%2)!=0;

        if(!obstructed || (obstructed && returnObstructed)) {
            // Remove initial==360 && final==0
            if(angI==GEARSystem::Angle::twoPi && angF==0.0)
                continue;

            retn.push_back(Interval(angI, angF, obstructed));
        }
    }

    // Merge intervals
    if(retn.empty()==false) {
        int size = retn.size();
        int i=0;
        for(int c=0; c<size+1; c++) {
            int nextI = i+1;
            if(nextI>=retn.size())
                nextI = 0;

            // Check merge
            Interval curr = retn.at(i);
            Interval next = retn.at(nextI);
            if((curr.angFinal()==next.angInitial() || (curr.angFinal()-GEARSystem::Angle::twoPi==next.angInitial())) && curr.obstructed()==next.obstructed()) {
                retn.removeAt(i);
                retn.removeAt(nextI);
                retn.insert(i, Interval(curr.angInitial(), next.angFinal(), curr.obstructed()));
            }

            i++;
            if(i>=retn.size())
                i = 0;
        }
    }

    // Remove empty intervals

    /// DEBUG PRINT FREE ANGLES
//    std::cout << "***\nFree angles 3 (" << retn.size() << "):";
//    for(int i=0; i<retn.size(); i++) {
//        float angI = retn.at(i).angInitial();
//        float angF = retn.at(i).angFinal();
//        bool obstructed = retn.at(i).obstructed();
//        std::cout << " " << GEARSystem::Angle::toDegrees(angI) << ":" << GEARSystem::Angle::toDegrees(angF) << ":" << obstructed;
//    }
//    std::cout << "\n";

    // Return free angles
    return retn;
}

QList<Obstacle> FreeAngles::calcObstaclesObstruction(const Position &watcher, const QList<Obstacle> &obstacles) {
    // Iterate through obstacles and calc obstruction
    QList<Obstacle> retnObstacles;
    for(int i=0; i<obstacles.size(); i++) {
        Obstacle obst = obstacles.at(i);

        // Continue if obstacle is the watcher
        if(watcher.x()==obst.position().x() && watcher.y()==obst.position().y())
            continue;

        obst.calcAnglesFrom(watcher);
        retnObstacles.push_back(obst);
    }
    return retnObstacles;
}

QList<Obstacle> FreeAngles::getObstacles(const Position &watcher) {
    return FreeAngles::getObstacles(watcher, 1000.0f);
}

QList<Obstacle> FreeAngles::getObstacles() {
    return FreeAngles::getObstacles(Position(false, 0.0, 0.0, 0.0), 1000.0f);
}
