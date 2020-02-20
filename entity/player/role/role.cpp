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
    _configureEnabled = false;
}

Role::~Role() {
    // Delete behaviours
    QList<Behaviour*>:: iterator it;
    for(it = _behaviourList.begin(); it != _behaviourList.end(); it++){
        delete *it;
    }
    _behaviourList.clear();
}

Locations* Role::loc() {
    return _ourTeam->loc();
}

void Role::initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, Locations *loc, SSLReferee *ref){
    _ourTeam = ourTeam;
    _theirTeam = theirTeam;
    _loc = loc;
    _ref = ref;

    // Configure Role
    _configureEnabled = true;
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

    // Run role (child)
    run();

    // Run Behaviour
    if(_behaviour->isInitialized() == false){
        _behaviour->initialize(_loc);
    }
    _behaviour->setPlayer(_player, _playerAccess);
    _behaviour->runBehaviour();
}

bool Role::canKickBall() const {
    return _ref->getGameInfo(_ourTeam->teamColor())->canKickBall();
}

void Role::usesBehaviour(Behaviour *behaviour){
    if(_configureEnabled == false){
        std::cout << "[WARNING] Blocked '" << name().toStdString() << "' setting Behaviour to use outside configure().\n";
        return ;
    }

    if(_behaviourList.contains(behaviour) == false){
        // Add to list
        _behaviourList.push_back(behaviour);

        // If don't have initial skill, put it as initial skill
        if(_behaviour == NULL){
            _behaviour = behaviour;
        }
    }
}

void Role::setBehaviour(Behaviour *behaviour){
    if(_behaviourList.contains(behaviour)){
        _behaviour = behaviour;
    }else{
        std::cout << "[ERROR] " << name().toStdString() << " setting behaviour '" << behaviour->name().toStdString() << "' that isn't at behaviours list.\n";
    }
}

PlayerAccess* Role::player(){
    if(_playerAccess == NULL){
        std::cout << "[ERROR] " << name().toStdString() << ", requesting player(), playerAccess not set!\n";
    }
    return _playerAccess;
}
