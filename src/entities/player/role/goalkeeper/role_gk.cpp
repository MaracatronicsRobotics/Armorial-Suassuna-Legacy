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
    _gkOverlap = false;
    _isOverlapTimerInit = false;

    _ellipseParameters.first = 0.0f;
    _ellipseParameters.second = 0.0f;
    _ellipseCenter = Geometry::Vector2D(0.0f, 0.0f);
}

QString Role_GK::name() {
    return "Role_GK";
}

void Role_GK::configure() {
    // Starting behaviors
    _behavior_moveTo = new Behavior_MoveTo();
    _behavior_intercept = new Behavior_Intercept();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);
    addBehavior(BEHAVIOR_INTERCEPT, _behavior_intercept);

    // Taking the position where the GK wait for
    float goal_pos = 0.65f;

    if (Constants::teamPlaySide() == Common::Enums::Side::SIDE_RIGHT) {
        goal_pos = -1 * goal_pos;
    }
    _standardPos = Geometry::Vector2D(goal_pos, 0.0f);
    _currState = STATE_BLOCKBALL;
}

void Role_GK::run() {
    // Fixed variables
    Geometry::Vector2D ballVelocity = getWorldMap()->getBall().getVelocity();
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    //Geometry::Vector2D ballPred = getWorldMap()->getBall().getPredPosition(3 * getConstants()->predictionBaseCycles() / 4);
//    Geometry::Vector2D ballPred = getWorldMap()->getBall().getPosition();
//    Geometry::Vector2D playerPos = player()->getPosition();

    Geometry::Vector2D ballDirection;
    if(ballVelocity.length() > 0) {
        ballDirection = Geometry::Vector2D(ballVelocity.x()/ballVelocity.length(), ballVelocity.y()/ballVelocity.length());
    } else {
        ballDirection = Geometry::Vector2D(0, 0);
    }
    float factor = std::fminf(GKFACTOR * ballVelocity.length(), 0.5f);
    Geometry::Vector2D ballProjection = Geometry::Vector2D(ballPos.x() + factor*ballDirection.x(), ballPos.y() + factor*ballDirection.y());
    // Spin minimal distance from ball
//    float distSpin = 0.09f;
    // max vx = 2.8  min vx = 0
//    float maxPlus = 0.16f;
//    distSpin += maxPlus*(ballVelocity.length()/2.8f);
    // 0.09 + 0.16 * (ballVelocity / 2.8)

    // State Machine
    switch (_currState){
        case STATE_BLOCKBALL:
            {
                spdlog::info("STATE_BLOCKBALL");
                // position to block ball
                float block_y = ballProjection.y();
                float leftPostY = getWorldMap()->getField().ourGoalLeftPost().y() + 0.05f;
                float rightPostY = getWorldMap()->getField().ourGoalRightPost().y() - 0.05f;
                if (block_y < leftPostY){
                    block_y = leftPostY;
                }
                if (block_y > rightPostY){
                    block_y = rightPostY;
                }

                Geometry::Vector2D blockingPosition(_standardPos.x(), block_y);
                _behavior_moveTo->enableRotation(false);
                //_behavior_moveTo->setBaseSpeed(getConstants()->playerBaseSpeed());
                //_behavior_moveTo->enableSpin(false);
                //player()->setPlayerDesiredPosition(blockingPosition);
                _behavior_moveTo->setPosition(blockingPosition);
                spdlog::info("Desired Position: ({}, {})", blockingPosition.x(), blockingPosition.y());
                setBehavior(BEHAVIOR_MOVETO);


                bool pickCondition = getWorldMap()->getField().ourPenaltyArea().contains(ballPos);
                float pickBallMinVel = 0.2f;
                //std::cout << ballVelocity.length() << std::endl;
                pickCondition = pickCondition && (ballVelocity.length() <= pickBallMinVel);

                if (pickCondition){
                    _currState = STATE_PICKBALL;
                }

                break;
            }

        case STATE_PICKBALL:
            {
                spdlog::info("STATE_PICKBALL");
                Geometry::Vector2D pickPosition = ballPos;

                _behavior_moveTo->enableRotation(false);
//                _behavior_moveTo->setBaseSpeed(getConstants()->playerBaseSpeed());
//                player()->setPlayerDesiredPosition(pickPosition);
                _behavior_moveTo->setPosition(pickPosition);


//                if (playerPos.dist(ballPos) <= distSpin) {
//                    _behavior_moveTo->enableSpin(true);
//                }
//                else {
//                    _behavior_moveTo->enableSpin(false);
//                }
                spdlog::info("Desired Position: ({}, {})", pickPosition.x(), pickPosition.y());
                setBehavior(BEHAVIOR_MOVETO);

                if (!getWorldMap()->getField().ourField().contains(ballPos)){
                    _currState = STATE_BLOCKBALL;
                }

                break;
            }

        default:
            {
                _currState = STATE_BLOCKBALL;
            }
    }

}

