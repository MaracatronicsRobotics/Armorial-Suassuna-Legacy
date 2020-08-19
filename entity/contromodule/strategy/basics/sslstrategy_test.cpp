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

#include "sslstrategy_test.h"
#include <entity/contromodule/strategy/strategystate.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_Test::name() {
    return "SSLStrategy_Test";
}

SSLStrategy_Test::SSLStrategy_Test() {
    _pb_test = NULL;
}

void SSLStrategy_Test::configure(int numOurPlayers) {
    usesPlaybook(_pb_test = new Playbook_Test());
}

void SSLStrategy_Test::run(int numOurPlayers) {
    _pb_test->addPlayers(dist()->getAllPlayers());
}
