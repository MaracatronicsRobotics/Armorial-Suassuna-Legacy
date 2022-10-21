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
#include "spdlog/spdlog.h"

#define ANGLE_OPENNESS 0.2
#define RADIUS 0.1f
#define ANGLE_ERROR 0.2f

Role_Attacker::Role_Attacker(bool supporter) {
    _supporter = supporter;
}

void Role_Attacker::configure() {
    _behavior_moveTo = new Behavior_MoveTo();
    _behavior_chaser = new Behavior_Chaser();

    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);
    addBehavior(BEHAVIOR_CHASER, _behavior_chaser);

    _currState = STATE_CHASE;
    if (_supporter) {
        _currState = STATE_SUPPORT;
    }
}

void Role_Attacker::run() {
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    std::string state = "NULL";

    Geometry::Vector2D chasePos = ballPos;

    bool hasBallPoss = hasPossession(ballPos);

    switch(_currState) {
    case(STATE_CHASE) :{
        state = "CHASE_BALL";
        if (hasBallPoss) {
            state = "CHASE_GOAL";
            chasePos = getWorldMap()->getField().theirGoalCenter();
            _behavior_chaser->hasBallPos(true);
        }

        _behavior_chaser->setChase(chasePos);
        _behavior_chaser->toCharge(false);

        setBehavior(BEHAVIOR_CHASER);
        if (hasBallPoss && alignedToTheirGoal()) {
            _currState = STATE_CHARGE;
        }
        if (ballPos.dist(getSecondAttackerPos()) < ballPos.dist(player()->getPosition())) {
            _currState = STATE_SUPPORT;
        }
        break;
    }
    case(STATE_CHARGE) :{
        state = "CHARGE";
        _behavior_chaser->setChase(chasePos);
        _behavior_chaser->toCharge(true);

        if (hasBallPoss) {
            _behavior_chaser->hasBallPos(true);
        }

        setBehavior(BEHAVIOR_CHASER);
        if (!hasPossession(ballPos)) {
            _currState = STATE_CHASE;
        }
        // Don't think state charge should go to support
        break;
    }
    case(STATE_SUPPORT) :{
        state = "SUPPORT";
        Geometry::Vector2D center = Geometry::Vector2D(0.2, ballPos.y());
        if (Constants::teamPlaySide() == Common::Enums::Side::SIDE_LEFT) {
            center = Geometry::Vector2D(center.x() * -1, center.y());
        }

        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->enableAntiStuck(true);
        _behavior_moveTo->setPosition(center);
        setBehavior(BEHAVIOR_MOVETO);
        if (ballPos.dist(getSecondAttackerPos()) <= ballPos.dist(player()->getPosition())) {
            _currState = STATE_CHASE;
        }
        break;
    }
    default:
        break;
    }
//    spdlog::info(state);
}

bool Role_Attacker::alignedToTheirGoal(){
    Geometry::Angle playerOri = player()->getOrientation();
    QList<double> points({-0.8, 0.0, 0.8});

    for (float point : points) {
        Geometry::Vector2D goalPoint = getWorldMap()->getField().theirGoalCenter();
        goalPoint += Geometry::Vector2D(0.0, point);
        Geometry::Angle playerAngleToGoal = (goalPoint - player()->getPosition()).angle();

        if (playerOri.shortestAngleDiff(playerAngleToGoal) <= ANGLE_ERROR) {
            return true;
        }
    }

    return false;
}

bool Role_Attacker::hasPossession(Geometry::Vector2D ballPos) {
    Geometry::Angle playerOri = player()->getOrientation();
    Geometry::Arc front = Geometry::Arc(player()->getPosition(), RADIUS, Geometry::Angle(playerOri - ANGLE_OPENNESS), Geometry::Angle(playerOri + ANGLE_OPENNESS));

    return front.pointInArc(ballPos);
}

Geometry::Vector2D Role_Attacker::getSecondAttackerPos() {
    Geometry::Vector2D secondAttackerPos;
    for (Player *p : player()->getTeamList()) {
        if ((p->roleName() == "ATTACKER") && (p->playerId() != player()->playerId())) {
            secondAttackerPos = p->getPosition();
        }
    }
    return secondAttackerPos;
}
