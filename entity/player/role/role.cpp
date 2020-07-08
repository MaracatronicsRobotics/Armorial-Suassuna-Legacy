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

#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/playersdistribution.h>
#include <entity/player/behaviour/behaviour.h>

#include "role.h"

Role::Role() {
    _behaviour = NULL;
    _player = NULL;
    _playerAccess = NULL;
    _loc = NULL;
    _initialized = false;
    _configureEnabled = true; // for set behaviours
    _actualBehaviour = -1;
}

Role::~Role() {
    // Delete behaviours
    QHash<int, Behaviour*>:: iterator it;
    for(it = _behaviourList.begin(); it != _behaviourList.end(); it++){
        if((*it) == NULL)
            continue;
        else
            delete *it;
    }
    _behaviourList.clear();
}

Locations* Role::loc() {
    return _ourTeam->loc();
}

SSLReferee* Role::ref(){
    return _ref;
}

void Role::initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, Locations *loc, SSLReferee *ref){
    _ourTeam = ourTeam;
    _theirTeam = theirTeam;
    _loc = loc;
    _ref = ref;

    // Configure Role
    _configureEnabled = true;
    _bh_dn = new Behaviour_DoNothing();
    _bh_tm = new Behaviour_TimeOut();
    initializeBehaviours();
    configure();
    _configureEnabled = false;

    // Initialize
    _initialized = true;
}

void Role::setPlayer(Player *player, PlayerAccess *playerAccess){
    _player = player;
    _playerAccess = playerAccess;
}

void Role::runRole(){
    if(_behaviourList.size() == 0){
        std::cout << "[ERROR] " << name().toStdString() << " has no behaviours set!\n";
        return ;
    }

    // Disabling dribble when occur an stop
    if(!player()->canKickBall())
        player()->dribble(false);

    if(!_ref->getGameInfo(player()->team()->teamColor())->canMove()){
        // If is in halt
        if(_bh_dn->isInitialized() == false)
            _bh_dn->initialize(_loc);

        // Configure
        _bh_dn->setPlayer(_player, _playerAccess);
        _bh_dn->runBehaviour();
    }
    else if(_ref->getGameInfo(player()->team()->teamColor())->timeOut()){
        // If is in timeout
        if(_bh_tm->isInitialized() == false)
            _bh_tm->initialize(_loc);

        // Configure
        _bh_tm->setPlayer(_player, _playerAccess);
        _bh_tm->getPlayerTimeOutId();
        _bh_tm->runBehaviour();
    }
    else{
        // reset timeout
        _bh_tm->reset();

        // Run role (child)
        run();

        // Run Behaviour
        if(_behaviour->isInitialized() == false){
            _behaviour->initialize(_loc);
        }
        _behaviour->setPlayer(_player, _playerAccess);
        _behaviour->runBehaviour();
    }
}

bool Role::canKickBall() const {
    return _ref->getGameInfo(_ourTeam->teamColor())->canKickBall();
}

QHash<int, Behaviour*> Role::getBehaviours(){
    return _behaviourList;
}

void Role::usesBehaviour(int id, Behaviour *behaviour){
    if(_configureEnabled == false){
        std::cout << "[WARNING] Blocked '" << name().toStdString() << "' setting Behaviour to use outside configure().\n";
        return ;
    }

    if(_behaviourList.contains(id) == false){
        // Add to list
        _behaviourList.insertMulti(id, behaviour);

        // If don't have initial behaviour, put it as initial behaviour
        if(_behaviour == NULL){
            _behaviour = behaviour;
            _actualBehaviour = id;
        }
    }else{
        std::cout << "[WARNING] Blocked '" << name().toStdString() << "' adding behaviours with same id!\n";
    }

}

int Role::getActualBehaviour(){
    return _actualBehaviour;
}

void Role::setBehaviour(int behaviour_id){
    if(_behaviourList.contains(behaviour_id)){
        _behaviour = _behaviourList.value(behaviour_id);
        _actualBehaviour = behaviour_id;
    }else{
        std::cout << "[ERROR] " << name().toStdString() << " setting behaviour with id '" << behaviour_id << "' that isn't at behaviours list.\n";
    }
}

PlayerAccess* Role::player(){
    if(_playerAccess == NULL){
        std::cout << "[ERROR] " << name().toStdString() << ", requesting player(), playerAccess not set!\n";
    }
    return _playerAccess;
}
