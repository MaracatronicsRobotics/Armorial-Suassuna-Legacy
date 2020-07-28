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

#include "sslstrategy_halt.h"
#include <entity/contromodule/strategy/strategystate.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_Halt::name() {
    return "SSLStrategy_Halt";
}

SSLStrategy_Halt::SSLStrategy_Halt() {
    _pb_doNothing = NULL;
}

void SSLStrategy_Halt::configure(int numOurPlayers) {
    usesPlaybook(_pb_doNothing = new Playbook_DoNothing());
    usesPlaybook(_pb_attack = new Playbook_Attack());
}

void SSLStrategy_Halt::run(int numOurPlayers) {
    if(_pb_doNothing == NULL){
        std::cout << "oi bb" << std::endl;
        return;
    }
    for(int x = 0; x < numOurPlayers; x++){
        quint8 id = dist()->getPlayer();
        if(id != 1)
            _pb_doNothing->addPlayer(id);
        else
            _pb_attack->addPlayer(id);
    }
}
