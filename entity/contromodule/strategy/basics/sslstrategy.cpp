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

#include "sslstrategy.h"

SSLStrategy::SSLStrategy() {

}

SSLStrategy::~SSLStrategy() {

}

void SSLStrategy::setStrategyState(SSLGameState gameState, StrategyState *strategyState) {
    Strategy::setStrategyState(gameState, strategyState);
}

void SSLStrategy::runStrategy() {
    SSLGameInfo::RefProcessedState refState = getGameState();

    // Convert SSLGameInfo state to SSLGameState
    SSLGameState gameState = refState2SSLGameState(refState);
    if(gameState==UNDEFINED)
        return;

    //Strategy::runStrategy(gameState, refState);
    Strategy::runStrategy(HALT, SSLGameInfo::RefProcessedState::STATE_CANTMOVE);

}

SSLStrategy::SSLGameState SSLStrategy::refState2SSLGameState(SSLGameInfo::RefProcessedState refState) const {
    switch(refState) {
        case SSLGameInfo::STATE_CANTMOVE:           return HALT;
        case SSLGameInfo::STATE_GAMEON:             return GAMEON;
        case SSLGameInfo::STATE_GAMEOFF:            return GAMEOFF;
        case SSLGameInfo::STATE_OURDIRECTKICK:      return OURDIRECTKICK;
        case SSLGameInfo::STATE_OURINDIRECTKICK:    return OURINDIRECTKICK;
        case SSLGameInfo::STATE_OURKICKOFF:         return OURKICKOFF;
        case SSLGameInfo::STATE_OURPENALTY:         return OURPENALTY;
        case SSLGameInfo::STATE_THEIRDIRECTKICK:    return THEIRDIRECTKICK;
        case SSLGameInfo::STATE_THEIRINDIRECTKICK:  return THEIRINDIRECTKICK;
        case SSLGameInfo::STATE_THEIRKICKOFF:       return THEIRKICKOFF;
        case SSLGameInfo::STATE_THEIRPENALTY:       return THEIRPENALTY;
        case SSLGameInfo::STATE_TIMEOUT:            return TIMEOUT;

        case SSLGameInfo::STATE_UNDEFINED:
        default:
            std::cout << "[ERROR] SSLStrategy: undefined state, should never reach here!\n";
            return UNDEFINED;
    }
}

QString SSLStrategy::state2str(int gameState) {
    switch(gameState) {
        case HALT:              return "HALT";
        case GAMEON:            return "GAME ON";
        case GAMEOFF:           return "GAME OFF";
        case OURDIRECTKICK:     return "OUR DIRECT KICK";
        case OURINDIRECTKICK:   return "OUR INDIRECT KICK";
        case OURKICKOFF:        return "OUR KICKOFF";
        case OURPENALTY:        return "OUR PENALTY";
        case THEIRDIRECTKICK:   return "THEIR DIRECT KICK";
        case THEIRINDIRECTKICK: return "THEIR INDIRECT KICK";
        case THEIRKICKOFF:      return "THEIR KICKOFF";
        case THEIRPENALTY:      return "THEIR PENALTY";
        case UNDEFINED:
        default:
            return "UNDEFINED";
    }
}
