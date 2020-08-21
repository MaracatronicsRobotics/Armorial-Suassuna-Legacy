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

#include "skill_kick.h"
#include <entity/player/skills/skills_include.h>

#define DIST_BEHINDBALL 0.16f
#define MINDISTBALL_AVOIDROBOTS 0.4f
#define SWITCHSTATE_MINDISTBALL 0.35f

QString Skill_Kick::name() {
    return "Skill_Kick";
}

Skill_Kick::Skill_Kick() {
    _isChip = false;
    _aimPosition = Position(false, 0.0, 0.0, 0.0);

    _aimPosition.setUnknown();

    _state = STATE_KICK;
}

void Skill_Kick::run() {
    if(_aimPosition.isUnknown())
        return;

    // Local parameters
    Position desiredPos;
    bool avoidBall = false, avoidOpp = false, avoidTeam = false;

    const Position behindBall = WR::Utils::threePoints(loc()->ball(), _aimPosition, DIST_BEHINDBALL, GEARSystem::Angle::pi);

    switch(_state) {
        default:
        case STATE_POS: {

            // Desired position
            desiredPos = behindBall;

            // PID hacking
            desiredPos = WR::Utils::threePoints(desiredPos, player()->position(), 0.03f, GEARSystem::Angle::pi);

            // Avoids
            avoidBall = isBehindBall(desiredPos);
            if(player()->distBall() > MINDISTBALL_AVOIDROBOTS) {
                avoidTeam = true;
                avoidOpp = true;
            } else {
                avoidTeam = false;
                avoidOpp = false;
            }

            // Switch state condition: is at behindBall
            if(player()->isNearbyPosition(behindBall, 1.5*player()->lError()))
                _state = STATE_KICK;

        } break;
        case STATE_KICK: {
            desiredPos = loc()->ball();
            avoidBall = false;
            avoidTeam = false;
            avoidOpp = false;

            // Switch state condition: distance to ball
            if(player()->distBall() > SWITCHSTATE_MINDISTBALL)
                _state = STATE_POS;

            // Switch state condition: looking to ball
            if(isBallInFront() == false)
                _state = STATE_POS;
        } break;
    }

    // goToLookTo
    Position lookPosition = WR::Utils::threePoints(_aimPosition, loc()->ball(), 999.0f, GEARSystem::Angle::pi);
    player()->goToLookTo(desiredPos, lookPosition, avoidTeam, avoidOpp, avoidBall, false, false);

    // Enable kick
    if(player()->isLookingTo(_aimPosition))
        player()->kick(_power, _isChip);
}

bool Skill_Kick::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/2.0f);
}

bool Skill_Kick::isBallInFront(){
    Angle anglePlayerBall = player()->angleTo(loc()->ball());
    float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());

    return (diff <= atan(0.7)); // atan(0.7) aprox = 35 degree
}

bool Skill_Kick::isInFrontOfObjective(){
    Angle anglePlayerObj = player()->angleTo(_aimPosition);
    float diff = WR::Utils::angleDiff(anglePlayerObj, player()->orientation());

    return (diff <= GEARSystem::Angle::toRadians(3)); // aprox 3 graus
}
