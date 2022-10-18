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
    Geometry::Vector2D  ballPos = Geometry::Vector2D(getWorldMap()->getBall().getPosition());
//    Geometry::Vector2D goalCenter = Geometry::Vector2D(getWorldMap()->getField().ourGoalCenter().x(), 0.0f);
//    Geometry::Vector2D goalCenter = Geometry::Vector2D(getWorldMap()->getField().ourGoalLeftPost().x(), 0.0f);
    Geometry::Vector2D goalCenter = Geometry::Vector2D(0.7f, 0.0f);
    spdlog::info("Current State: {}", _currState);
    spdlog::info("BallPos: ({},{})", ballPos.x(), ballPos.y());

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
        float sidePosAdjust = 0.05f;
        if (getWorldMap()->getField().ourField().contains(getWorldMap()->getField().leftField().center())) {
            sidePosAdjust = -0.05f;
        }
        goalCenter = Geometry::Vector2D(goalCenter.x() - sidePosAdjust, limitGKSide(ballPos.y()));
        spdlog::info("GoalCenter: ({},{})", goalCenter.x(), limitGKSide(ballPos.y()));
        if(player()->getPosition().dist(goalCenter) < 0.1){
            _currState = STOP;
        } else if (dist_GKball < 0.2) {
            _currState = CATCH;
        }

        _behavior_moveTo->setPosition(goalCenter);
        setBehavior(BEHAVIOR_MOVETO);
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
}

float Role_Goalkeeper::limitGKSide(float desiredY) {
    if ((desiredY < 0.0f)) {
        return std::max(desiredY, -0.3f);
    } else {
        return std::min(desiredY, 0.3f);
    }
}
