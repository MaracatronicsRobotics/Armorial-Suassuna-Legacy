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

#include "role_defender.h"

QString Role_Defender::name(){
    return "Role_Defender";
}

Role_Defender::Role_Defender() {
    _bh_bar = NULL;
}

void Role_Defender::initializeBehaviours(){
    usesBehaviour(BEHAVIOUR_BARRIER, _bh_bar = new Behaviour_Barrier());
}

void Role_Defender::configure(){
    _barrierId = 0;
}

void Role_Defender::run(){
    _bh_bar->setBarrierId(_barrierId);
    _bh_bar->setD(0.0);
    setBehaviour(BEHAVIOUR_BARRIER);
}
