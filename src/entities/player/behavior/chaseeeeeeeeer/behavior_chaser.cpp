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

#include "behavior_chaser.h"
#include <src/constants/constants.h>

#define ALIGNMENT_ERROR 0.2f

Behavior_Chaser::Behavior_Chaser() {
    _CHAAAAAAAAAAAAAAAAAAARGE = false;
}

void Behavior_Chaser::configure() {
    _skill_goTo = new Skill_GoTo();
    _skill_charge = new Skill_Charge();

    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_CHARGE, _skill_charge);

    _hasBallPos = false;
}

void Behavior_Chaser::run() {
    if (hammerTime()) {
        Geometry::Angle orientationToTarget = Geometry::Angle((_chasePos - player()->getPosition()).angle());
        Geometry::Angle orientationPlayer = player()->getOrientation();
        Geometry::Angle orientationPlayerBack = Geometry::Angle(player()->getOrientation().value() + M_PI);
        float frontAngle = orientationPlayer.shortestAngleDiff(orientationToTarget);
        float backAngle = orientationPlayerBack.shortestAngleDiff(orientationToTarget);

        _skill_charge->setUseSwappedOrientation(backAngle < frontAngle);
        runSkill(SKILL_CHARGE);
    } else {
        _skill_goTo->setTargetPosition(getChasePosition());
        _skill_goTo->enableWallAntiStuck(true);
        runSkill(SKILL_GOTO);
    }
}

Geometry::Vector2D Behavior_Chaser::getChasePosition() {
    Geometry::Vector2D chasePos = _chasePos;
    bool aimmingBackwards;
    if (Constants::teamPlaySide() == Common::Enums::Side::SIDE_RIGHT) {
        aimmingBackwards = player()->getPosition().x() <= _chasePos.x();
    } else {
        aimmingBackwards = player()->getPosition().x() >= _chasePos.x();
    }

    if (aimmingBackwards) {
        Geometry::Vector2D unitVector = getWorldMap()->getBall().getPosition() - getWorldMap()->getField().theirGoalCenter();
        chasePos = unitVector.normalize() * 0.4f * unitVector.length();

        if (inDangerZone(ALIGNMENT_ERROR)) {
            if (!_hasBallPos) {
                float sideY = chasePos.y();
                float sideFactor = 0.4f;

                Geometry::Vector2D topBoundary = Geometry::Vector2D(chasePos.x(), 10.0f);
                Geometry::Vector2D bottomBoundary = Geometry::Vector2D(chasePos.x(), -10.0f);
                if (chasePos.dist(topBoundary) < chasePos.dist(bottomBoundary)){
                    sideFactor = sideFactor * -1;
                }
                sideY = sideY + sideFactor;

                if (sideY > 0.7f) sideY = 0.7f;
                if (sideY < -0.7f) sideY = -0.7f;

                chasePos = Geometry::Vector2D(chasePos.x(), sideY);
            } else {
                chasePos = player()->getPosition();
            }
        }

        Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();

        if (!getWorldMap()->getField().field().contains(chasePos)) {
            spdlog::warn("chasePos out of field. Resetting to Going to ball.");
            std::cout << chasePos.x() << " | " << chasePos.y() << std::endl;
            chasePos = ballPos;
        }

        if (!player()->canEnterGoalArea() && getWorldMap()->getField().ourPenaltyArea().contains(ballPos)) {
            spdlog::warn("Cannot enter goal Area. Resetting to Going to center.");
            chasePos = getWorldMap()->getField().centerCircle().center();
        }
    }
    return chasePos;
}

bool Behavior_Chaser::inDangerZone(float alignementError) {
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    Geometry::Vector2D playerPos = player()->getPosition();

    bool centerTunnel = ballPos.y() >= getWorldMap()->getField().ourGoalRightPost().y() - alignementError && ballPos.y() <= getWorldMap()->getField().ourGoalLeftPost().y() + alignementError;
    bool aligned = (fabs(ballPos.y() - playerPos.y()) <= alignementError);
    bool closerToOurGoal = (ballPos.dist(getWorldMap()->getField().ourGoalCenter()) < ballPos.dist(getWorldMap()->getField().theirGoalCenter()));



    return centerTunnel && aligned && closerToOurGoal;
}

bool Behavior_Chaser::inDangerZoneOld() {
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();

    // Check if our goal posts are okay
    spdlog::info("Our Goal Right Post: ({}, {})", getWorldMap()->getField().ourGoalRightPost().x(), getWorldMap()->getField().ourGoalRightPost().y());
    spdlog::info("Our Goal Left Post: ({}, {})", getWorldMap()->getField().ourGoalLeftPost().x(), getWorldMap()->getField().ourGoalLeftPost().y());

    Geometry::Arc goalZone;
    if (Constants::teamPlaySide() == Common::Enums::SIDE_RIGHT) {
        if (player()->getPosition().y() >= 0.0) {
            goalZone = Geometry::Arc(ballPos, ballPos.dist(getWorldMap()->getField().ourGoalCenter()), Geometry::Angle(getWorldMap()->getField().ourGoalLeftPost().angle()), Geometry::Angle(getWorldMap()->getField().ourGoalRightPost().angle()));
        } else {
            goalZone = Geometry::Arc(ballPos, ballPos.dist(getWorldMap()->getField().ourGoalCenter()), Geometry::Angle(getWorldMap()->getField().ourGoalRightPost().angle()), Geometry::Angle(getWorldMap()->getField().ourGoalLeftPost().angle()));
        }
    } else {
        if (player()->getPosition().y() >= 0.0) {
            goalZone = Geometry::Arc(ballPos, ballPos.dist(getWorldMap()->getField().ourGoalCenter()), Geometry::Angle(getWorldMap()->getField().ourGoalRightPost().angle()), Geometry::Angle(getWorldMap()->getField().ourGoalLeftPost().angle()));
        } else {
            goalZone = Geometry::Arc(ballPos, ballPos.dist(getWorldMap()->getField().ourGoalCenter()), Geometry::Angle(getWorldMap()->getField().ourGoalLeftPost().angle()), Geometry::Angle(getWorldMap()->getField().ourGoalRightPost().angle()));
        }
    }

    Geometry::LineSegment robotToBall(getWorldMap()->getBall().getPosition(), player()->getPosition());

    std::vector<Geometry::Vector2D> intersection = goalZone.intersectionWithLine(robotToBall);


    spdlog::info("Intersection goal zone <-> robotToBall: {}", intersection.size());

    if (intersection.size() > 0) {
        return true;
    }

    spdlog::info("Not in danger zone");
    return false;
}
