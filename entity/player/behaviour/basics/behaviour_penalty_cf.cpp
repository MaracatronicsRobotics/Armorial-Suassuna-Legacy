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
}

void Behaviour_Penalty_CF::configure() {
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());
    setInitialSkill(_skill_goToLookTo);

    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_GOTO, _skill_kick, _skill_goToLookTo);
}

void Behaviour_Penalty_CF::run() {

    _skill_kick->setIsPass(false);

    /*const float minAngle = WR::Utils::getAngle(loc()->ball(), loc()->ourGoalRightPost());
    const float maxAngle = WR::Utils::getAngle(loc()->ball(), loc()->ourGoalLeftPost());

    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(player()->position(), minAngle, maxAngle, false);
    QList<FreeAngles::Interval>::iterator it;

    for(it = freeAngles.begin(); it != freeAngles.end(); it++){
        if(it->obstructed()) continue;
        float initAngle = it->angInitial();
        float endAngle = it->angFinal();
        WR::Utils::angleLimitZeroTwoPi(&initAngle);
        WR::Utils::angleLimitZeroTwoPi(&endAngle);

        float dif = endAngle - initAngle;
        WR::Utils::angleLimitZeroTwoPi(&dif);
        if(dif > largestAngle){
            largestAngle = dif;
            largestMid = endAngle - dif/2;
        }
    }*/

    //Position behindBall = WR::Utils::threePoints(loc()->ball(), _kickPosition, 0.2, GEARSystem::Angle::pi); // por trás da bola
    Position Aim = getBestKickPosition();

    _skill_goToLookTo->setAimPosition(Aim);
    _skill_kick->setAim(Aim);
    _skill_goToLookTo->setOffsetToBall(0.2f);
    _skill_goToLookTo->setDesiredPosition(loc()->ball());
    loc()->theirGoalLeftPost().y();
    if (player()->distBall() < 0.2f) {
        enableTransition(STATE_KICK); //distBall deve ser trocado de acordo com o offset
    }
    else enableTransition(STATE_GOTO);
}

Position Behaviour_Penalty_CF::getBestKickPosition(){
    const Position goalRightPost = loc()->ourGoalRightPost();
    const Position goalLeftPost = loc()->ourGoalLeftPost();
    const Position goalCenter = loc()->ourGoal();

    // calculating angles
    const float minAngle = WR::Utils::getAngle(loc()->ball(), goalRightPost);
    const float maxAngle = WR::Utils::getAngle(loc()->ball(), goalLeftPost);

    // generating list of freeAngles to goal
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(loc()->ball(), minAngle, maxAngle);

    float largestAngle, largestMid;
    // get the largest interval
    if(freeAngles.size() == 0){
        return Position(false, 0.0, 0.0, 0.0); // debugar isso dps
    }else{
        QList<FreeAngles::Interval>::iterator it;
        for(it = freeAngles.begin(); it != freeAngles.end(); it++){
            if(it->obstructed()) continue;
            float initAngle = it->angInitial();
            float endAngle = it->angFinal();
            WR::Utils::angleLimitZeroTwoPi(&initAngle);
            WR::Utils::angleLimitZeroTwoPi(&endAngle);

            float dif = endAngle - initAngle;
            WR::Utils::angleLimitZeroTwoPi(&dif);
            if(dif > largestAngle){
                largestAngle = dif;
                largestMid = endAngle - dif/2;
            }
        }
    }

    // Triangularization
    const float x = goalCenter.x() - loc()->ball().x();
    const float tg = tan(largestMid);
    const float y = tg * x;

    // Impact point
    const float pos_y = loc()->ball().y() + y;
    const Position impactPosition(true, goalCenter.x(), pos_y, 0.0);

    // Check if impact position has space for ball radius
    const float distImpactPos = WR::Utils::distance(loc()->ball(), impactPosition);
    const float radiusAngle = largestAngle/2.0;
    const float distR = radiusAngle * distImpactPos;

    if(distR < (1.5 * 0.025)){ // 1.5 * raioDaBola (ruido ft. tristeza)
        return Position(false, 0.0, 0.0, 0.0); // bola n passa, debugar isso dps
    }

    return impactPosition;
}
