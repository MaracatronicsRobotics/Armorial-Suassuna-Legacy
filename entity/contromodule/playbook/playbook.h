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

#ifndef PLAYBOOK_H
#define PLAYBOOK_H

#include <climits>
#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/playersdistribution.h>
#include <entity/player/behaviour/behaviour.h>
#include <entity/player/role/role.h>

class Playbook : public QObject {
    Q_OBJECT
public:
    Playbook();
    virtual ~Playbook();

    // Called one time before run is first called
    bool isInitialized() { return _initialized; }
    void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, CoachUtils *utils, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref);

    // Called in StrategyState loop
    void runPlaybook(QString strategyState);
    void clearOldRoles();

    // Players manipulation
    void addPlayer(quint8 id);
    void addPlayers(const QList<quint8> &ids);
    void clearPlayers();
    int numPlayers();
    virtual QString name() = 0;
protected:
    // Players on this playbook access
    bool hasPlayer(quint8 id);
    void setPlayerRole(quint8 id, Role *role);

    // Role functions
    void usesRole(Role *role);

    // canKickBall access (normal start)
    bool canKickBall() const;

    // Utils, loc and dist access
    CoachUtils* utils() { return _utils; }
    Locations* loc() const;
    PlayersDistribution* dist() { return _dist; }
private:
    // Implemented by children
    virtual void run(int numPlayers) = 0;
    virtual void configure(int numPlayers) = 0;
    virtual int maxNumPlayer() = 0;

    // Configure info
    bool _configureEnabled;
    int _lastNumPlayers;
    void updatePlayersRoles();

    // Behaviours garbage collector
    QList<Role*> _oldRoles;
    void setCurrRolesToOld();

    // Game info
    MRCTeam *_ourTeam;
    MRCTeam *_opTeam;
    CoachUtils *_utils;
    SSLReferee *_ref;

    // Players in this playbook list
    PlayersDistribution *_dist;
    QList<quint8> _players;
    QHash<quint8,Role*> _assignmentTable;
    QList<Role*> _rolesList;

    // Playbook initialized
    bool _initialized;
};

#endif // PLAYBOOK_H
