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

#include "strategy.h"
#include <entity/contromodule/playersdistribution.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/strategy/strategystate.h>

Strategy::Strategy()
{
    _kickerId = -1;
    _lastState = SSLGameInfo::STATE_UNDEFINED;
    _initialized = false;
    _dist = NULL;
}

Strategy::~Strategy(){
    // Load StrategyStates
    QList<StrategyState*> gameStates = _strategyStatesTable.values();

    // Delete one by one
    QList<StrategyState*>::const_iterator it;
    for(it=gameStates.constBegin(); it!=gameStates.constEnd(); it++)
        delete *it;

    // Clear for convenience
    _strategyStatesTable.clear();

    if(_dist!=NULL)
        delete _dist;
}

void Strategy::initialize(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam, CoachUtils *utils){
    _ref = ref;
    _ourTeam = ourTeam;
    _theirTeam = theirTeam;
    _utils = utils;

    _dist = new PlayersDistribution(_ourTeam, &_kickerId, &_lastState, _ref);

    // Configure strategy states
    configure();

    _initialized = true;

}

SSLGameInfo::RefProcessedState Strategy::getGameState(){
    return _ref->getGameInfo(_ourTeam->teamColor())->processedState();
}

void Strategy::runStrategy(int gameState, SSLGameInfo::RefProcessedState refState) {
    // Get current StrategyState and run
    StrategyState *strategyState = getStrategyState(gameState);
    if(strategyState!=NULL) {
        if(strategyState->isInitialized()==false)
            strategyState->initialize(_ourTeam, _theirTeam, _utils, _dist, &_kickerId, &_lastState, _ref);
        strategyState->runStrategyState();
    }
}

void Strategy::setStrategyState(int gameState, StrategyState *strategyState) {
    // Check pointer
    if(strategyState==NULL)
        return;
    // Add to game states table
    if(_strategyStatesTable.contains(gameState))
        delete _strategyStatesTable.value(gameState);
    _strategyStatesTable.insert(gameState, strategyState);
}

StrategyState* Strategy::getStrategyState(int gameState) {
    // Check if child strategy set that game state
    if(_strategyStatesTable.contains(gameState)==false) {
        std::cout << "[ERROR] Strategy '" << name().toStdString() << "' has not set state " << state2str(gameState).toStdString() << ".\n";
        return NULL;
    }
    // Return StrategyState
    return _strategyStatesTable.value(gameState);
}
