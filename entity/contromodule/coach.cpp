#include "coach.h"

#include <entity/contromodule/mrcteam.h>
#include <entity/player/playerbus.h>
#include <entity/contromodule/strategy/strategy.h>
#include <entity/contromodule/coachutils.h>

#include <entity/coachview/mainwindow.h>

QString Coach::name(){
    std::cout << "Coach" << std::endl;
}

Coach::Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam, CoachView *ourGUI)
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

    // suassuna ui
    _ourGUI = ourGUI;

    // debug
    _updateRoles = true;
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
        }
        strat->runStrategy();
        if(_updateRoles){
            for(int x = 0; x < _ourTeam->avPlayersSize(); x++){
                _ourGUI->getUI()->setPlayerRole((int) _ourTeam->avPlayers().values().at(x)->playerId(), _ourTeam->avPlayers().values().at(x)->getRoleName());
            }
            _updateRoles = false;
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
    _updateRoles = true;

    _mutexStrategy.unlock();
}

Strategy* Coach::strategy(){
    _mutexStrategy.lock();
    Strategy *s = _strat;
    _mutexStrategy.unlock();

    return s;
}
