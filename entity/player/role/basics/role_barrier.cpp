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

#include "role_barrier.h"

QString Role_Barrier::name(){
    return "Role_Barrier";
}

Role_Barrier::Role_Barrier() {
}

void Role_Barrier::initializeBehaviours(){
    // Aqui são inseridos os behaviours possíveis de serem usados
    // na ordem: ID do behaviour, instanciação dele
    usesBehaviour(BHV_BARRIER, _bh_bar = new Behaviour_Barrier());
}

void Role_Barrier::configure(){
    // Aqui são setados parametros que devem ser configurados
}

void Role_Barrier::run(){
    /*
     * Aqui devem ocorrer os sets de parametros de acordo com o behaviour
     * que estiver sendo executado, de preferencia declare todos os parametros
     * na classe da role, sete-os aqui e envie para o behaviour (usando as funções
     * set presentes neles)
    */

    if(player()->playerId() == 0) _bh_bar->setBarrierId(0);
    else if(player()->playerId() == 2) _bh_bar->setBarrierId(1);
    else if(player()->playerId() == 4) _bh_bar->setBarrierId(2);

    setBehaviour(BHV_BARRIER);

}
