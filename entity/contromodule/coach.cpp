#include "coach.h"

#include <entity/contromodule/mrcteam.h>
#include <entity/player/playerbus.h>
#include <entity/contromodule/strategy/strategy.h>
#include <entity/contromodule/coachutils.h>

QString Coach::name(){
    std::cout << "Coach" << std::endl;
}

Coach::Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam)
{
    _ref = ref;
    _ourTeam = ourTeam;
    _theirTeam = theirTeam;

    // Initialize PlayerBus
    PlayerBus::initialize(ourTeam, theirTeam);

    // Coach utils
    _utils = new CoachUtils(ourTeam);

    // null strat
    _strat = NULL;
}

Coach::~Coach(){
    if(_strat != NULL)
        delete _strat;

    delete _utils;
}

void Coach::run(){
    if(_ourTeam->avPlayersSize() == 0){
        std::cout << "[COACH] No players available!" << std::endl;
        return ;
    }

    // get strategy
    Strategy *strat = strategy();

    // run strategy
    if(strat != NULL){
        if(strat->isInitialized() == false){
            strat->initialize(_ref, _ourTeam, _theirTeam, _utils);
            // debug behaviours
            for(int x = 0; x < _ourTeam->avPlayersSize(); x++){
                std::cout << "Player " << (int) _ourTeam->avPlayers().values().at(x)->playerId() << " has behaviour " << _ourTeam->avPlayers().values().at(x)->getBehaviourName().toStdString() << std::endl;
            }

            strat->runStrategy();
        }
    }

}

void Coach::setStrategy(Strategy *strat){
    _mutexStrategy.lock();

    // deleting old strategy
    if(_strat != NULL){
        delete _strat;
    }

    //setting new
    _strat = strat;

    _mutexStrategy.unlock();
}

Strategy* Coach::strategy(){
    _mutexStrategy.lock();
    Strategy *s = _strat;
    _mutexStrategy.unlock();

    return s;
}
