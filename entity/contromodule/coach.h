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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <entity/contromodule/basecoach.h>
#include <entity/contromodule/controlmodule.h>
#include <entity/coachview/coachview.h>

class Coach : public ControlModule
{
public:
    Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam);
    virtual ~Coach();
    QString name();

    QString getAgressivity();
    void setStrategy(Strategy *strat);
private:
    // run controller
    void run();

    // Game info
    SSLReferee *_ref;
    CoachUtils *_utils;

    // Teams
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // Strategy
    QMutex _mutexStrategy;
    QMutex *_UIMutex;
    Strategy *_strat;
    Strategy* strategy();

    // Agressivity
    QHash<std::string, std::vector<std::vector<double>>> _agressivityClusters;
    void loadClusters();
    std::string calculateAgressivity(std::vector<double> &gaussians);
    std::vector<double> getEnemyGaussiansInAreas();
    std::string _lastAgressivity;

    std::vector<double> proportionsOld;
};

#endif // CONTROLLER_H
