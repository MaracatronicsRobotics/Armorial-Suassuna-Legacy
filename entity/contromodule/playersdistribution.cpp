#include "playersdistribution.h"
#include <utils/utils.hh>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/player/player.h>
#include <entity/locations.h>

PlayersDistribution::PlayersDistribution(MRCTeam *team, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref) {
    _team = team;
    _kickerId = kickerId;
    _lastState = lastState;
    _ref = ref;
    _gkId = DIST_INVALID_ID;
}

quint8 PlayersDistribution::getPlayer() {
    return getOneKNN(Position(false, 0, 0, 0));
}

quint8 PlayersDistribution::getOneKNN(const Position &pos) {
    return _players.empty()? DIST_INVALID_ID : getKNN(1, pos).first();
}

QList<quint8> PlayersDistribution::getPlayers(int n) {
    // Empty
    if(_players.empty())
        return QList<quint8>();
    // Create list of random players
    QList<quint8> retn;
    int size = _players.size();
    for(int i=0; i<(n>size? size:n); i++)
        retn.push_back(getPlayer());
    return retn;
}

QList<quint8> PlayersDistribution::getAllPlayers() {
    // Empty
    if(_players.empty())
        return QList<quint8>();
    // Return all players
    QList<quint8> retn;
    int size = _players.size();
    for(int i=0; i<size; i++)
        retn.push_back(getPlayer());
    return retn;
}

QList<quint8> PlayersDistribution::getKNN(int k, const Position &pos) {
    // Empty
    if(k==0 ||_players.empty())
        return QList<quint8>();
    // Sort
    sortByDistanceTo(pos);
    // Return 'k' players
    int available = _players.size();
    QList<quint8> retn;
    for(int i=0; i<(k>available?available:k); i++)
        retn.push_back(_players.takeFirst().id);
    return retn;
}

QList<quint8> PlayersDistribution::getKNNandY(int k, const Position &pos) {
    // Empty
    if(k==0 ||_players.empty())
        return QList<quint8>();
    // Sort
    sortByDistanceTo(pos);
    sortByY(k);
    // Return 'k' players
    int available = _players.size();
    QList<quint8> retn;
    for(int i=0; i<(k>available?available:k); i++)
        retn.push_back(_players.takeFirst().id);
    return retn;
}

QList<quint8> PlayersDistribution::getAllKNN(const Position &pos) {
    return getKNN(_players.size(), pos);
}

QList<quint8> PlayersDistribution::getAllY() {
    return getKNNandY(_players.size(), Position(false, 0.0, 0.0, 0.0));
}

QList<quint8> PlayersDistribution::getAllKNNandY(const Position &pos) {
    return getKNNandY(_players.size(), pos);
}

quint8 PlayersDistribution::getGK() {
    if(_players.empty())
        return DIST_INVALID_ID;
    quint8 gk;
    // Select first GK
    if(_gkId == DIST_INVALID_ID)
        _gkId = selectGK();
    // Check game state; dont not change GK in HALT/GAMEOFF
    SSLGameInfo::RefProcessedState currState = _ref->getGameInfo(_team->teamColor())->processedState();
    if(currState==SSLGameInfo::STATE_CANTMOVE || currState==SSLGameInfo::STATE_GAMEOFF) {
        // If GK is on field, maintain
        if(contains(_gkId))
            gk = _gkId;
        else { // else, select temporary GK
            gk = selectGK();
        }
    } else {
        // Check if GK is set; if not, set it
        if(contains(_gkId)==false)
            _gkId = selectGK();
        // Keep same GK when game is running
        gk = _gkId;
    }
    // Returns GK
    remove(gk);
    return gk;
}

quint8 PlayersDistribution::getRefGK() {
    quint8 goalie = _ref->getGameInfo(_team->teamColor())->getGoalie();
    if(contains(goalie)) {
        remove(goalie);
        return goalie;
    } else {
        return DIST_INVALID_ID;
    }
}

quint8 PlayersDistribution::selectGK() {
    Position posGoal = _team->loc()->ourGoal();
    // Sort players list
    sortByDistanceTo(posGoal);
    // Return nearest
    return _players.isEmpty()? DIST_INVALID_ID : _players.first().id;
}

