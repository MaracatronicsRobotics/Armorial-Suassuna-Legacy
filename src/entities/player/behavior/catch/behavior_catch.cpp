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

#include "behavior_catch.h"

#include <math.h>

#include <spdlog/spdlog.h>

Behavior_Catch::Behavior_Catch() {
    _skill_goTo = nullptr;
}

void Behavior_Catch::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);

    _actualState = STATE_CATCH;
}

void Behavior_Catch::run() {
   Geometry::Vector2D ballPosition = getWorldMap()->getBall().getPosition();
   Geometry::LineSegment goalSegment(getWorldMap()->getField().ourGoalLeftPost(), getWorldMap()->getField().ourGoalRightPost());
   Geometry::Vector2D projectionBallOntoGoal = goalSegment.project(ballPosition);
   Geometry::Vector2D catchPosition = Geometry::Vector2D(0.65, projectionBallOntoGoal.y());

   spdlog::info("Actual State:");

   // Check the best orientation to catch
   switch(_actualState) {
   case STATE_CATCH: {
       spdlog::info("STATE_CATCH:");
       Geometry::Angle angleToCatchPosition = (catchPosition - player()->getPosition()).angle();
       float diffUsingOrientation = player()->getOrientation().shortestAngleDiff(angleToCatchPosition);
       float diffUsingSwappedOrientation = Geometry::Angle(player()->getOrientation().value() + M_PI).shortestAngleDiff(angleToCatchPosition);

       spdlog::info("Catch Pos: ({}, {})", catchPosition.x(), catchPosition.y());
       _skill_goTo->setTargetPosition(catchPosition);
       if(diffUsingOrientation < diffUsingSwappedOrientation) {
           _skill_goTo->setUseSwappedOrientation(false);
       }
       else {
           _skill_goTo->setUseSwappedOrientation(true);
       }
       _skill_goTo->enableWallAntiStuck(true);
       runSkill(SKILL_GOTO);

       if(player()->getPosition().dist(catchPosition) <= 0.1) {
           _actualState = STATE_STOP;
       }
       break;
   }
   case STATE_STOP: {
       spdlog::info("STATE_STOP");
       player()->idle();

       if(player()->getPosition().dist(catchPosition) > 0.1) {
           _actualState = STATE_CATCH;
       }

       break;
   }
   }

    //Geometry::Vector2D dest(0.6, 0.0);
    
    // if(player()->getPosition().dist(dest) <= 0.1) {
    //     _skill_rotateTo->setTargetAngle(Geometry::Angle(M_PI/2.0));
    //     runSkill(SKILL_ROTATETO);
    //     spdlog::info("a");
    // }
    // else {
    //     _skill_goTo->setTargetPosition(dest);
    //     runSkill(SKILL_GOTO);
    //     spdlog::info("b");
    // }
}
