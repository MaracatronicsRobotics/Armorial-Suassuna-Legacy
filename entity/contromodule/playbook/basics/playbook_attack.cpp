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
    if(!_takeMainAttacker || numPlayers != lastNumPlayers){
        quint8 player = dist()->getKNN(1, loc()->ball()).first();
        mainAttacker = player;

        _attackerId = player;
        _takeMainAttacker = true;
    }

    lastNumPlayers = numPlayers;

    resetQuadrantList();
    resetMarkList();

    quint8 player = mainAttacker;
    if(player != DIST_INVALID_ID){
        if(player != _attackerId){
            _rl_stk->setQuadrant(requestQuadrant(player));
            _rl_stk->setMarkId(requestMarkPlayer(player));
        }
        dist()->removePlayer(player);
        setPlayerRole(player, _rl_stk);
    }

    player = dist()->getPlayer();
    if(player != DIST_INVALID_ID){
        if(player != _attackerId){
            _rl_stk2->setQuadrant(requestQuadrant(player));
            _rl_stk2->setMarkId(requestMarkPlayer(player));
        }
        setPlayerRole(player, _rl_stk2);
    }

    player = dist()->getPlayer();
    if(player != DIST_INVALID_ID){
        if(player != _attackerId){
            _rl_stk3->setQuadrant(requestQuadrant(player));
            _rl_stk3->setMarkId(requestMarkPlayer(player));
        }
        setPlayerRole(player, _rl_stk3);
    }
}

int Playbook_Attack::requestQuadrant(quint8 playerId) {
    // If in direct / indirect / stop / penalty / kickoff, set our receivers to up and bot (walk together with attacker)
    SSLGameInfo *gameInfo = ref()->getGameInfo(team()->teamColor());
    if(gameInfo->freeKick() || gameInfo->kickoff() || gameInfo->penaltyKick() || !gameInfo->gameOn()){
        if(!leftTaked){
            leftTaked = true;
            return QUADRANT_UP;
        }
        else if(!rightTaked){
            rightTaked = true;
            return QUADRANT_BOT;
        }
    }

    // Check for potential quadrants to our player
    int qtAtQuadrants[4] = {0};
    QList<Player*> opPlayers = loc()->getOpPlayers().values();

    for(int x = 0; x < opPlayers.size(); x++){
        if(opPlayers.at(x) != NULL){
            // we don't count enemy gk
            if(!loc()->isInsideTheirArea(opPlayers.at(x)->position())){
                int quadrant = WR::Utils::getPlayerQuadrant(opPlayers.at(x)->position());
                if(quadrant != NO_QUADRANT){
                    qtAtQuadrants[quadrant-1]++;
                }
            }
        }
    }
/*
    float closestDist = 999.0f;
    float largestDist = 0.0f;
    int lessQtAtQuadrant = 999;
    int bestQuadrant = NO_QUADRANT;
    for(int x = 0; x < NUM_QUADRANTS; x++){
        if(quadrants[x] == true) continue;
        else{
            Position barycenter = WR::Utils::getQuadrantBarycenter(x+1);
            float distToBarycenter = WR::Utils::distance(loc()->ball(), barycenter);
            if(qtAtQuadrants[x] <= lessQtAtQuadrant){
                if(qtAtQuadrants[x] == lessQtAtQuadrant){
                    if(distToBarycenter > largestDist){
                        if(PlayerBus::ourPlayerAvailable(playerId)){
                            float distPlayerBarycenter = PlayerBus::ourPlayer(playerId)->distanceTo(barycenter);
                            if(distPlayerBarycenter < closestDist){
                                closestDist = distPlayerBarycenter;
                                largestDist = distToBarycenter;
                                bestQuadrant = x+1;
                            }
                        }
                    }
                }
                else{
                    bestQuadrant = x + 1;
                    largestDist = distToBarycenter;
                }
                lessQtAtQuadrant = qtAtQuadrants[x];
            }
        }
    }
*/

    float closestDist = 999.0f;
    int bestQuadrant = NO_QUADRANT;
    for(int x = 0; x < NUM_QUADRANTS; x++){
        if(quadrants[x] == true) continue;
        else{
            if(PlayerBus::ourPlayerAvailable(playerId)){
                Position barycenter = WR::Utils::getQuadrantBarycenter(x+1);
                float distPlayerToBarycenter = PlayerBus::ourPlayer(playerId)->distanceTo(barycenter);
                if(distPlayerToBarycenter < closestDist){
                    closestDist = distPlayerToBarycenter;
                    bestQuadrant = x + 1;
                }
            }
        }
    }

    if(bestQuadrant != NO_QUADRANT){
        //CoachView::drawLine(PlayerBus::ourPlayer(playerId)->position(), WR::Utils::getQuadrantBarycenter(bestQuadrant), RGBA(0.0, 255, 0.0, 1.0, MRCConstants::robotZ + 0.03));
        quadrants[bestQuadrant - 1] = true;
    }
    return bestQuadrant;
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

    if(playersList.size() >= 1){
        if(PlayerBus::ourPlayerAvailable(playersList.at(0))){
            if(ref()->getGameInfo(team()->teamColor())->freeKick() || ref()->getGameInfo(team()->teamColor())->kickoff()){
                emit sendAttacker(mainAttacker);
                return ;
            }
        }
    }

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
    if(team()->hasBallPossession()){
        emit sendIsMarkNeeded(false);
    }
    else{
        bool isNeeded = true;
        QList<quint8> playersList = getPlayers();
        for(int x = 0; x < playersList.size(); x++){
            PlayerAccess *player;
            if(!PlayerBus::ourPlayerAvailable(playersList.at(x))) continue;
            else player = PlayerBus::ourPlayer(playersList.at(x));
            if(isBallComing(player->position(), STRIKER_INTERCEPT_MINVEL, 1.0) && !team()->hasBallPossession()){
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

void Playbook_Attack::resetQuadrantList() {
    // mark all as free
    for(int x = 0; x < NUM_QUADRANTS; x++){
        quadrants[x] = false;
    }

    // mark ball quadrant as used
    int atkQuadrant = WR::Utils::getPlayerQuadrant(loc()->ball());
    if(atkQuadrant != NO_QUADRANT){
        quadrants[atkQuadrant-1] = true;
    }

    leftTaked  = false;
    rightTaked = false;
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
    for(int x = 0; x < markList.size() - 1; x++){
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
