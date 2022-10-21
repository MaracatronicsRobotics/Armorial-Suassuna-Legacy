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
    Geometry::Vector2D chasePos = ballPos;

    bool hasBallPoss = player()->hasPossession(ballPos);

    switch(_currState) {
    case(STATE_CHASE) :{
        if (hasBallPoss) {
            chasePos = getWorldMap()->getField().theirGoalCenter();
            _behavior_chaser->hasBallPos(true);
        }

        _behavior_chaser->setChase(chasePos);
        _behavior_chaser->toCharge(false);

        setBehavior(BEHAVIOR_CHASER);
        if (hasBallPoss && player()->alignedToTheirGoal()) {
            _currState = STATE_CHARGE;
        }
        break;
    }
    case(STATE_CHARGE) :{
        _behavior_chaser->setChase(chasePos);
        _behavior_chaser->toCharge(true);

        if (hasBallPoss) {
            _behavior_chaser->hasBallPos(true);
        }

        setBehavior(BEHAVIOR_CHASER);
        if (!player()->hasPossession(ballPos)) {
            _currState = STATE_CHASE;
        }
        break;
    }

    default:
        break;
    }
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
