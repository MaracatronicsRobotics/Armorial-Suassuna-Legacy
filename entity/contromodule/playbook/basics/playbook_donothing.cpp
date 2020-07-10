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

#include "playbook_donothing.h"

QString Playbook_DoNothing::name() {
    return "Playbook_DoNothing";
}

Playbook_DoNothing::Playbook_DoNothing() {
}

int Playbook_DoNothing::maxNumPlayer() {
    return MRCConstants::_qtPlayers;
}

void Playbook_DoNothing::configure(int numPlayers) {
    _rl_gk = new Role_Goalkeeper();
    usesRole(_rl_gk);

    for(int i = 0; i < 3; i++){
        Role_Defender *rl_def = new Role_Defender();
        usesRole(rl_def);
        _rl_def.push_back(rl_def);
    }

    _rl_def_midf = new Role_Defensive_Midfielder();
    usesRole(_rl_def_midf);

    _rl_default = new Role_Default();
    usesRole(_rl_default);

    _rl_default2 = new Role_Default();
    usesRole(_rl_default2);
}

void Playbook_DoNothing::run(int numPlayers) {
    quint8 playerId = 1;
    dist()->removePlayer(playerId);
    setPlayerRole(playerId, _rl_default);
/*
    playerId = 5;
    dist()->removePlayer(playerId);
    setPlayerRole(playerId, _rl_default2);
*/
    playerId = dist()->getPlayer();
    if(playerId != DIST_INVALID_ID){
        setPlayerRole(playerId, _rl_gk);
    }
/*
    playerId = dist()->getPlayer();
    if(playerId != DIST_INVALID_ID){
        setPlayerRole(playerId, _rl_def_midf);
    }
*/

    int placedBarriers = 0;
    for(int i = 0; i < 3; i++){
        quint8 playerId = dist()->getPlayer();
        if(playerId != DIST_INVALID_ID){
            // place the first barrier in the "quadrant" of the ball
            if(placedBarriers == 0){
                if(loc()->ball().y() < 0.0) _rl_def.at(i)->setBarrierId(1);
                else _rl_def.at(i)->setBarrierId(0);
                placedBarriers++;
            }else{
                if(loc()->ball().y() < 0.0) _rl_def.at(i)->setBarrierId(0);
                else _rl_def.at(i)->setBarrierId(1);
            }
            setPlayerRole(playerId, _rl_def.at(i));
            _rl_def.at(i)->setBarrierId(i);
        }
    }
}
