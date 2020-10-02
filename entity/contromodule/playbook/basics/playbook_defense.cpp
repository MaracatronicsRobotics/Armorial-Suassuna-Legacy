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

#include "playbook_defense.h"

QString Playbook_Defense::name() {
    return "Playbook_Defense";
}

Playbook_Defense::Playbook_Defense() {
    _goalieId = DIST_INVALID_ID;
}

int Playbook_Defense::maxNumPlayer() {
    return 4;
}

void Playbook_Defense::configure(int numPlayers) {
    _rl_gk = new Role_Goalkeeper();
    usesRole(_rl_gk);

    for(int i = 0; i < 2; i++){
        Role_Defender *rl_def = new Role_Defender();
        usesRole(rl_def);
        _rl_def.push_back(rl_def);
    }

    _rl_def_midf = new Role_Defensive_Midfielder();
    usesRole(_rl_def_midf);
}

void Playbook_Defense::run(int numPlayers) {
    resetMarkList();

    // Taking the goalkeeper
    if(_goalieId != DIST_INVALID_ID){
        dist()->removePlayer(_goalieId);
        setPlayerRole(_goalieId, _rl_gk);
    }

    if(numPlayers >= 4){
        // Taking the Defensive Midfielder
        QList<quint8> allies = getAllPlayers();
        for(int x = 0; x < allies.size(); x++){
            if(allies.at(x) == _goalieId){
                allies.removeAt(x);
            }
        }

        // Take closest players to enemies
        quint8 mark = DIST_INVALID_ID;
        if(markList.size() > 0){
            mark = markList.takeFirst();

            if(PlayerBus::theirPlayerAvailable(mark)){
                for(int x = 0; x < allies.size(); x++){
                    for(int y = 0; y < allies.size() - 1; y++){
                        if(PlayerBus::ourPlayerAvailable(allies.at(y)) && PlayerBus::ourPlayerAvailable(allies.at(y+1))){
                            if(PlayerBus::ourPlayer(allies.at(y))->distanceTo(PlayerBus::theirPlayer(mark)->position()) > PlayerBus::ourPlayer(allies.at(y+1))->distanceTo(PlayerBus::theirPlayer(mark)->position())){
                                swap(allies[y], allies[y+1]);
                            }
                        }
                    }
                }
            }

            std::cout << "mais proximo de : " << int(mark) << " eh o " << int(allies[0]) << std::endl;
        }

        // Closest to closest enemy
        quint8 playerId = DIST_INVALID_ID;
        if(allies.size() > 0){
            playerId = allies.takeFirst();
            dist()->removePlayer(playerId);
        }

        // Taking the Defensive Midfielder 1
        if(playerId != DIST_INVALID_ID){
            quint8 markId = mark;
            _rl_def_midf->setMarkId(markId);
            setPlayerRole(playerId, _rl_def_midf);
        }
    }

    // Take barriers and sort them by closest to top quadrant
    QList<quint8> players = dist()->getKNN(2, loc()->ball());
    for(int x = 0; x < players.size(); x++){
        for(int y = 0; y < players.size() - 1; y++){
            if(PlayerBus::ourPlayerAvailable(players.at(y)) && PlayerBus::ourPlayerAvailable(players.at(y+1))){
                Position top1 = Position(true, PlayerBus::ourPlayer(players.at(y))->position().x(), 3.0, 0.0);
                Position top2 = Position(true, PlayerBus::ourPlayer(players.at(y+1))->position().x(), 3.0, 0.0);
                if(PlayerBus::ourPlayer(players.at(y))->distanceTo(top1) > PlayerBus::ourPlayer(players.at(y+1))->distanceTo(top2)){
                    swap(players[y], players[y+1]);
                }
            }
        }
    }

    QList<quint8> barriers;
    int placedBarriers = 0;
    float minDist = 999.0f;
    quint8 id = DIST_INVALID_ID;
    for(int x = 0; x < 2; x++){
        quint8 playerId = players.takeFirst();
        dist()->removePlayer(playerId);
        if(playerId != DIST_INVALID_ID){
            barriers.push_back(playerId);
            if(placedBarriers == 0){
                if(loc()->ourSide().isRight()) _rl_def.at(x)->setBarrierSide('l');
                else                           _rl_def.at(x)->setBarrierSide('r');
                placedBarriers++;
            }
            else{
                if(loc()->ourSide().isRight()) _rl_def.at(x)->setBarrierSide('r');
                else                           _rl_def.at(x)->setBarrierSide('l');
            }

            setPlayerRole(playerId, _rl_def.at(x));

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

}

void Playbook_Defense::resetMarkList(){
    markList.clear();

    QList<Player*> list = loc()->getOpPlayers().values();
    QList<Player*>::iterator it;

    int ballQuadrant = WR::Utils::getPlayerQuadrant(loc()->ball(), loc()->ourSide().oppositeSide());

    // Remove their defensive players or who is in the same quadrant as ball
    for(it = list.begin(); it != list.end(); it++){
        int playerQuadrant = WR::Utils::getPlayerQuadrant((*it)->position(), loc()->ourSide().oppositeSide());
        if((*it)->distOurGoal() >= 2.0f && playerQuadrant != ballQuadrant){
            markList.push_back((*it)->playerId());
        }
    }

    // Sort for priority (closest to our goal)
    for(int x = 0; x < markList.size(); x++){
        for(int y = 0; y < markList.size() - 1; y++){
            if(PlayerBus::theirPlayerAvailable(markList.at(y))){
                if(PlayerBus::theirPlayerAvailable(markList.at(y+1))){
                    if(PlayerBus::theirPlayer(markList.at(y))->distTheirGoal() > PlayerBus::theirPlayer(markList.at(y+1))->distTheirGoal()){
                        swap(markList[y], markList[y+1]);
                    }
                }
            }
        }
    }
}

quint8 Playbook_Defense::requestMarkPlayer(quint8 playerId){
    float minDist = 999.0f;
    quint8 markId = DIST_INVALID_ID;
    int pos = DIST_INVALID_ID;

    // always have 1 gk + 2 barriers, so the qt markers are defined by the actual minus 3
    int qtMarkers = std::max(0, getAllPlayers().size() - 3);
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
