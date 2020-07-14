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

#include "behaviour_attacker.h"

#include <utils/freeangles/freeangles.h>
#include <utils/line/line.hh>

#define RECEIVER_INVALID_ID 200

QString Behaviour_Attacker::name() {
    return "Behaviour_Attacker";
}

Behaviour_Attacker::Behaviour_Attacker() {
    _sk_goToLookTo = NULL;
    _sk_push       = NULL;
    _mrcconstants=getConstants();
}

void Behaviour_Attacker::configure() {
    // Skills
    usesSkill(_sk_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_sk_push       = new Skill_Test());
    usesSkill(_sk_kick       = new Skill_Kick());

    // Transitions
    addTransition(SKT_POS,  _sk_push, _sk_goToLookTo);
    addTransition(SKT_POS,  _sk_kick, _sk_goToLookTo);

    addTransition(SKT_PUSH, _sk_goToLookTo, _sk_push);
    addTransition(SKT_PUSH, _sk_kick, _sk_push);

    addTransition(SKT_KICK, _sk_goToLookTo, _sk_kick);
    addTransition(SKT_KICK, _sk_push, _sk_kick);

    // Initial Skill
    setInitialSkill(_sk_goToLookTo);

    // Initial state
    _state = STATE_CANTKICK;

    _alreadyShooted = true;
}

