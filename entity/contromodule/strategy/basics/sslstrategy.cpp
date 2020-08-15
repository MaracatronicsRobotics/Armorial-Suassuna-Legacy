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

void SSLStrategy::setStrategyState(AgressivityLevel gameState, StrategyState *strategyState) {
    Strategy::setStrategyState(gameState, strategyState);
}

void SSLStrategy::runStrategy(AgressivityLevel agressivity) {
    SSLGameInfo::RefProcessedState gameState = getGameState();

    Strategy::runStrategy(agressivity, gameState);
}

QString SSLStrategy::state2str(AgressivityLevel gameState) {
    switch(gameState) {
        case HIGH_DEFENSE:   return "HIGH_DEFENSE";
        case MEDIUM_DEFENSE: return "MEDIUM_DEFENSE";
        case EQUILIBRATED:   return "EQUILIBRATED";
        case MEDIUM_ATTACK:  return "MEDIUM_ATTACK";
        case HIGH_ATTACK:    return "HIGH_ATTACK";
        default:
            return "UNDEFINED";
    }
}
