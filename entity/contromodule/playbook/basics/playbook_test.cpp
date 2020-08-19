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

#include "playbook_test.h"

QString Playbook_Test::name() {
    return "Playbook_Test";
}

Playbook_Test::Playbook_Test() {

}

int Playbook_Test::maxNumPlayer() {
    return getConstants()->getQtPlayers();
}

void Playbook_Test::configure(int numPlayers) {
    rl_test = new Role_Test();

    usesRole(rl_test);
}

void Playbook_Test::run(int numPlayers) {
    // Taking the test player
    quint8 playerId = dist()->getPlayer();
    if(playerId != DIST_INVALID_ID){
        setPlayerRole(playerId, rl_test);
    }

}
