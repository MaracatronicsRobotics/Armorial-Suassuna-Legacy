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

#include "behavior_push.h"
#include <spdlog/spdlog.h>

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <Armorial/Libs/magic_enum/include/magic_enum.hpp>

Behavior_Push::Behavior_Push() {
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
}

void Behavior_Push::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);

    // Player distance to ball
    _pushingDistance = 0.1f;

    _behaviorState = GOTO_STATE;
}

void Behavior_Push::run() {
    Geometry::Vector2D ballPosition = getWorldMap()->getBall().getPosition();
    Geometry::Vector2D playerPosition = player()->getPosition();
    Geometry::Vector2D VertLineVector1,VertLineVector2;

    Geometry::Line pushingLine(Geometry::LineSegment(getWorldMap()->getField().theirGoalCenter(),
                                                     ballPosition));
    std::optional<Geometry::Vector2D> pushPos;

    switch (_behaviorState) {
    case GOTO_STATE:{
        if(getWorldMap()->getField().playingLeftSide()){
            VertLineVector1 = Geometry::Vector2D(ballPosition.x() - 0.1f, -0.650f);
            VertLineVector2 = Geometry::Vector2D((ballPosition.x() - 0.1f), 0.650f);
        }else{
            VertLineVector1 = Geometry::Vector2D(ballPosition.x() + 0.1f, -0.650f);
            VertLineVector2 = Geometry::Vector2D((ballPosition.x() + 0.1f), 0.650f);
        }

        Geometry::LineSegment ballVerticalLine(VertLineVector1,VertLineVector2);

        pushPos = pushingLine.intersect(ballVerticalLine);

        _skill_goTo->setTargetPosition(pushPos.value());
        runSkill(SKILL_GOTO);

        if(playerPosition.dist(pushPos.value()) < 0.1f){
            _behaviorState = ROTATE_STATE;
        }

        break;
    }
    case ROTATE_STATE:{
        Geometry::Angle angleToBall = (ballPosition - playerPosition).angle();
        _skill_rotateTo->setTargetAngle(angleToBall.value());
        runSkill(SKILL_ROTATETO);

        if(abs(player()->getOrientation().value() - angleToBall.value()) < 0.05f){
            _behaviorState = PUSH_STATE;
        }else if(abs(playerPosition.dist(ballPosition)) > 0.2f){
            _behaviorState = GOTO_STATE;
        }

        break;
    }
    case PUSH_STATE:
        if(abs(playerPosition.dist(ballPosition)) < 0.03f){
            _skill_goTo->setTargetPosition(getWorldMap()->getField().theirGoalCenter());
        }else{
            _skill_goTo->setTargetPosition(ballPosition);
        }

        runSkill(SKILL_GOTO);
        
        if(playerPosition.dist(ballPosition) > 0.15f){
            _behaviorState = GOTO_STATE;
        }
        break;
    default:
        _behaviorState = GOTO_STATE;
        break;
    }
}
