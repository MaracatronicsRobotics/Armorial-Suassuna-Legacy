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

#include "role_defensive_midfielder.h"
#define INVALID_ID 200

QString Role_Defensive_Midfielder::name(){
    return "Role_Defensive_Midfielder";
}

Role_Defensive_Midfielder::Role_Defensive_Midfielder() {
    _bh_bar = NULL;
    _bh_mkBall = NULL;
}

void Role_Defensive_Midfielder::initializeBehaviours(){
    usesBehaviour(BEHAVIOUR_BARRIER, _bh_bar = new Behaviour_Barrier());
    usesBehaviour(BEHAVIOUR_MARKBALL, _bh_mkBall = new Behaviour_MarkBall());
}

void Role_Defensive_Midfielder::configure(){
    _barrierId = 0;
    _minDist = 0.5;
}

void Role_Defensive_Midfielder::run(){
    double a = -0.06;
    double b = 0.35;
    double c = -1.45;

    double x = -loc()->ball().x();
    double eq = a * pow(x, 2) + b * x + c;

    if(ref()->getGameInfo(player()->team()->teamColor())->directKick() || ref()->getGameInfo(player()->team()->teamColor())->indirectKick() || ref()->getGameInfo(player()->team()->teamColor())->kickoff()){
        if(loc()->isInsideOurField(loc()->ball()))
            _bh_bar->setRadius(1.8);
        else
            _bh_bar->setRadius(std::min(std::max(1.8, 4.5 + eq), 3.1));

        // If is their direct, mark the player that will make the shoot (avoid direct kick to goal)
        if(ref()->getGameInfo(player()->team()->teamColor())->theirDirectKick()){
            quint8 theirPlayerId = getTheirPlayerClosestToBall();
            if(theirPlayerId == INVALID_ID)
                _bh_bar->setMarkBall();
            else
                _bh_bar->setMarkPlayer(theirPlayerId);
        }
        else
            _bh_bar->setMarkBall();

        setBehaviour(BEHAVIOUR_BARRIER);

    }
    else if(player()->team()->opTeam()->hasBallPossession() && loc()->isInsideOurField(loc()->ball())){
        quint8 playerId = getOurPlayerClosestToBall();
        if(playerId == INVALID_ID){
            setBehaviour(BEHAVIOUR_MARKBALL);
            return ;
        }
        double ourPlayerMinDist = PlayerBus::ourPlayer(playerId)->distBall();
        // If we are more far to the enemy than our closest player
        if(ourPlayerMinDist <= player()->distBall()){
            quint8 playerId = getTheirPlayerClosestToBall();
            _bh_bar->setRadius(1.8f);
            if(playerId != INVALID_ID)
                _bh_bar->setMarkPlayer(playerId);
            else
                _bh_bar->setMarkBall();
            setBehaviour(BEHAVIOUR_BARRIER);
        }
        else{
            setBehaviour(BEHAVIOUR_MARKBALL);
        }
    }
    else{
        // min radius: 1.8m       /     max radius: 3.1m      (from goal)
        _bh_bar->setRadius(std::min(std::max(1.8, 4.5 + eq), 3.1));
        setBehaviour(BEHAVIOUR_BARRIER);
    }
}

quint8 Role_Defensive_Midfielder::getOurPlayerClosestToBall(){
    float closestDist = 999.0f;
    quint8 playerId = INVALID_ID;
    for(quint8 id = 0; id < MRCConstants::_qtPlayers; id++){
        if(id == player()->playerId()) continue;
        if(PlayerBus::ourPlayerAvailable(id)){
            double distToBall = PlayerBus::ourPlayer(id)->distBall();
            if(closestDist <= distToBall){
                closestDist = distToBall;
                playerId = id;
            }
        }
    }
    return playerId;
}

quint8 Role_Defensive_Midfielder::getTheirPlayerClosestToBall(){
    float closestDist = 999.0f;
    quint8 playerId = INVALID_ID;
    for(quint8 id = 0; id < MRCConstants::_qtPlayers; id++){
        if(PlayerBus::theirPlayerAvailable(id)){
            double distToBall = PlayerBus::theirPlayer(id)->distBall();
            if(closestDist <= distToBall){
                closestDist = distToBall;
                playerId = id;
            }
        }
    }
    return playerId;
}
