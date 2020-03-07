#include "mrcteam.h"
#include <entity/locations.h>

MRCTeam::MRCTeam(quint8 teamId, WorldMap *wm){
    _teamId = teamId;
    _wm = wm;

    _loc = new Locations(this);
}

MRCTeam::~MRCTeam(){
    delete _loc;
}

Colors::Color MRCTeam::teamColor() const{
    QMutexLocker locker(&_mutexTeamColor);
    return _teamColor;
}

Position MRCTeam::ballPosition(){
    return loc()->ball();
}

FieldSide MRCTeam::fieldSide() const{
    QMutexLocker locker(&_mutexFieldSide);
    return _fieldSide;
}

void MRCTeam::setTeamColor(Colors::Color color){
    QMutexLocker locker(&_mutexTeamColor);
    _teamId = _teamColor = color;
}

void MRCTeam::setFieldSide(FieldSide fieldSide){
    QMutexLocker locker(&_mutexFieldSide);
    _fieldSide = fieldSide;
}

bool MRCTeam::hasBallPossession(){
    // iterando nos jogadores
    const QList<Player*> players = _players.values();
    QList<Player*>::const_iterator it;
    for(it = players.constBegin(); it != players.end(); it++){
        const Player* player = *it;
        if(player->hasBallPossession()){
            return true;
        }
    }
    return false;
}

quint8 MRCTeam::ballPossession(){
    // iterando nos jogadores
    const QList<Player*> players = _players.values();
    QList<Player*>::const_iterator it;
    for(it = players.constBegin(); it != players.end(); it++){
        const Player* player = *it;
        if(player->hasBallPossession()){
            return player->playerId();
        }
    }
    return BALLPOSS_NONE;
}

void MRCTeam::addPlayer(Player *player) {
    _players.insert(player->playerId(), player);
}

int MRCTeam::avPlayersSize() {
    _muxPlayers.lock();
    int result = _avPlayers.size();
    _muxPlayers.unlock();
    return result;
}

QHash<quint8,Player*> MRCTeam::avPlayers() {
    _muxPlayers.lock();
    QHash<quint8,Player*>result(_avPlayers);
    _muxPlayers.unlock();
    return result;
}

void MRCTeam::updateAvailablePlayers() {
    _muxPlayers.lock();
    // Clear hash
    _avPlayers.clear();
    // Update players
    QHash<quint8,Player*>::iterator it;
    for(it=_players.begin(); it!=_players.end(); it++) {
        quint8 id = it.key();
        Player *player = it.value();
        // If position is known, player is available
        if(player->position().isUnknown()==false)
            _avPlayers.insert(id, player);
    }
    _muxPlayers.unlock();
}