void Behaviour_Attacker::run() {
    if(!canTakeBall())
        _state = STATE_CANTKICK;

    switch(_state){
    case STATE_CANTKICK:{
        // Check if can switch state
        if(canTakeBall()){
            _state = STATE_PUSH;
            break;
        }

        // Transition to goToLookTo
        enableTransition(SKT_POS);
        Position lookPosition, desiredPosition;
        if(loc()->isOutsideField(loc()->ball())){
            desiredPosition = player()->position();
            lookPosition = loc()->ball();
        }
        else{
            if(ref()->getGameInfo(player()->team()->teamColor())->theirDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->theirIndirectKick()){
                desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 0.6f, 0.0f);
                lookPosition = loc()->ball();
            }
            else{
                lookPosition = loc()->theirGoal();
                if(loc()->isInsideTheirArea(loc()->ball())) desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 1.7f, GEARSystem::Angle::pi);
                else if(loc()->isInsideOurArea(loc()->ball())){
                    desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 1.7f, GEARSystem::Angle::pi);
                    lookPosition = loc()->ourGoal();
                }
                else desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.6f, GEARSystem::Angle::pi);
            }
        }

        // Adjusting goToLookTo
        _sk_goToLookTo->setAvoidBall(true);
        _sk_goToLookTo->setDesiredPosition(desiredPosition);
        _sk_goToLookTo->setAimPosition(lookPosition);
    }
    break;
    case STATE_PUSH:{
        if(!player()->hasBallPossession() && loc()->ballVelocity().abs() >= 0.2f)
            _alreadyShooted = true;

        if(ref()->getGameInfo(player()->team()->teamColor())->ourIndirectKick()){
            quint8 bestReceiver = getBestReceiver();
            if(bestReceiver != RECEIVER_INVALID_ID){
                // Aim to our best receiver
                Position ourReceiverKickDevice = WR::Utils::getPlayerKickDevice(bestReceiver);
                _sk_kick->setAim(ourReceiverKickDevice);

                // Check if the path is obstructed
                QList<quint8> shootList = {player()->playerId(), bestReceiver};
                bool isObstructed = loc()->isVectorObstructed(player()->position(), ourReceiverKickDevice, shootList, MRCConstants::_robotRadius * 3.0, false);

                // Adjust kick power based on obstructed path or distance to receiver
                if(isObstructed) _sk_kick->setPower(std::min(6.0, 0.75 * sqrt((player()->distanceTo(ourReceiverKickDevice) * 9.8) / sin(2 * GEARSystem::Angle::toRadians(65.0)))));
                else             _sk_kick->setPower(std::min(6.0, std::max(3.0, 2.0 * player()->distanceTo(ourReceiverKickDevice))));

                // Set if is parabolic
                _sk_kick->setIsChip(isObstructed);
            }
            else{
                // If we don't have any receivers, shoot to their goalie
                quint8 theirGoalie = getTheirClosestPlayerToGoal();
                if(theirGoalie == RECEIVER_INVALID_ID){
                    // they don't have any players (?) shoot to their goal anyway
                    _sk_kick->setAim(loc()->theirGoal());
                    _sk_kick->setIsChip(false);
                    _sk_kick->setPower(MRCConstants::_maxKickPower);
                }
                else{
                    // shoot to their goalie
                    _sk_kick->setAim(PlayerBus::theirPlayer(theirGoalie)->position());
                    _sk_kick->setIsChip(false);
                    _sk_kick->setPower(MRCConstants::_maxKickPower);
                }
            }
            enableTransition(SKT_KICK);
        }
        else{
            // This is the game_on / our kickoff / our direct kick situation
            // For kickoff and direct kick always use KICK skill
            // Otherwise, use pushBall skill

            // First get the best position to kick and the angle interval
            std::pair<float, Position> bestAim = getBestAimPosition();

            // If don't have any free angles, the path is obstructed or have an unsufficient opening for kick
            if((bestAim.second.isUnknown() || bestAim.first <= GEARSystem::Angle::toRadians(6.0f))){
                // Get the best receiver to make a pass
                quint8 bestReceiver = getBestReceiver();
                if(bestReceiver != RECEIVER_INVALID_ID){
                    // Aim to our best receiver
                    Position ourReceiverKickDevice = WR::Utils::getPlayerKickDevice(bestReceiver);

                    // Check if the path is obstructed
                    QList<quint8> shootList = {player()->playerId(), bestReceiver};
                    bool isObstructed = loc()->isVectorObstructed(player()->position(), ourReceiverKickDevice, shootList, MRCConstants::_robotRadius * 3.0, false);
                    float power;
                    // Adjust kick power based on obstructed path or distance to receiver
                    if(isObstructed) power = std::min(6.0, (0.75 * sqrt((player()->distanceTo(ourReceiverKickDevice) * 9.8) / sin(2 * GEARSystem::Angle::toRadians(65.0)))));
                    else             power = (std::min(6.0, std::max(3.0, 2.0 * player()->distanceTo(ourReceiverKickDevice))));


                    // Set if is parabolic and make it shoot when sufficiently aligned to the receiver
                    if(ref()->getGameInfo(player()->team()->teamColor())->ourDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
                        _sk_kick->setAim(ourReceiverKickDevice);
                        _sk_kick->setIsChip(isObstructed);
                        _sk_kick->setPower(power);

                        enableTransition(SKT_KICK);
                    }else{
                        _sk_push->setAim(ourReceiverKickDevice);
                        _sk_push->setIsParabolic(isObstructed);
                        _sk_push->setKickPower(power);
                        _sk_push->shootWhenAligned(true);

                        enableTransition(SKT_PUSH);
                    }
                }
                else{
                    // If we don't have any receivers available, shot to their goal even if in bad conditions

                    // Check if the position to aim isn't unknown
                    Position aimPos;
                    if(!bestAim.second.isUnknown()) aimPos = (bestAim.second);
                    else                            aimPos = (loc()->theirGoal());

                    if(ref()->getGameInfo(player()->team()->teamColor())->ourDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
                        _sk_kick->setAim(aimPos);
                        _sk_kick->setIsChip(false);
                        _sk_kick->setPower(MRCConstants::_maxKickPower);

                        enableTransition(SKT_KICK);
                    }
                    else{
                        _sk_push->setAim(aimPos);
                        _sk_push->setIsParabolic(false);
                        _sk_push->setKickPower(MRCConstants::_maxKickPower);
                        _sk_push->shootWhenAligned(true);

                        enableTransition(SKT_PUSH);
                    }
                }
            }else{
                // Debug to UI
                if(CoachView::_isEnabled) CoachView::drawAttackerTriangle(player()->position(), loc()->theirGoalLeftPost(), loc()->theirGoalRightPost());
                if(CoachView::_isEnabled) CoachView::drawAttackerLine(player()->position(), bestAim.second);
                // If we have an sufficient opening to their goal, make an direct kick to it
                if(ref()->getGameInfo(player()->team()->teamColor())->ourDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
                    _sk_kick->setAim(bestAim.second);
                    _sk_kick->setIsChip(false);
                    _sk_kick->setPower(MRCConstants::_maxKickPower);

                    enableTransition(SKT_KICK);
                }
                else{
                    _sk_push->setAim(bestAim.second);
                    _sk_push->setIsParabolic(false);
                    _sk_push->setKickPower(MRCConstants::_maxKickPower);
                    _sk_push->shootWhenAligned(true);

                    enableTransition(SKT_PUSH);
                }
            }
        }
    }
    break;
    }
}

bool Behaviour_Attacker::canTakeBall(){
    return !(!player()->canKickBall() || loc()->isInsideOurArea(loc()->ball()) || loc()->isInsideTheirArea(loc()->ball()) || loc()->isOutsideField(loc()->ball()));
}

