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

    Velocity ballVelocity = loc()->ballVelocity();

    // Check ball speed (maybe a error)
    if(ballVelocity.abs() <= 0.1)
        objectivePos = player()->position(); // manter posicao
    else{
        // Unitary velocity vector (project point at goal)
        Position posBall = loc()->ball();
        Position unitaryBallVelocity = Position(true, ballVelocity.x()/ballVelocity.abs(), ballVelocity.y()/ballVelocity.abs(), 0.0);

        // Now ball velocity line (pos + uni_velocity vector)
        Position ballVelocityLine = Position(true, posBall.x()+unitaryBallVelocity.x(), posBall.y()+unitaryBallVelocity.y(), 0.0);

        // Call utils to get projection
        objectivePos = WR::Utils::projectPointAtLine(posBall, ballVelocityLine, player()->position()); //Intercepta em 90 graus
    }

    player()->goToLookTo(objectivePos.x(), objectivePos.y(), loc()->ball().x(), loc()->ball().y(), 0.01);
}