quint8 PlayersDistribution::getKicker(const Position &posBall) {
    if(_players.empty())
        return DIST_INVALID_ID;
    // Sort
    sortByDistanceTo(posBall);
    // Check state
    SSLGameInfo::RefProcessedState newState = _ref->getGameInfo(_team->teamColor())->processedState();
    if(newState==SSLGameInfo::STATE_GAMEOFF || newState==SSLGameInfo::STATE_CANTMOVE)
        *_kickerId = -1;
    if(*_lastState!=newState) { // state changed
        // Store state
        *_lastState = newState;
        // Stop kick state, store kicker and return
        if(newState==SSLGameInfo::STATE_OURDIRECTKICK || newState==SSLGameInfo::STATE_OURINDIRECTKICK || newState==SSLGameInfo::STATE_OURKICKOFF) {
            *_kickerId = _players.takeFirst().id;
            return *_kickerId;
        }
        return _players.takeFirst().id;
    } else {
        // If still in stop kick, keep kicker
        if(newState==SSLGameInfo::STATE_OURDIRECTKICK || newState==SSLGameInfo::STATE_OURINDIRECTKICK || newState==SSLGameInfo::STATE_OURKICKOFF) {
            // Timer to clean last kicker
            _kickerTimer.start();
            // Check if kicker still in this distribution; if not, get another
            if(this->contains(*_kickerId)==false) {
                *_kickerId = _players.takeFirst().id;
                return *_kickerId;
            }
            // Return kicker
            remove(*_kickerId);
            return *_kickerId;
        } else { // else, get anyone, but the last kicker choosed
            // Check timer
            _kickerTimer.stop();
            if(_kickerTimer.timesec() >= 5) {
                *_kickerId = -1;
                _kickerTimer.start();
            }
            // Return kicker
            quint8 possibleKicker = _players.first().id;
            if(possibleKicker!=*_kickerId) // different kicker, take it!
                return _players.takeFirst().id;
            else {                         // same kicker, take next
                if(_players.size() >= 2)
                    return _players.takeAt(1).id;
                else {
                    std::cout << "[WARNING] No players available in state to avoid double touch!\n";
                    return _players.takeFirst().id;
                }
            }
        }
    }
}

void PlayersDistribution::removePlayer(quint8 id) {
    remove(id);
}

bool PlayersDistribution::contains(quint8 id) const {
    for(int i=0; i<_players.size(); i++) {
        if(_players.at(i).id==id)
            return true;
    }
    return false;
}

int PlayersDistribution::playersAvailable() const {
    return _players.size();
}

bool PlayersDistribution::hasPlayersAvailable() const {
    return (_players.empty()==false);
}

void PlayersDistribution::clear() {
    _players.clear();
}

void PlayersDistribution::insert(quint8 id) {
    playerInfo ins;
    ins.id = id;
    _players.push_back(ins);
}

void PlayersDistribution::remove(quint8 id) {
    QList<playerInfo>::iterator it;
    for(it=_players.begin(); it!=_players.end(); it++) {
        playerInfo info = *it;
        if(info.id==id) {
            _players.erase(it);
            break;
        }
    }
}

void PlayersDistribution::print() {
    std::cout << "Players distribution:";
    for(QList<playerInfo>::iterator it=_players.begin(); it!=_players.end(); it++)
        std::cout << " " << (int)(*it).id;
    std::cout << "\n";
}

void PlayersDistribution::updateDistancesTo(const Position &pos) {
    if(pos.isUnknown())
        return;
    // Update distances
    for(int i=0; i<_players.size(); i++) {
        playerInfo player = _players.at(i);
        Position playerPos;
        if(player.id!=DIST_INVALID_ID)
            playerPos = _team->avPlayers().value(player.id)->position();
        if(playerPos.isUnknown() || player.id==DIST_INVALID_ID)
            player.distance = 999;
        else
            player.distance = WR::Utils::distance(pos, playerPos);
        _players.replace(i, player);
    }
}

void PlayersDistribution::updateDistancesToY() {
    for(int i=0; i<_players.size(); i++) {
        playerInfo player = _players.at(i);
        Position playerPos;
        if(player.id!=DIST_INVALID_ID)
            playerPos = _team->avPlayers().value(player.id)->position();
        if(playerPos.isUnknown() || player.id==DIST_INVALID_ID)
            player.distance = 999;
        else
            player.distance = playerPos.y();
        _players.replace(i, player);
    }
}

void PlayersDistribution::sortByDistanceTo(const Position &pos) {
    const int N = _players.size();
    // Check players list
    if(N==0)
        return;
    // Update distances
    updateDistancesTo(pos);
    // Quick-sort
    quickSort(0, N-1);
}

void PlayersDistribution::sortByY(int numToSort) {
    const int N = _players.size();
    // Check players list
    if(N==0)
        return;
    // Sort only 'numToSort' players
    int NTosort = (numToSort==0)? N : numToSort;
    if(NTosort>N)
        NTosort = N;
    // Update distances to Y
    updateDistancesToY();
    // Quick-sort
    quickSort(0, NTosort-1);
}

float PlayersDistribution::medianValue(float a, float b, float c) {
    if ((a <= b && b <= c) || (c <= b && b <= a))
            return b;
    if ((b <= a && a <= c) || (c <= a && a <= b))
            return a;
    if ((a <= c && c <= b) || (b <= c && c <= a))
            return c;
    else
        return -1;
}

void PlayersDistribution::quickSort(int ini, int end) {
    int i = ini, j = end, mid;
    float pivo;
    mid = ini + (end - ini)/2;
    pivo = medianValue(_players.at(i).distance, _players.at(j).distance, _players.at(mid).distance);
    while (i < j) {
        while (_players.at(i).distance < pivo && i <= end)
            i++;
        while (_players.at(j).distance > pivo && j >= 0)
            j--;
        if (i <= j) {
            _players.swap(i, j);
            i++;
            j--;
        }
    }
    if (j > ini)
        quickSort (ini, j);
    if (i < end)
        quickSort (i, end);
}
