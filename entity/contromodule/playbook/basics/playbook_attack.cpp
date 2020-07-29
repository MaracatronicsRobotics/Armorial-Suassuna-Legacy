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

#include "playbook_attack.h"

QString Playbook_Attack::name() {
    return "Playbook_Attack";
}

Playbook_Attack::Playbook_Attack() {
    _takeMainAttacker = false;
    _attackerId = DIST_INVALID_ID;
}

int Playbook_Attack::maxNumPlayer() {
    return 3;
}

void Playbook_Attack::configure(int numPlayers) {
    usesRole(_rl_stk = new Role_Striker());
    usesRole(_rl_stk2 = new Role_SecondStriker());
    usesRole(_rl_stk3 = new Role_SecondStriker());

    // Make connections
    connect(_rl_stk, SIGNAL(requestReceivers(quint8)), this, SLOT(requestReceivers(quint8)), Qt::DirectConnection);
    connect(this, SIGNAL(sendReceiver(quint8)), _rl_stk, SLOT(takeReceiver(quint8)), Qt::DirectConnection);

    connect(_rl_stk, SIGNAL(requestAttacker()), this, SLOT(requestAttacker()), Qt::DirectConnection);
    connect(this, SIGNAL(sendAttacker(quint8)), _rl_stk, SLOT(takeAttacker(quint8)), Qt::DirectConnection);

    connect(_rl_stk, SIGNAL(requestIsMarkNeeded()), this, SLOT(requestIsMarkNeeded()), Qt::DirectConnection);
    connect(this, SIGNAL(sendIsMarkNeeded(bool)), _rl_stk, SLOT(takeIsMarkNeeded(bool)), Qt::DirectConnection);

    connect(_rl_stk2, SIGNAL(requestReceivers(quint8)), this, SLOT(requestReceivers(quint8)), Qt::DirectConnection);
    connect(this, SIGNAL(sendReceiver(quint8)), _rl_stk2, SLOT(takeReceiver(quint8)), Qt::DirectConnection);

    connect(_rl_stk2, SIGNAL(requestAttacker()), this, SLOT(requestAttacker()), Qt::DirectConnection);
    connect(this, SIGNAL(sendAttacker(quint8)), _rl_stk2, SLOT(takeAttacker(quint8)), Qt::DirectConnection);

    connect(_rl_stk2, SIGNAL(requestIsMarkNeeded()), this, SLOT(requestIsMarkNeeded()), Qt::DirectConnection);
    connect(this, SIGNAL(sendIsMarkNeeded(bool)), _rl_stk2, SLOT(takeIsMarkNeeded(bool)), Qt::DirectConnection);

    connect(_rl_stk3, SIGNAL(requestReceivers(quint8)), this, SLOT(requestReceivers(quint8)), Qt::DirectConnection);
    connect(this, SIGNAL(sendReceiver(quint8)), _rl_stk3, SLOT(takeReceiver(quint8)), Qt::DirectConnection);

    connect(_rl_stk3, SIGNAL(requestAttacker()), this, SLOT(requestAttacker()), Qt::DirectConnection);
    connect(this, SIGNAL(sendAttacker(quint8)), _rl_stk3, SLOT(takeAttacker(quint8)), Qt::DirectConnection);

    connect(_rl_stk3, SIGNAL(requestIsMarkNeeded()), this, SLOT(requestIsMarkNeeded()), Qt::DirectConnection);
    connect(this, SIGNAL(sendIsMarkNeeded(bool)), _rl_stk3, SLOT(takeIsMarkNeeded(bool)), Qt::DirectConnection);
}

void Playbook_Attack::run(int numPlayers) {
    resetMarkList();
    if(!_takeMainAttacker || numPlayers != lastNumPlayers){
        quint8 player = dist()->getKNN(1, loc()->ball()).first();
        mainAttacker = player;

        _takeMainAttacker = true;
    }

    lastNumPlayers = numPlayers;

    quint8 player = mainAttacker;
    if(player != DIST_INVALID_ID){
        if(player != _attackerId) _rl_stk->setMarkId(requestMarkPlayer(player));
        dist()->removePlayer(player);
        setPlayerRole(player, _rl_stk);
    }

    player = dist()->getPlayer();
    if(player != DIST_INVALID_ID){
        if(player != _attackerId) _rl_stk2->setMarkId(requestMarkPlayer(player));
        setPlayerRole(player, _rl_stk2);
    }

    player = dist()->getPlayer();
    if(player != DIST_INVALID_ID){
        if(player != _attackerId) _rl_stk3->setMarkId(requestMarkPlayer(player));
        setPlayerRole(player, _rl_stk3);
    }
}

void Playbook_Attack::requestReceivers(quint8 playerId){
    QList<quint8> playersList = getPlayers();

    for(int x = 0; x < playersList.size(); x++){
        quint8 playerIdList = playersList.at(x);
        if(playerIdList == playerId){
            continue;
        }
        else{
            emit sendReceiver(playerIdList);
        }
     }
}

void Playbook_Attack::requestAttacker(){
    quint8 playerId = DIST_INVALID_ID;
    float maxDist = 999.0f;
    QList<quint8> playersList = getPlayers();

    for(int x = 0; x < playersList.size(); x++){
        PlayerAccess *player;
        if(!PlayerBus::ourPlayerAvailable(playersList.at(x))) continue;
        else player = PlayerBus::ourPlayer(playersList.at(x));
        if(player->distBall() < maxDist){
            maxDist = player->distBall();
            playerId = player->playerId();
        }
    }

    _attackerId = playerId;
    emit sendAttacker(playerId);
}

void Playbook_Attack::requestIsMarkNeeded(){
    // Check if the ball is coming to our players
    bool isNeeded = true;
    QList<quint8> playersList = getPlayers();
    for(int x = 0; x < playersList.size(); x++){
        PlayerAccess *player;
        if(!PlayerBus::ourPlayerAvailable(playersList.at(x))) continue;
        else player = PlayerBus::ourPlayer(playersList.at(x));
        if(isBallComing(player->position(), 0.2f, 1.0)){
            isNeeded = false;
            break;
        }
    }

    if(!isNeeded){
        // If ball is coming to our players
        emit sendIsMarkNeeded(isNeeded);
    }
    else{
        // Check other conditions
        if(loc()->isInsideOurField(loc()->ball()) || team()->opTeam()->hasBallPossession()){
            emit sendIsMarkNeeded(isNeeded);
        }
        else{
            emit sendIsMarkNeeded(!isNeeded);
        }
    }
}

bool Playbook_Attack::isBallComing(Position playerPosition, float minVelocity, float radius) {
    const Position posBall = loc()->ball();
    const Position posPlayer = playerPosition;

    // Check ball velocity
    if(loc()->ballVelocity().abs() < minVelocity)
        return false;

    // Angle player
    float angVel = loc()->ballVelocity().arg().value();
    float angPlayer = WR::Utils::getAngle(posBall, posPlayer);

    // Check angle difference
    float angDiff = WR::Utils::angleDiff(angVel, angPlayer);
    float angError = atan2(radius, WR::Utils::distance(playerPosition, loc()->ball()));

    return (fabs(angDiff) < fabs(angError));
}

void Playbook_Attack::resetMarkList(){
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

quint8 Playbook_Attack::requestMarkPlayer(quint8 playerId){
    float minDist = 999.0f;
    quint8 markId = DIST_INVALID_ID;
    int pos = DIST_INVALID_ID;

    int qtMarkers = getPlayers().size() - 1;
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

    if(pos != DIST_INVALID_ID)
        markList.removeAt(pos);

    return markId;
}
