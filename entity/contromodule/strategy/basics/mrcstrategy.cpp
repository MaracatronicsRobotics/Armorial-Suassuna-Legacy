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

#include "mrcstrategy.h"
#include <entity/contromodule/strategy/mrcstrategies.h>

QString MRCStrategy::name() {
    return "MRCStrategy_2020";
}

MRCStrategy::MRCStrategy() {

}

void MRCStrategy::configure() {
    setStrategyState(HIGH_DEFENSE, new SSLStrategy_Halt());
    setStrategyState(MEDIUM_DEFENSE, new SSLStrategy_Halt());
    setStrategyState(EQUILIBRATED, new SSLStrategy_Halt());
    setStrategyState(MEDIUM_ATTACK, new SSLStrategy_Halt());
    setStrategyState(HIGH_ATTACK, new SSLStrategy_Halt());
    setStrategyState(AgressivityLevel(10), new SSLStrategy_Test());
}
