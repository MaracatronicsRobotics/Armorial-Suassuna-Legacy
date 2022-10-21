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
    int factor = 1;
    if (Constants::teamPlaySide() == Common::Enums::Side::SIDE_RIGHT) {
        factor = -1;
    }
    Geometry::Vector2D goalCenter = Geometry::Vector2D(getWorldMap()->getField().ourGoalCenter().x() + factor * 0.06, 0.0f);

    switch(_currState) {
    case(CATCH):{
//        std::cout << "Fon\n";
//        Geometry::LineSegment lineDefense(Geometry::Vector2D(goalCenter.x(), 0.3f), Geometry::Vector2D(goalCenter.x(), 0.3f));
//        Geometry::LineSegment ballLine(Geometry::Vector2D(getWorldMap()->getBall().getPosition()),
//                                       Geometry::Vector2D(getWorldMap()->getField().ourGoalCenter()));
//        std::vector interceptPosition = lineDefense.intersects(ballLine);
//        float ball_y = getWorldMap()->getBall().getPosition().y();

//        if(ball_y > 0.2){
//            ball_y = 0.2;
//        }else if(ball_y < -0.2){
//            ball_y = 0.2;
//        }

//        if(ball_y >= player()->getPosition().y()){
//            _behavior_moveTo->setLeftWheelPower(255);
//            _behavior_moveTo->setRightWheelPower(255);
//        } else if (ball_y < player()->getPosition().y()) {
//            _behavior_moveTo->setLeftWheelPower(-255);
//            _behavior_moveTo->setRightWheelPower(-255);
//        }

        if(getWorldMap()->getField().ourPenaltyArea().contains(getWorldMap()->getBall().getPosition())){
            _behavior_moveTo->enableRotation(false);
            _behavior_moveTo->enableSpin(false);
            _behavior_moveTo->setForcebleMotion(true);
            _behavior_moveTo->setPosition(getWorldMap()->getBall().getPosition());
            setBehavior(BEHAVIOR_MOVETO);

        }else{
            //_behavior_moveTo->enableRotation()
        }

        if(getWorldMap()->getBall().getPosition().dist(player()->getPosition()) < 0.1){
            _currState = SPIN;
        } else if ((dist_GKball > 0.2) || (goalCenter.dist(player()->getPosition()) > 0.2)) {
            _currState = ROTATE;
        }
        if (!getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition())) {
            _currState = MOVETO;
        }
        break;        
    }
    case(MOVETO):{
        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->setPosition(goalCenter);
        setBehavior(BEHAVIOR_MOVETO);

        if(player()->getPosition().dist(goalCenter) < 0.05){
            _currState = ROTATE;
        }
        break;
    }
    case(ROTATE):{
        Geometry::Vector2D rotateReference = Geometry::Vector2D(goalCenter.x(), 1.3f);
        _behavior_moveTo->enableRotation(true);
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->setPosition(rotateReference);
        setBehavior(BEHAVIOR_MOVETO);

        if(fabs((rotateReference - player()->getPosition()).angle() - player()->getOrientation().value()) < 0.15) {
            _currState = CATCH;
        }
        break;
    }
    case(SPIN):{
        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->enableSpin(true);
        _behavior_moveTo->setForcebleMotion(false);
        setBehavior(BEHAVIOR_MOVETO);

        if (getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition())) {
            _currState = ROTATE;
        } else {
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

bool Role_Goalkeeper::isInsideGoalArea() {
    if (fabs(player()->getPosition().y()) < 0.35f && (player()->getPosition().x()) > 0.65f);
    getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition());
}
