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

#include "role_default.h"

QString Role_Default::name(){
    return "Role_Default";
}

Role_Default::Role_Default() {
}

void Role_Default::initializeBehaviours(){
    // Aqui são inseridos os behaviours possíveis de serem usados
    // na ordem: ID do behaviour, instanciação dele

    usesBehaviour(BHV_DONOTHING, _bh_dn = new Behaviour_Attacker());
    usesBehaviour(1, _bh_gk = new Behaviour_Goalkeeper());
    usesBehaviour(2, _bh_bar = new Behaviour_Receiver());
    usesBehaviour(3, _bh_dn2 = new Behaviour_Attacker());
    usesBehaviour(4, _bh_dn3 = new Behaviour_DoNothing());

}

void Role_Default::configure(){
    // Aqui são setados parametros que devem ser configurados
}

void Role_Default::run(){
    /*
     * Aqui devem ocorrer os sets de parametros de acordo com o behaviour
     * que estiver sendo executado, de preferencia declare todos os parametros
     * na classe da role, sete-os aqui e envie para o behaviour (usando as funções
     * set presentes neles)
    */

    //_bh_dn->addReceiver(5);
    if(player()->playerId() == 1){
        if(player()->distBall() <= 0.5f)
            setBehaviour(BHV_DONOTHING);
        else{
            _bh_bar->setAttackerId(5);
            setBehaviour(2);
        }
    }else{
        if(player()->distBall() <= 0.5f)
            setBehaviour(BHV_DONOTHING);
        else{
            _bh_bar->setAttackerId(1);
            setBehaviour(2);
        }
    }

}
