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

#include "behaviour_penalty_cf.h"
#include <entity/player/playerbus.h>

QString Behaviour_Penalty_CF::name() {
    return "Behaviour_Penalty_CF";
}

Behaviour_Penalty_CF::Behaviour_Penalty_CF() {

    _skill_kick = NULL;
    _skill_goToLookTo = NULL;
    firstChoose = false;
    joked = false;

    changeAimTimer.start();
}

void Behaviour_Penalty_CF::configure() {
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Test());
    setInitialSkill(_skill_goToLookTo);

    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_GOTO, _skill_kick, _skill_goToLookTo);
}

void Behaviour_Penalty_CF::run() {
    if(getConstants()==NULL)return;

    if(ref()->getGameInfo(player()->team()->teamColor())->theirPenaltyKick()){
        Position desiredPosition = Position(true, loc()->fieldCenter().x() + (loc()->ourSide().isLeft() ? 0.5 : -0.5), loc()->fieldCenter().y(), 0.0);
        _skill_goToLookTo->setAvoidBall(true);
        _skill_goToLookTo->setAvoidOpponents(true);
        _skill_goToLookTo->setAvoidTeammates(true);
        _skill_goToLookTo->setDesiredPosition(desiredPosition);
        _skill_goToLookTo->setAimPosition(loc()->ball());

        enableTransition(STATE_GOTO);
        return;
    }

    if(!player()->canKickBall()){
        joked = false;
        Position desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.5f, GEARSystem::Angle::pi);
        _skill_goToLookTo->setDesiredPosition(desiredPosition);
        _skill_goToLookTo->setAvoidBall(true);
        _skill_goToLookTo->setAimPosition(loc()->theirGoal());
        _skill_goToLookTo->setAvoidOpponents(true);
        _skill_goToLookTo->setAvoidTheirGoalArea(false);
        _skill_goToLookTo->setAvoidOurGoalArea(true);

        enableTransition(STATE_GOTO);
    }else{
        changeAimTimer.stop();
        if(changeAimTimer.timesec() >= 0.25 || !firstChoose){
            if(!firstChoose) firstChoose = true;
            _kickPosition = getBestKickPosition().second;
            changeAimTimer.start();
        }

        _skill_kick->setIsPenalty(true);
        _skill_kick->setKickPower(getConstants()->getMaxKickPower());

        if(loc()->isInsideTheirArea(loc()->ball(), 1.1f) || _skill_kick->getPushedDistance() >= 0.9 * _skill_kick->getMaxPushDistance()){
            // goal area penalty
            // try to 'joke' the enemy goalkeeper
            _skill_kick->setDestination(Position(false, 0.0, 0.0, 0.0));
            if(!joked){
                _skill_kick->setAim(loc()->theirGoalLeftPost());
                _skill_kick->shootWhenAligned(false);

                if(player()->getPlayerRotateAngleTo(loc()->theirGoalLeftPost()) < player()->aError()) joked = true;
            }
            else{
                _skill_kick->setAim(_kickPosition);
                _skill_kick->shootWhenAligned(true);
            }
        }
        else{
            // mid field area penalty
            _skill_kick->setAim(loc()->theirGoal());
            _skill_kick->setDestination(loc()->theirGoal());
            _skill_kick->shootWhenAligned(false);
        }

        enableTransition(STATE_KICK);
    }
}

std::pair<float, Position> Behaviour_Penalty_CF::getBestKickPosition(){
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
