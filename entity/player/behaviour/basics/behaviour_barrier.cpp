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

#include "behaviour_barrier.h"

#define INTERCEPT_MINBALLVELOCITY 0.2f
#define INTERCEPT_MINBALLDIST 0.5f
#define ERROR_GOAL_OFFSET 0.15f

QString Behaviour_Barrier::name() {
    return "Behaviour_Barrier";
}

Behaviour_Barrier::Behaviour_Barrier() {
    setMarkBall();
    setDistanceFromGk(0.0); // distance from our gk line to ball
    setRadius(1.4f); // radius from our goal center

    _sk_goto = NULL;
    _sk_gk = NULL;
    _sk_kick = NULL;
    _sk_push = NULL;

    _canTakeout = false;
    _avoidAllies = false;
}

void Behaviour_Barrier::configure() {
    usesSkill(_sk_goto = new Skill_GoToLookTo());
    usesSkill(_sk_gk = new Skill_InterceptBall());
    usesSkill(_sk_kick = new Skill_Kick());
    usesSkill(_sk_push = new Skill_PushBall2());

    // Setting initial skill
    setInitialSkill(_sk_goto);

    // Transitions
    addTransition(STATE_GOTO, _sk_kick, _sk_goto);
    addTransition(STATE_GOTO, _sk_gk, _sk_goto);
    addTransition(STATE_GOTO, _sk_push, _sk_goto);

    addTransition(STATE_GK, _sk_goto, _sk_gk);
    addTransition(STATE_GK, _sk_kick, _sk_gk);
    addTransition(STATE_GK, _sk_push, _sk_gk);

    addTransition(STATE_KICK, _sk_goto, _sk_kick);
    addTransition(STATE_KICK, _sk_gk, _sk_kick);
    addTransition(STATE_KICK, _sk_push, _sk_kick);

    addTransition(STATE_PUSH, _sk_goto, _sk_push);
    addTransition(STATE_PUSH, _sk_gk, _sk_push);
    addTransition(STATE_PUSH, _sk_kick, _sk_push);
};

