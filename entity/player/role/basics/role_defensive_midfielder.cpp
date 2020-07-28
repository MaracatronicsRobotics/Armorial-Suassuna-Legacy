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

QString Role_Defensive_Midfielder::name(){
    return "Role_Def_Midfielder";
}

Role_Defensive_Midfielder::Role_Defensive_Midfielder() {
    _bh_bar = NULL;
    _bh_mkBall = NULL;

    setMarkId(DIST_INVALID_ID);
}

void Role_Defensive_Midfielder::initializeBehaviours(){
    usesBehaviour(BEHAVIOUR_BARRIER, _bh_bar = new Behaviour_Barrier());
    usesBehaviour(BEHAVIOUR_MARKBALL, _bh_mkBall = new Behaviour_MarkBall());
}

void Role_Defensive_Midfielder::configure(){
    _minDist = 0.5;

    _bh_bar->setAvoidAllies(true);
    _bh_bar->setCanTakeout(true);
    _bh_bar->setInterceptRadius(0.3f);
}

void Role_Defensive_Midfielder::run(){
    float a = -0.06f;
    float b = 0.35f;
    float c = -1.45f;

    float x = -loc()->ball().x();
    float eq = a * powf(x, 2) + b * x + c;

    if(ref()->getGameInfo(player()->team()->teamColor())->directKick() || ref()->getGameInfo(player()->team()->teamColor())->indirectKick() || ref()->getGameInfo(player()->team()->teamColor())->kickoff()){
        if(loc()->isInsideOurField(loc()->ball()))
            _bh_bar->setRadius(1.8f);
        else
            _bh_bar->setRadius(std::min(std::max(1.8f, 4.5f + eq), 3.1f));

        // If is their direct, mark the player that will make the shoot (avoid direct kick to goal)
        if(ref()->getGameInfo(player()->team()->teamColor())->theirDirectKick()){
            quint8 theirPlayerId = getTheirPlayerClosestToBall();
            if(theirPlayerId == DIST_INVALID_ID)
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
        if(playerId == DIST_INVALID_ID){
            setBehaviour(BEHAVIOUR_MARKBALL);
        }
        else{
            float ourPlayerMinDist = PlayerBus::ourPlayer(playerId)->distBall();
            // If we are more far to the enemy than our closest player
            if(ourPlayerMinDist <= player()->distBall()){
                // setting radius
                _bh_bar->setRadius(1.8f);
                // updating mark id
                quint8 markId = getMarkId();
                if(markId != DIST_INVALID_ID) _bh_bar->setMarkPlayer(markId);
                else                          _bh_bar->setMarkBall();

                setBehaviour(BEHAVIOUR_BARRIER);
            }
            else{
                setBehaviour(BEHAVIOUR_MARKBALL);
            }
        }
    }
    else{
        // updating mark id
        quint8 markId = getMarkId();
        if(markId != DIST_INVALID_ID) _bh_bar->setMarkPlayer(markId);
        else                          _bh_bar->setMarkBall();

        if(loc()->isInsideOurField(loc()->ball())){
            _bh_bar->setRadius(1.8f);
        }
        else{
            if(markId != DIST_INVALID_ID && loc()->isInsideOurField(PlayerBus::theirPlayer(markId)->position())) _bh_bar->setRadius(1.8f);
            else _bh_bar->setRadius(std::min(std::max(1.8f, 4.5f + eq), 3.1f));
        }

        setBehaviour(BEHAVIOUR_BARRIER);
    }
}

quint8 Role_Defensive_Midfielder::getOurPlayerClosestToBall(){
    float closestDist = 999.0f;
    quint8 playerId   = DIST_INVALID_ID;
    for(quint8 id = 0; id < MRCConstants::_qtPlayers; id++){
        if(id == player()->playerId()) continue;
        if(PlayerBus::ourPlayerAvailable(id)){
            float distToBall = PlayerBus::ourPlayer(id)->distBall();
            if(distToBall <= closestDist){
                closestDist = distToBall;
                playerId = id;
            }
        }
    }

    return playerId;
}

quint8 Role_Defensive_Midfielder::getTheirPlayerClosestToBall(){
    float closestDist = 999.0f;
    quint8 playerId   = DIST_INVALID_ID;
    for(quint8 id = 0; id < MRCConstants::_qtPlayers; id++){
        if(PlayerBus::theirPlayerAvailable(id)){
            float distToBall = PlayerBus::theirPlayer(id)->distBall();
            if(distToBall <= closestDist){
                closestDist = distToBall;
                playerId = id;
            }
        }
    }
    return playerId;
}

void Role_Defensive_Midfielder::setMarkId(quint8 markId){
    _markMutex.lock();
    _markId = markId;
    _markMutex.unlock();
}

quint8 Role_Defensive_Midfielder::getMarkId(){
    _markMutex.lock();
    quint8 markId = _markId;
    _markMutex.unlock();

    return markId;
}
