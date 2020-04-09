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

#ifndef STRATEGYSTATE_H
#define STRATEGYSTATE_H

#include <entity/contromodule/basecoach.h>
#include <entity/referee/SSLReferee/sslgameinfo.h>

class StrategyState {
public:
    StrategyState();
    virtual ~StrategyState();

    // Called one time before run is first called
    bool isInitialized() { return _initialized; }
    void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, CoachUtils *utils, PlayersDistribution *dist, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref);

    // Called in Coach -> Strategy loop
    void runStrategyState();
    virtual QString name() = 0;
protected:
    // Playbook functions
    void usesPlaybook(Playbook *playbook);

    // canKickBall access (normal start)
    bool canKickBall() const;

    // Ball possession access
    bool hasBallPossession() const;
    quint8 ballPossession() const;

    // Utils, loc and dist access
    CoachUtils* utils() { return _utils; }
    Locations* loc() const;
    PlayersDistribution* dist() { return _dist; }
private:
    // Implemented by children
    virtual void configure(int numOurPlayers) = 0;
    virtual void run(int numOurPlayers) = 0;
    bool _configureEnabled;

    // Players access; used to give access to playbook
    int _lastNumOurPlayers;
    PlayersDistribution *_dist;
    qint8 *_kickerId;
    SSLGameInfo::RefProcessedState *_lastState;
    SSLReferee *_ref;

    // Game info
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;
    CoachUtils *_utils;

    // Playbook list
    QList<Playbook*> _playbookList;
    void clearOldPlaybook();
    void setCurrPlaybookToOld();
    QList<Playbook*> _oldPlaybook;

    // StrategyState initialized
    bool _initialized;
};

#endif // STRATEGYSTATE_H
