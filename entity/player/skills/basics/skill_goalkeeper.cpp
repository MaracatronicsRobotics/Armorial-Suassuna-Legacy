#include "skill_goalkeeper.h"
#include <entity/player/skills/skills_include.h>
#include <bits/stdc++.h>

QString Skill_GoalKeeper::name() {
    return "Skill_GoalKeeper";
}

Skill_GoalKeeper::Skill_GoalKeeper() {
    setInterceptAdvance(false);
}

void Skill_GoalKeeper::run() {
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
        objectivePos = WR::Utils::projectPointAtLine(posBall, ballVelocityLine, player()->position());
    }

    player()->goToLookTo(player()->position().x(), player()->position().y(), objectivePos.x(), objectivePos.y(), loc()->ball().x(), loc()->ball().y(), player()->orientation().value(), 0.1);
}
