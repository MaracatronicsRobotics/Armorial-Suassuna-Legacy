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

    QList<quint8> players = getAllPlayers();
    QList<Position> playersPositions;
    for(int x = 0; x < players.size(); x++){
        // Removing goalkeeper from list
        if(players.at(x) == _goalieId){
            players.removeAt(x);
            x--;
        }
        else{
            // If player is available, take it's position
            if(PlayerBus::ourPlayerAvailable(players.at(x)))
                playersPositions.push_back(PlayerBus::ourPlayer(players.at(x))->position());
        }
    }

    if(players.size() >= 3 && (players.size() == playersPositions.size())){
        // DPzona
        for(int x = 0; x < 32; x++){
            for(int y = 0; y < 32; y++){
                for(int z = 0; z < 32; z++){
                    dp[x][y][z].ok = false;
                }
            }
        }
        n_targets = 3;
        n_allies = playersPositions.size();

        // Taking "enemy" positions
        QList<Position> marks;
        Position posBall = loc()->ball();
        if(!markPositions.isEmpty()) marks.push_back(markPositions.takeFirst());
        else                         marks.push_back(posBall);
        marks.push_back(posBall);
        marks.push_back(posBall);

        // Calculating weights
        for(int x = 0; x < n_allies; x++){
            for(int y = 0; y < n_targets; y++){
                Position desiredX;

                if(y != 0) desiredX = WR::Utils::threePoints(loc()->ourGoal(), marks[y], 1.4f, 0.0f);
                else if(_rl_def_midf->getState() == 0) desiredX = WR::Utils::threePoints(loc()->ourGoal(), marks[y], 1.4f, 0.0f);
                else desiredX = loc()->ball();

                weight[x][y] = int((1000 * WR::Utils::distance(playersPositions[x], desiredX)));
            }
        }

        // Taking dp results
        int solution[6];

        std::pair<int, int> initialPlayer = solve();
        int actualPlayer = initialPlayer.first;
        int actualTarget = initialPlayer.second;
        int allieMask = 0;
        int targetMask = 0;

        while(actualPlayer != DIST_INVALID_ID || actualTarget != DIST_INVALID_ID){
            allieMask = (allieMask) | (1 << actualPlayer);
            targetMask = (targetMask) | (1 << actualTarget);

            int at = actualPlayer;
            solution[players[actualPlayer]] = actualTarget;

            actualPlayer = dp[allieMask][targetMask][at].player;
            actualTarget = dp[allieMask][targetMask][at].target;

        }

        QList<quint8> barriers;
        for(int x = 0; x < players.size(); x++){
            if(solution[players[x]] == 0){
                if(!markPositions.isEmpty()) _rl_def_midf->setMarkId(markList.takeFirst());
                else                         _rl_def_midf->setMarkId(DIST_INVALID_ID);
                setPlayerRole(players[x], _rl_def_midf);
            }
            else{
                barriers.push_back(players[x]);
            }
        }

        for(int x = 0; x < barriers.size(); x++){
            for(int y = 0; y < barriers.size() - 1; y++){
                if(PlayerBus::ourPlayerAvailable(barriers.at(y)) && PlayerBus::ourPlayerAvailable(barriers.at(y+1))){
                    Position top1 = Position(true, PlayerBus::ourPlayer(barriers.at(y))->position().x(), 3.0, 0.0);
                    Position top2 = Position(true, PlayerBus::ourPlayer(barriers.at(y+1))->position().x(), 3.0, 0.0);
                    if(PlayerBus::ourPlayer(barriers.at(y))->distanceTo(top1) > PlayerBus::ourPlayer(barriers.at(y+1))->distanceTo(top2)){
                        swap(barriers[y], barriers[y+1]);
                    }
                }
            }
        }

        int barrierAt = 0;
        float dist = 999.0f;
        quint8 bestId = DIST_INVALID_ID;
        for(int x = 0; x < barriers.size(); x++){
            float ourDist = WR::Utils::distance(PlayerBus::ourPlayer(barriers[x])->position(), loc()->ball());
            if(ourDist < dist){
                dist = ourDist;
                bestId = barriers[x];
            }
            setPlayerRole(barriers[x], _rl_def.at(barrierAt));
            if(barrierAt == 0){
                if(loc()->ourSide().isRight()) _rl_def.at(barrierAt)->setBarrierSide('l');
                else                           _rl_def.at(barrierAt)->setBarrierSide('r');
                barrierAt++;
            }
            else{
                if(loc()->ourSide().isRight()) _rl_def.at(barrierAt)->setBarrierSide('r');
                else                           _rl_def.at(barrierAt)->setBarrierSide('l');
            }
        }

        // Enable takeout for the barrier that is closest to the ball
        // (avoid double takeout and dribble errors)
        for(int x = 0; x < barriers.size(); x++){
            if(bestId == barriers.at(x))
                _rl_def.at(x)->setBarrierCanTakeout(true);
            else
                _rl_def.at(x)->setBarrierCanTakeout(false);
        }
    }
    else{
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
            quint8 playerId = DIST_INVALID_ID;
            if(players.size() > 0) playerId = players.takeFirst();
            if(playerId != DIST_INVALID_ID){
                dist()->removePlayer(playerId);
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
}

void Playbook_Defense::resetMarkList(){
    markList.clear();
    markPositions.clear();

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

    // Saving enemy position
    for(int x = 0; x < markList.size(); x++){
        if(PlayerBus::theirPlayerAvailable(markList.at(x))){
            markPositions.push_back(PlayerBus::theirPlayer(markList.at(x))->position());
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

int Playbook_Defense::search(int atual_aly_mask, int atual_target_mask, int player_atual, int target_atual){
/*
    marcation answer;

    // Starting with default values
    answer.ok = false;
    answer.val = inf;

    // If calculated, return (dp)
    if(dp[actualAllieMask][actualTargetMask][actualPlayer].ok) return dp[actualAllieMask][actualTargetMask][actualPlayer].val;

    // Else, calc
    dp[actualAllieMask][actualTargetMask][actualPlayer].val = weight[actualPlayer][actualTarget];
    dp[actualAllieMask][actualTargetMask][actualPlayer].ok = true;

    int allieMask, targetMask;

    for(int i = 0; i < n_allies; i++){
        if(actualTarget + 1 == n_targets) break;
        if((1 << i) & actualAllieMask) continue;

        allieMask = (actualAllieMask) | (1 << i);
        targetMask = (actualTargetMask) | (1 << (actualTarget + 1));

        int ans = search(allieMask, targetMask, i, actualTarget + 1);
        if(answer.ok == 0){
            answer.val = ans;
            answer.player = i;
            answer.target = actualTarget + 1;
            answer.ok = true;
        }
        else if(ans < answer.val){
            answer.player = i;
            answer.target = actualTarget + 1;
            answer.val = ans;
        }
    }

    if(answer.ok){
        dp[actualAllieMask][actualTargetMask][actualPlayer].val += answer.val;
        dp[actualAllieMask][actualTargetMask][actualPlayer].player = answer.player;
        dp[actualAllieMask][actualTargetMask][actualPlayer].target = answer.target;
    }
    else{
        dp[actualAllieMask][actualTargetMask][actualPlayer].player = DIST_INVALID_ID;
        dp[actualAllieMask][actualTargetMask][actualPlayer].target = DIST_INVALID_ID;
    }

    return dp[actualAllieMask][actualTargetMask][actualPlayer].val;
    */

    marcation resposta;
    /*if(player_atual == 2 && target_atual == 0){
        cout << "Comecei o suspeito!\n";
        cout << atual_aly_mask << " " << atual_target_mask << endl;
        cout << " ok vale "  << dp[atual_aly_mask][atual_target_mask].ok << endl;
    }*/
    resposta.ok = 0;
    resposta.val = inf;
    if(dp[atual_aly_mask][atual_target_mask][player_atual].ok)return dp[atual_aly_mask][atual_target_mask][player_atual].val;

    /*if(player_atual == 2 && target_atual == 0){
        cout << "suspeito rolando\n";
    }*/
    dp[atual_aly_mask][atual_target_mask][player_atual].val = weight[player_atual][target_atual];
    dp[atual_aly_mask][atual_target_mask][player_atual].ok = 1;

    int aly_mask, target_mask;

    for(int i=0; i<n_allies; i++){
        if(target_atual+1 == n_targets)break;
        if((1 << i) & atual_aly_mask)continue;
        aly_mask = (atual_aly_mask) | (1 << i);
        target_mask = (atual_target_mask) | (1 << (target_atual+1));

        int ans_recursao = search(aly_mask, target_mask, i, target_atual+1);
        if(resposta.ok == 0){
            resposta.val = ans_recursao;
            resposta.player = i;
            resposta.target = target_atual+1;
            resposta.ok = 1;
        }else if(ans_recursao < resposta.val){
            resposta.player = i;
            resposta.target = target_atual+1;
            resposta.val = ans_recursao;
        }
    }
    if(resposta.ok){
        dp[atual_aly_mask][atual_target_mask][player_atual].val += resposta.val;
        dp[atual_aly_mask][atual_target_mask][player_atual].player = resposta.player;
        dp[atual_aly_mask][atual_target_mask][player_atual].target = resposta.target;
    }else{
        dp[atual_aly_mask][atual_target_mask][player_atual].player = DIST_INVALID_ID;
        dp[atual_aly_mask][atual_target_mask][player_atual].target = DIST_INVALID_ID;
    }
    /*if(player_atual == 2 && target_atual == 0){
        cout << "Esse e o suspeito: ";
    }*/
    //cout << dp[atual_aly_mask][atual_target_mask].player << " " << dp[atual_aly_mask][atual_target_mask].target << endl;
    //cout << dp[atual_aly_mask][atual_target_mask][player_atual].val << endl;
    return dp[atual_aly_mask][atual_target_mask][player_atual].val;
}

std::pair<int, int> Playbook_Defense::solve(){
    /*
    marcation answer;

    // Setting default
    answer.ok = 0;
    answer.val = inf;

    int ans;
    for(int i = 0; i < n_allies; i++){
        int targetMask = 1 << 0; // Mask to allert marked players
        int allieMask = 1 << i; // Mask to allert allies that're marking

        ans = search(allieMask, targetMask, i, 0);

        if(answer.ok == 0){
            answer.player = i;
            answer.target = 0;
            answer.val = ans;
            answer.ok = true;
        }
        else{
            answer.player = i;
            answer.target = 0;
            answer.val = ans;
        }
    }

    return make_pair(answer.player, answer.target);
    */

    marcation resposta;
    resposta.ok = 0;
    resposta.val = inf;

    int ans_da_recurssao;
    for(int i=0; i<n_allies; i++){
        int target_mask = 1 << 0;//a máscara que avisa quais alvos estão marcados, aqui estamos avisando que o alvo 0 estar marcado
        int aly_mask = 1 << i;//máscara que avisa quais aliados estão ocupados marcando
        ans_da_recurssao = search(aly_mask,target_mask,i,0);
        if(resposta.ok == 0){
            resposta.player = i;
            resposta.target = 0;
            resposta.val = ans_da_recurssao;
            resposta.ok = 1;
        }else if(ans_da_recurssao < resposta.val){
            resposta.player = i;
            resposta.target = 0;
            resposta.val = ans_da_recurssao;
        }
    }

    return make_pair(resposta.player, resposta.target);
}
