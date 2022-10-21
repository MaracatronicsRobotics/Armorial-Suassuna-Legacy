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

#include "role_barrier.h"
#include "spdlog/spdlog.h"

#define TOWER_VARIANCE 0.05f
#define START_TOWER_AREA -0.15f
#define END_TOWER_AREA 0.15f

Role_Barrier::Role_Barrier() {

}

void Role_Barrier::configure() {
    _behavior_chaser = new Behavior_Chaser();
    addBehavior(BEHAVIOR_CHASER, _behavior_chaser);

    _currState = STATE_TOWER;

    _fixedTowerX = Constants::teamPlaySide() == Common::Enums::Side::SIDE_LEFT ? TOWER_VARIANCE : -1 * TOWER_VARIANCE;
}

void Role_Barrier::run() {
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    float ballY = ballPos.y();

    switch(_currState) {
    case(STATE_TOWER) :{

        Geometry::Vector2D towerPos = Geometry::Vector2D(_fixedTowerX, ballY);

        _behavior_chaser->setChase(towerPos);
        _behavior_chaser->toCharge(false);
        setBehavior(BEHAVIOR_CHASER);


        if (isInTowerArea(ballPos)) {
            _currState = STATE_BLOCK;
        }

        if (getWorldMap()->getField().ourField().contains(ballPos)){
            _currState = STATE_DEFEND;
        }


        break;
    }
    case(STATE_BLOCK) :{

        _behavior_chaser->setChase(ballPos);
        _behavior_chaser->toCharge(true);
        setBehavior(BEHAVIOR_CHASER);

        if ((!isInTowerArea(ballPos)) || player()->teamHasBall(ballPos)) {
            _currState = STATE_TOWER;
        }

        if (getWorldMap()->getField().ourField().contains(ballPos)){
            _currState = STATE_DEFEND;
        }

        break;
    }

    case(STATE_DEFEND) :{

        Geometry::Vector2D chasePos = ballPos;

        if (player()->hasPossession(ballPos)) {
            chasePos = getWorldMap()->getField().theirGoalCenter();
            _behavior_chaser->hasBallPos(true);
        }
        bool aligned = false;
        if (player()->alignedToTheirGoal()) {
            aligned = true;
        }

        _behavior_chaser->setChase(chasePos);
        _behavior_chaser->toCharge(aligned);
        setBehavior(BEHAVIOR_CHASER);

        if (!getWorldMap()->getField().ourField().contains(ballPos)){
            _currState = STATE_TOWER;
        }

        break;
    }

    default:
        break;
    }
}

bool Role_Barrier::isInTowerArea(Geometry::Vector2D target){
    return target.x() >= START_TOWER_AREA && target.x() <= END_TOWER_AREA;
}