void Behaviour_Barrier::run() {
    Position markPosition;
    if(_markNearestPlayer){
        if(_markPlayerId != 200){
            if(PlayerBus::theirPlayerAvailable(_markPlayerId))
                markPosition = PlayerBus::theirPlayer(_markPlayerId)->position();
            else
                markPosition = loc()->ball();
        }
        else{
            markPosition = loc()->ball();
        }
    }
    else
        markPosition = loc()->ball();

    // Pos barrier
    Position goalProjection = WR::Utils::projectPointAtSegment(loc()->ourGoalRightMidPost(), loc()->ourGoalLeftMidPost(), markPosition);
    Position desiredPosition = WR::Utils::threePoints(loc()->ourGoal(), markPosition, _radius, 0.0f);

    // Position to look
    Position aimPosition = WR::Utils::threePoints(loc()->ourGoal(), markPosition, 1000.0f, 0.0); // high distance (always will look)

    // Adjust _d
    if(_distanceFromGK != 0.0f){
        Position vector2Ball(true, loc()->ball().x() - desiredPosition.x(), loc()->ball().y() - desiredPosition.y(), 0.0);
        float vectorMod = sqrt(powf(vector2Ball.x(), 2) + powf(vector2Ball.y(), 2));
        vector2Ball.setPosition(vector2Ball.x()/vectorMod, vector2Ball.y()/vectorMod, 0.0);

        float ang = acos(vector2Ball.x());
        if(vector2Ball.y() < 0.0f) ang = GEARSystem::Angle::twoPi - ang;
        ang += GEARSystem::Angle::pi / 2.0f;
        WR::Utils::angleLimitZeroTwoPi(&ang);

        vector2Ball.setPosition(cos(ang) * _distanceFromGK, sin(ang) * _distanceFromGK, 0.0);

        desiredPosition.setPosition(desiredPosition.x() + vector2Ball.x(), desiredPosition.y() + vector2Ball.y(), 0.0);
    }

    // Error goal (desiredPosition sometimes goes off the field)
    if(loc()->ourSide().isRight() && desiredPosition.x() > loc()->ourGoal().x()-ERROR_GOAL_OFFSET){
        desiredPosition.setPosition(loc()->ourGoal().x()-ERROR_GOAL_OFFSET, desiredPosition.y(), 0.0);
    }else if(loc()->ourSide().isLeft() && desiredPosition.x() < loc()->ourGoal().x()+ERROR_GOAL_OFFSET){
        desiredPosition.setPosition(loc()->ourGoal().x()+ERROR_GOAL_OFFSET, desiredPosition.y(), 0.0);
    }

    // settings of goto
    _sk_goto->setDesiredPosition(desiredPosition);
    _sk_goto->setAimPosition(aimPosition);
    _sk_goto->setAvoidBall(false);
    _sk_goto->setAvoidTeammates(_avoidAllies);
    _sk_goto->setAvoidOpponents(false);
    _sk_goto->setAvoidOurGoalArea(true);

    // settings of intercept
    _sk_gk->setInterceptAdvance(true);
    _sk_gk->setPositionToLook(loc()->theirGoal());

    _sk_kick->setAim(loc()->theirGoal());
    _sk_kick->setPower(MRCConstants::_maxKickPower);
    _sk_kick->setIsChip(true);

    _sk_push->setAim(loc()->theirGoal());
    _sk_push->setDestination(desiredPosition);

    // Transitions
    if(player()->distBall() > INTERCEPT_MINBALLDIST && (isBallComing(INTERCEPT_MINBALLVELOCITY, 1.0f) || isBallComingToGoal(INTERCEPT_MINBALLDIST))) {
        enableTransition(STATE_GK);
    } else {
        if(player()->canKickBall() && player()->distBall() <= 0.4f && _canTakeout && !loc()->isInsideOurArea(loc()->ball(), 1.05f) && WR::Utils::distance(player()->position(), loc()->ourGoal()) <= (_radius + 1.5f)){
            if(!isBehindBall(loc()->theirGoal())){
                enableTransition(STATE_PUSH);
            }
            else{
                enableTransition(STATE_KICK);
            }
        }else{
            enableTransition(STATE_GOTO);
        }
    }

}

bool Behaviour_Barrier::isBallComing(float minVelocity, float radius) {
    const Position posBall = loc()->ball();
    const Position posPlayer = player()->position();

    // Check ball velocity
    if(loc()->ballVelocity().abs() < minVelocity)
        return false;

    // Angle player
    float angVel = loc()->ballVelocity().arg().value();
    float angPlayer = WR::Utils::getAngle(posBall, posPlayer);

    // Check angle difference
    float angDiff = WR::Utils::angleDiff(angVel, angPlayer);
    float angError = atan2(radius, player()->distBall());

    return (fabs(angDiff) < fabs(angError));
}

bool Behaviour_Barrier::isBallComingToGoal(float minSpeed, float postsFactor){
    // postFactor é caso queiramos alongar mais a posição y da barra (margem de erro)
    const Position posBall = loc()->ball();
    const Position posRightPost(true, loc()->ourGoalRightPost().x(), loc()->ourGoalRightPost().y()*postsFactor, 0.0);
    const Position posLeftPost(true, loc()->ourGoalLeftPost().x(), loc()->ourGoalLeftPost().y()*postsFactor, 0.0);

    // Check ball velocity
    if(loc()->ballVelocity().abs()<minSpeed)
         return false;

    // Angle ball velocity
    float angVel = loc()->ballVelocity().arg().value();
    float angRightPost = WR::Utils::getAngle(posBall, posRightPost);
    float angLeftPost = WR::Utils::getAngle(posBall, posLeftPost);
    float angDiffPosts = fabs(WR::Utils::angleDiff(angRightPost, angLeftPost));

    // Check angle difference with posts
    float angDiffRight = fabs(WR::Utils::angleDiff(angVel, angRightPost));
    float angDiffLeft = fabs(WR::Utils::angleDiff(angVel, angLeftPost));

    return (angDiffRight<angDiffPosts && angDiffLeft<angDiffPosts);
}

bool Behaviour_Barrier::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/1.5f);
}