//QPair<Geometry::Vector2D, Geometry::Vector2D> Role_GK::getPlacementPosition(VSSRef::Foul foul, VSSRef::Color forTeam, VSSRef::Quadrant atQuadrant) {
//    Position standardPosition;
//    if (getWorldMap()->getLocations()->ourSide().isRight()) {
//        standardPosition = Position(true, 0.67f, 0.0f);
//    } else {
//        standardPosition = Position(true, -0.67f, 0.0f);
//    }

//    Position foulPosition;
//    Angle foulAngle;

//    _gkOverlap = false;
//    _overlapTimer.stop();

//    switch (foul) {
//    case VSSRef::Foul::PENALTY_KICK: {
//        if (VSSRef::Color(getConstants()->teamColor()) == forTeam) {
//            foulPosition = standardPosition;
//            foulAngle = Angle(true, static_cast<float>(M_PI) / 2);
//        } else {
//            _gkOverlap = true;
//            _isOverlapTimerInit = false;
//            _overlapTimer.start();
//            if (getWorldMap()->getLocations()->ourSide().isLeft()) {
//                foulPosition = Position(true, -0.7125f, 0.0f);
//                foulAngle = Angle(true, 0);
//            } else {
//                foulPosition = Position(true, 0.7125f, 0.0f);
//                foulAngle = Angle(true, static_cast<float>(M_PI));
//            }
//        }
//    } break;
//    case VSSRef::Foul::KICKOFF: {
//        foulPosition = standardPosition;
//        foulAngle = Angle(true, static_cast<float>(M_PI) / 2);
//    } break;
//    case VSSRef::Foul::FREE_BALL: {
//        foulPosition = standardPosition;
//        foulAngle = Angle(true, static_cast<float>(M_PI) / 2);
//    } break;
//    case VSSRef::Foul::GOAL_KICK: {
//        if (static_cast<VSSRef::Color>(getConstants()->teamColor()) == forTeam) {
//            _gkOverlap = true;
//            _isOverlapTimerInit = false;
//            _overlapTimer.start();
//            Position ballPosition = getWorldMap()->getBall().getPosition();
//            if (getWorldMap()->getLocations()->ourSide().isLeft()) {
//                if (ballPosition.y() > 0.0f) {
//                    //Quadrant 2
//                    foulPosition = Position(true, standardPosition.x(), 0.28f);
//                    foulAngle = Angle(true, static_cast<float>(M_PI) / 4);
//                } else {
//                    // Quadrant 3
//                    foulPosition = Position(true, standardPosition.x(), -0.28f);
//                    foulAngle = Angle(true, static_cast<float>(M_PI) * 7 / 4);
//                }
//            } else {
//                if (ballPosition.y() > 0.0f) {
//                    // Quadrant 1
//                    foulPosition = Position(true, standardPosition.x(), 0.28f);
//                    foulAngle = Angle(true, static_cast<float>(M_PI) * 3 / 4);
//                } else {
//                    //Quadrant 4
//                    foulPosition = Position(true, standardPosition.x(), -0.28f);
//                    foulAngle = Angle(true, static_cast<float>(M_PI) * 5 / 4);
//                }
//            }
//        } else {
//            foulPosition = standardPosition;
//            foulAngle = Angle(true, static_cast<float>(M_PI) / 2);
//        }
//    } break;
//    default: {
//        foulPosition = standardPosition;
//        foulAngle = Angle(true, static_cast<float>(M_PI) / 2);
//    }
//    }

//    return QPair<Position, Angle>(foulPosition, foulAngle);
//}
