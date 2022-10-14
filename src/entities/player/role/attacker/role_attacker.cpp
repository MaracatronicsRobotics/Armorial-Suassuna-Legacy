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

#include "role_attacker.h"

#define ANGLE_OPENNESS 0.2
#define RADIUS 0.1f
#define ANGLE_ERROR 0.1f

Role_Attacker::Role_Attacker() {

}

void Role_Attacker::configure() {
    _behavior_moveTo = new Behavior_MoveTo();
    _behavior_chaser = new Behavior_Chaser();

    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);
    addBehavior(BEHAVIOR_CHASER, _behavior_chaser);

    _currState = STATE_CHASE;
}

void Role_Attacker::run() {
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    switch(_currState) {
    case(STATE_CHASE) :{
        _behavior_chaser->setChase(ballPos);
        _behavior_chaser->toCharge(false);

        setBehavior(BEHAVIOR_CHASER);
        if (hasPossession(ballPos) && alignedToTheirGoal()) {
            _currState = STATE_CHARGE;
        }
        break;
    }
    case(STATE_CHARGE) :{
        _behavior_chaser->setChase(ballPos);
        _behavior_chaser->toCharge(true);

        setBehavior(BEHAVIOR_CHASER);
        if (!hasPossession(ballPos)) {
            _currState = STATE_CHASE;
        }
        break;
    }
    default:
        break;
    }
}

bool Role_Attacker::alignedToTheirGoal(){
    Geometry::Angle playerOri = player()->getOrientation();
    Geometry::Angle playerAngleToGoal = (getWorldMap()->getField().theirGoalCenter() - player()->getPosition()).angle();


    if (playerOri.shortestAngleDiff(playerAngleToGoal) <= ANGLE_ERROR) {
        return true;
    }
    return false;
}

bool Role_Attacker::hasPossession(Geometry::Vector2D ballPos) {
    Geometry::Angle playerOri = player()->getOrientation();
    Geometry::Arc front = Geometry::Arc(player()->getPosition(), RADIUS, Geometry::Angle(playerOri - ANGLE_OPENNESS), Geometry::Angle(playerOri + ANGLE_OPENNESS));
    Geometry::Arc rear =  Geometry::Arc(player()->getPosition(), RADIUS, Geometry::Angle((playerOri + M_PI) - ANGLE_OPENNESS), Geometry::Angle((playerOri + M_PI) + ANGLE_OPENNESS));

    return (front.pointInArc(ballPos) || rear.pointInArc(ballPos));
}
