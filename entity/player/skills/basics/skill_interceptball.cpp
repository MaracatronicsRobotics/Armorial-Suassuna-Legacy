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

#include "skill_interceptball.h"
#include <entity/player/skills/skills_include.h>
#include <bits/stdc++.h>

QString Skill_InterceptBall::name() {
    return "Skill_interceptBall";
}

Skill_InterceptBall::Skill_InterceptBall() {
    setInterceptAdvance(false);
}

void Skill_InterceptBall::run() {
    /* calculating projection of ball */
    Position objectivePos; // Position where the goalkeeper should be

    const Velocity ballVelocity = loc()->ballVelocity();
    // Check ball speed (maybe a error)
    if(ballVelocity.abs() <= 0.1)
        objectivePos = loc()->ball(); // manter posicao
    else{
        // Unitary velocity vector (project point at goal)
        const Position posBall = loc()->ball();
        const Position unitaryBallVelocity = Position(true, ballVelocity.x()/ballVelocity.abs(), ballVelocity.y()/ballVelocity.abs(), 0.0);

        // Now ball velocity line (pos + uni_velocity vector)
        const Position ballVelocityLine = Position(true, posBall.x()+unitaryBallVelocity.x(), posBall.y()+unitaryBallVelocity.y(), 0.0);

        // Call utils to get projection
        objectivePos = WR::Utils::projectPointAtLine(posBall, ballVelocityLine, player()->position()); //Intercepta em 90 graus
    }

    if(_interceptAdvance)
        objectivePos = WR::Utils::threePoints(objectivePos, loc()->ball(), 0.1f, 0.0);

    player()->goTo(objectivePos, 0, true, 0.4f);
    player()->dribble(true);
}



bool Skill_InterceptBall::isBehindBall(const Position &destination, float angularPrecision) {
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();

    // Translate the position of the player to the front
    float xPlayer = posPlayer.x() + cos(player()->orientation().value())*MRCConstants::_robotRadius;
    float yPlayer = posPlayer.y() + sin(player()->orientation().value())*MRCConstants::_robotRadius;
    posPlayer.setPosition(xPlayer, yPlayer, posPlayer.z());

    // Calc
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, destination);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);
    return (diff>angularPrecision);
}
