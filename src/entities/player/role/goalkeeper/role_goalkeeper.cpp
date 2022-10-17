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

#include "role_goalkeeper.h"

#include <spdlog/spdlog.h>

Role_Goalkeeper::Role_Goalkeeper() {
    
}

void Role_Goalkeeper::configure() {
    // Starting behaviors
    _behavior_moveTo = new Behavior_MoveTo();
    _behavior_catch = new Behavior_Catch();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);
    addBehavior(BEHAVIOR_CATCH, _behavior_catch);

    _currState = MOVETO;
}

void Role_Goalkeeper::run() {
    double dist_GKball = player()->getPosition().dist(getWorldMap()->getBall().getPosition());
    Geometry::Vector2D goalCenter = Geometry::Vector2D(getWorldMap()->getField().ourGoalCenter().x(), 0.0f);

    switch(_currState) {
    case(CATCH):{
        setBehavior(BEHAVIOR_CATCH);
        if ((dist_GKball > 0.2) || (goalCenter.dist(player()->getPosition()) > 0.02)) {
            _currState = MOVETO;
        }
        break;        
    }
    case(MOVETO):{
        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->setPosition(goalCenter);
        setBehavior(BEHAVIOR_MOVETO);
        if(player()->getPosition().dist(goalCenter) < 0.02){
            _currState = STOP;
        }
        else if (dist_GKball < 0.2) {
            _currState = CATCH;
        }
        break;
    }
    case(STOP):{
        player()->idle();
        if(dist_GKball < 0.2){
            _currState = CATCH;
        } else if (goalCenter.dist(player()->getPosition()) > 0.02) {
            _currState = MOVETO;
        }
    }
    }

    /*
    O que o GK deve fazer?
    */
}
