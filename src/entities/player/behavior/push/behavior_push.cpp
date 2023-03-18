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
    std::optional<Geometry::Vector2D> pushPosOpt;

    //std::cout << magic_enum::enum_name(_behaviorState) << std::endl;

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

        pushPosOpt = pushingLine.intersect(ballVerticalLine);

        Geometry::Vector2D pushPos = pushPosOpt.value();

        printf("Ponto de Bola: (%f,%f)\n", getWorldMap()->getBall().getPosition().x(), ballPosition.y());
        printf("Ponto de V1: (%f,%f)\n", VertLineVector1.x(), VertLineVector1.y());
        printf("Ponto de V2: (%f,%f)\n", VertLineVector2.x(), VertLineVector2.y());
        printf("Ponto de Interceptacao: (%f,%f)\n", pushPos.x(), pushPos.y());

        _skill_goTo->setTargetPosition(pushPosOpt.value());
        runSkill(SKILL_GOTO);

        if(playerPosition.dist(pushPosOpt.value()) < 0.1f){
            _behaviorState = ROTATE_STATE;
        }

        break;
    }
    case ROTATE_STATE:{
        Geometry::Angle angleToBall = (ballPosition - playerPosition).angle();
        _skill_rotateTo->setTargetAngle(angleToBall.value());
        runSkill(SKILL_ROTATETO);
        if((player()->getOrientation() - angleToBall).value() < 0.05f){
            _behaviorState = PUSH_STATE;
        }else if(playerPosition.dist(pushPosOpt.value()) > 0.20f){
            _behaviorState = GOTO_STATE;
        }
        break;
    }
    case PUSH_STATE:
        _skill_goTo->setTargetPosition(Geometry::Vector2D(0.0f,0.0f));
        runSkill(SKILL_GOTO);
        if(playerPosition.dist(Geometry::Vector2D(0.0f,0.0f)) < 0.05f){
            _behaviorState = GOTO_STATE;
        }
        break;
    default:
        _behaviorState = GOTO_STATE;
        break;
    }
}
