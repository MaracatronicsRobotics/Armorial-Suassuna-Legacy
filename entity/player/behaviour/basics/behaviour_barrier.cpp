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
#define ERROR_GOAL_OFFSET 0.3f

QString Behaviour_Barrier::name() {
    return "Behaviour_Barrier";
}

Behaviour_Barrier::Behaviour_Barrier() {
    setD(0.2);
    setRadius(1.4); // radius from our goal center

    _sk_goto = NULL;
    _sk_gk = NULL;
    _sk_kick = NULL;
}

void Behaviour_Barrier::configure() {
    usesSkill(_sk_goto = new Skill_GoToLookTo());
    usesSkill(_sk_gk = new Skill_InterceptBall());
    usesSkill(_sk_kick = new Skill_Kick());

    // Setting initial skill
    setInitialSkill(_sk_goto);

    // Transitions
    addTransition(STATE_GOTO, _sk_kick, _sk_goto);
    addTransition(STATE_GOTO, _sk_gk, _sk_goto);

    addTransition(STATE_GK, _sk_goto, _sk_gk);
    addTransition(STATE_GK, _sk_kick, _sk_gk);

    addTransition(STATE_KICK, _sk_goto, _sk_kick);
    addTransition(STATE_KICK, _sk_gk, _sk_kick);
};

void Behaviour_Barrier::run() {
    // Pos barrier
    Position goalProjection = WR::Utils::projectPointAtSegment(loc()->ourGoalRightMidPost(), loc()->ourGoalLeftMidPost(), loc()->ball());
    Position desiredPosition = WR::Utils::threePoints(goalProjection, loc()->ball(), _radius, 0.0f);

    // Position to look
    Position aimPosition = WR::Utils::threePoints(loc()->ourGoal(), loc()->ball(), 1000.0f, 0.0); // high distance (always will look)

    // Error goal (desiredPosition sometimes goes off the field)
    if(loc()->ourSide().isRight() && desiredPosition.x() > loc()->ourGoal().x()-ERROR_GOAL_OFFSET){
        desiredPosition.setPosition(loc()->ourGoal().x()-ERROR_GOAL_OFFSET, desiredPosition.y(), 0.0);
    }else if(loc()->ourSide().isLeft() && desiredPosition.x() < loc()->ourGoal().x()+ERROR_GOAL_OFFSET){
        desiredPosition.setPosition(loc()->ourGoal().x()+ERROR_GOAL_OFFSET, desiredPosition.y(), 0.0);
    }

    // settings of goto
    _sk_goto->setDesiredPosition(desiredPosition);
    _sk_goto->setAimPosition(aimPosition);

    // settings of intercept
    _sk_gk->setInterceptAdvance(true);
    _sk_gk->setPositionToLook(loc()->theirGoal());

    _sk_kick->setAim(loc()->theirGoal());
    _sk_kick->setIsPass(false);

    // Transitions
    if(player()->distBall() > INTERCEPT_MINBALLDIST && isBallComingToGoal(INTERCEPT_MINBALLVELOCITY)) {
        enableTransition(STATE_GK);
    } else {
        if(player()->distBall() <= 0.2){
            enableTransition(STATE_KICK);
        }else{
            enableTransition(STATE_GOTO);
        }
    }

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