quint8 Behaviour_Attacker::getBestReceiver(){
    if(!_alreadyShooted){
        return _bestRcv;
    }
    else{
        float dist = 999.0f;
        quint8 id = RECEIVER_INVALID_ID;
        QList<quint8> list = _receiversList;
        for(int x = 0; x < list.size(); x++){
            if(list.at(x) == player()->playerId()) continue;
            if(PlayerBus::ourPlayerAvailable(list.at(x))){
                float distReceiver = player()->distanceTo(PlayerBus::ourPlayer(list.at(x))->position());
                if(distReceiver < dist){
                    dist = distReceiver;
                    id = list.at(x);
                }
            }
        }
        _alreadyShooted = false;
        _bestRcv = id;
        return id;
    }
}

quint8 Behaviour_Attacker::getTheirClosestPlayerToGoal(){
    QList<Player*> opPlayers = loc()->getOpPlayers().values();
    QList<Player*>::iterator it;

    float dist = 999.0f;
    quint8 id = RECEIVER_INVALID_ID;

    for(it = opPlayers.begin(); it != opPlayers.end(); it++){
        float distTheirGoal = (*it)->distanceTo(loc()->theirGoal());
        if(distTheirGoal < dist){
            dist = distTheirGoal;
            id = (*it)->playerId();
        }
    }

    return id;
}

std::pair<float, Position> Behaviour_Attacker::getBestAimPosition(){
    // Margin to avoid select the post as aim (or outside it)
    float postMargin = 0.05*loc()->fieldDefenseWidth()/2;

    // Adjust margin
    if(loc()->theirSide().isRight()) {
        postMargin = -postMargin;
    }

    // shift the position of the post to the center of the goal
    Position theirGoalRightPost = loc()->theirGoalRightPost();
    theirGoalRightPost.setPosition(theirGoalRightPost.x(),
                                   theirGoalRightPost.y() + postMargin,
                                   theirGoalRightPost.z());
    Position theirGoalLeftPost = loc()->theirGoalLeftPost();
    theirGoalLeftPost.setPosition(theirGoalLeftPost.x(),
                                  theirGoalLeftPost.y() - postMargin,
                                  theirGoalLeftPost.z());
    Position posTheirGoal = loc()->theirGoal();

    // get obstacles
    QList<Obstacle> obstacles = FreeAngles::getObstacles(loc()->ball());

    // Shift the obstacles
    QList<Obstacle>::iterator obst;

    // removing attacker from obstacles (avoid problems when he is rotating)
    for(int x = 0; x < obstacles.size(); x++){
        if(obstacles[x].id() == player()->playerId() && obstacles[x].team() == player()->teamId()){
            obstacles.removeAt(x);
            break;
        }
    }

    for(obst = obstacles.begin(); obst != obstacles.end(); obst++) {
        obst->radius() = 1.2 * getConstants()->getRobotRadius();
        // access the robot=
        PlayerAccess *robot = NULL;

        if(player()->opTeamId() == obst->team()) {
            robot = PlayerBus::theirPlayer(obst->id());
        } else {
            robot = PlayerBus::ourPlayer(obst->id());
        }

        Velocity robotVel;
        if(robot != NULL) {
            robotVel = robot->velocity();
        }

        if(robotVel.abs() > 2*0.015) {
            float time = WR::Utils::distance(loc()->ball(), obst->position())/8.0f;
            if(robotVel.abs()*time > 0.2f) {
                time = 0.2f/robotVel.abs();
            }
            obst->position() = WR::Utils::vectorSum(obst->position(), robotVel, time);
        }
    }

    // get free angle with the shifted obstacles
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(loc()->ball(), theirGoalRightPost, theirGoalLeftPost, obstacles);

    // Get the largest

    float largestAngle=0, largestMid=0;

    if(freeAngles.size()==0) { // Without free angles
        return std::make_pair(0.0, Position(false, 0.0, 0.0, 0.0));
    } else {
        for(int i=0; i<freeAngles.size(); i++) {
            float angI = freeAngles.at(i).angInitial();
            float angF = freeAngles.at(i).angFinal();
            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);
            float dif = angF - angI;
            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif>largestAngle) {
                largestAngle = dif;
                largestMid = angF - dif/2;
            }
        }
    }

    // With free angles

    // Triangle
    float x = posTheirGoal.x() - loc()->ball().x();
    float tg = tan(largestMid);
    float y = tg*x;

    // Impact point
    float pos_y = loc()->ball().y() + y;
    Position impactPos(true, posTheirGoal.x(), pos_y, 0.0);

    // Check if impact pos has enough space for the ball
    bool obstructedWay = loc()->isVectorObstructed(loc()->ball(), impactPos, player()->playerId(), getConstants()->getBallRadius()*1.5, false);

    if(obstructedWay) {
        return std::make_pair(0.0, Position(false, 0.0, 0.0, 0.0));
    }

    return std::make_pair(largestAngle, impactPos);
}
