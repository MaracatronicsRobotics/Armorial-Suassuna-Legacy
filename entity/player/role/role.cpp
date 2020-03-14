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

}

Role::~Role() {
    // Delete behaviours
    QHash<int, Behaviour*>:: iterator it;
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
