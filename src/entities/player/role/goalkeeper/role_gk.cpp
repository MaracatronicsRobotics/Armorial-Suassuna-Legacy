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

#include "role_gk.h"

#define GKFACTOR 0.2f

Role_GK::Role_GK() {

}

void Role_GK::configure() {
    // Starting behaviors
    _behavior_default = new Behavior_Default();
    _behavior_moveTo = new Behavior_MoveTo();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_DEFAULT, _behavior_default);
    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);

    _currState = MOVETO;
}

void Role_GK::run() {
    Geometry::Vector2D ball_pos = getWorldMap()->getBall().getPosition();

    Geometry::Vector2D goalCenter = Geometry::Vector2D(getWorldMap()->getField().ourGoalCenter().x() + factor * 0.06, 0.0f);

    switch(_currState){
    case(MOVETO):{
        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->setPosition(goalCenter);
        setBehavior(BEHAVIOR_MOVETO);

        if(player()->getPosition().dist(goalCenter) < 0.05){
            _currState = KEEP;
        }
        break;
    }
    case(KEEP):{
        Geometry::Line lineDefense(Geometry::Vector2D(goalCenter.x(), 0.3f), Geometry::Vector2D(goalCenter.x(), 0.3f));

        Geometry::Line ballLine(Geometry::Vector2D(getWorldMap()->getBall().getPosition()),
                                      getWorldMap()->getBall().getVelocity());

        std::optional<Geometry::Vector2D> interceptPosition = ballLine.intersect(lineDefense);

        std::cout << interceptPosition.size() << std::endl;




//        if(){ // Ball Near -> Ball is the Target
//            _behavior_moveTo->enableRotation(false);
//            _behavior_moveTo->enableSpin(false);
//            _behavior_moveTo->setForcebleMotion(false);
//            _behavior_moveTo->setPosition(interceptPosition[0]);
//        }

        break;
    }
    case(SPIN):{
        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->enableSpin(true);
        _behavior_moveTo->setForcebleMotion(false);
        setBehavior(BEHAVIOR_MOVETO);

        if(getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition())) {
            _currState = KEEP;
        } else {
            _currState = MOVETO;
        }
    }
    }
}


