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

#ifndef PLAYERSDISTRIBUTION_H
#define PLAYERSDISTRIBUTION_H

#include <entity/contromodule/basecoach.h>
#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <utils/wrtimer/wrtimer.hh>

#define DIST_INVALID_ID 200

class PlayersDistribution
{
public:
    PlayersDistribution(MRCTeam *team, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref);
    // Print
    void print();

    // Info
    int playersAvailable() const;
    bool hasPlayersAvailable() const;

    // Results
    quint8 getPlayer();
    quint8 getOneKNN(const Position &pos);
    QList<quint8> getPlayers(int n);
    QList<quint8> getAllPlayers();
    QList<quint8> getKNN(int k, const Position &pos);
    QList<quint8> getKNNandY(int k, const Position &pos);
    QList<quint8> getAllKNN(const Position &pos);
    QList<quint8> getAllY();
    QList<quint8> getAllKNNandY(const Position &pos);
    quint8 getGK();
    quint8 getRefGK();
    quint8 getKicker(const Position &posBall);
    void removePlayer(quint8 id);

private:
    // Friend classes (for inner access)
    friend class Playbook;
    friend class StrategyState;

    // Modifiers (accessed by friend classes)
    void insert(quint8 id);
    void remove(quint8 id);
    void clear();

    // playerInfo definition
    typedef struct {
        quint8 id;
        float distance;
    } playerInfo;

    // Team info
    MRCTeam *_team;

    // Players in this distribution info
    QList<playerInfo> _players;

    // Game state info
    SSLReferee *_ref;

    // GK info
    quint8 _gkId;
    quint8 selectGK();

    // Kicker info
    qint8 *_kickerId;
    SSLGameInfo::RefProcessedState *_lastState;
    WRTimer _kickerTimer;

    // Internal
    bool contains(quint8 id) const;

    // Sorts
    void updateDistancesTo(const Position &pos);
    void sortByDistanceTo(const Position &pos);
    void updateDistancesToY();
    void sortByY(int numToSort = 0);

    // Quick-sort
    float medianValue(float a, float b, float c);
    void quickSort(int ini, int end);
};

#endif // PLAYERSDISTRIBUTION_H
