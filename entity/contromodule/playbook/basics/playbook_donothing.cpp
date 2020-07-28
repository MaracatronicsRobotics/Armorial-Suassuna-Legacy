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
    return 5;
}

void Playbook_DoNothing::configure(int numPlayers) {
    _rl_gk = new Role_Goalkeeper();
    usesRole(_rl_gk);

    for(int i = 0; i < 2; i++){
        Role_Defender *rl_def = new Role_Defender();
        usesRole(rl_def);
        _rl_def.push_back(rl_def);
    }

    _rl_def_midf = new Role_Defensive_Midfielder();
    usesRole(_rl_def_midf);

    _rl_def_midf2 = new Role_Defensive_Midfielder();
    usesRole(_rl_def_midf2);
}

void Playbook_DoNothing::run(int numPlayers) {
    resetMarkList();

    // Taking the goalkeeper
    quint8 playerId = dist()->getGK();
    if(playerId != DIST_INVALID_ID){
        setPlayerRole(playerId, _rl_gk);
    }

    // Taking barriers (always 2)
    QList<quint8> barriers;
    int placedBarriers = 0;
    float minDist = 999.0f;
    quint8 id = DIST_INVALID_ID;
    for(int i = 0; i < 2; i++){
        quint8 playerId = dist()->getPlayer();
        if(playerId != DIST_INVALID_ID){
            // place the first barrier in the "quadrant" of the ball
            barriers.push_back(playerId);
            if(placedBarriers == 0){
                if(loc()->ball().y() < 0.0f) _rl_def.at(i)->setBarrierSide('r');
                else _rl_def.at(i)->setBarrierSide('r');
                placedBarriers++;
            }else{
                if(loc()->ball().y() < 0.0f) _rl_def.at(i)->setBarrierSide('l');
                else _rl_def.at(i)->setBarrierSide('l');
            }
            setPlayerRole(playerId, _rl_def.at(i));
            _rl_def.at(i)->setBarrierId(i);

            // save at id the closest barrier to ball
            if(PlayerBus::ourPlayerAvailable(playerId)){
                if(PlayerBus::ourPlayer(playerId)->distBall() < minDist){
                    minDist = PlayerBus::ourPlayer(playerId)->distBall();
                    id = playerId;
                }
            }
        }
    }

    // Enable takeout for the barrier that is closest to the ball
    // (avoid double takeout and dribble errors)
    for(int x = 0; x < barriers.size(); x++){
        if(id == barriers.at(x))
            _rl_def.at(x)->setBarrierCanTakeout(true);
        else
            _rl_def.at(x)->setBarrierCanTakeout(false);
    }

    // Taking the Defensive Midfielder 1
    playerId = dist()->getPlayer();
    if(playerId != DIST_INVALID_ID){
        quint8 markId = requestMarkPlayer(playerId);
        _rl_def_midf->setMarkId(markId);
        setPlayerRole(playerId, _rl_def_midf);
    }

    // Taking the Defensive Midfielder 2
    playerId = dist()->getPlayer();
    if(playerId != DIST_INVALID_ID){
        quint8 markId = requestMarkPlayer(playerId);
        _rl_def_midf2->setMarkId(markId);
        setPlayerRole(playerId, _rl_def_midf2);
    }

}

void Playbook_DoNothing::resetMarkList(){
    markList.clear();

    QList<Player*> list = loc()->getOpPlayers().values();
    QList<Player*>::iterator it;

    // Remove their defensive players
    for(it = list.begin(); it != list.end(); it++){
        if((*it)->distOurGoal() >= 2.0f){
            markList.push_back((*it)->playerId());
        }
    }

    // Remove their closest player to ball
    float minDist = 999.0f;
    int pos = DIST_INVALID_ID;

    for(int x = 0; x < markList.size(); x++){
        if(PlayerBus::theirPlayerAvailable(markList.at(x))){
            float dist = PlayerBus::theirPlayer(markList.at(x))->distBall();
            if(dist < minDist){
                minDist = dist;
                pos = x;
            }
        }
    }

    if(pos != DIST_INVALID_ID)
        markList.removeAt(pos);

    // Sort for priority (closest to our goal)
    for(int x = 0; x < markList.size(); x++){
        for(int y = x; y < markList.size(); y++){
            if(PlayerBus::theirPlayerAvailable(markList.at(y))){
                if(PlayerBus::theirPlayerAvailable(markList.at(x))){
                    if(PlayerBus::theirPlayer(markList.at(x))->distTheirGoal() > PlayerBus::theirPlayer(markList.at(y))->distTheirGoal()){
                        swap(markList[x], markList[y]);
                    }
                }
            }
        }
    }
}

quint8 Playbook_DoNothing::requestMarkPlayer(quint8 playerId){
    float minDist = 999.0f;
    quint8 markId = DIST_INVALID_ID;
    int pos = DIST_INVALID_ID;

    // always have 1 gk + 2 barriers, so the qt markers are defined by the actual minus 3
    int qtMarkers = std::max(0, getPlayers().size() - 3);
    qtMarkers = std::min(qtMarkers, markList.size());

    for(int x = 0; x < qtMarkers; x++){
        if(PlayerBus::theirPlayerAvailable(markList.at(x))){
            if(PlayerBus::ourPlayerAvailable(playerId)){
                float distance = PlayerBus::ourPlayer(playerId)->distanceTo(PlayerBus::theirPlayer(markList.at(x))->position());
                if(distance < minDist){
                    minDist = distance;
                    markId = markList.at(x);
                    pos = x;
                }
            }
        }
    }

    if(pos != DIST_INVALID_ID){
        markList.removeAt(pos);
    }

    return markId;
}
