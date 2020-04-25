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

#define BALLPREVISION_MINVELOCITY 0.02f
#define BALLPREVISION_VELOCITY_FACTOR 3.0f
#define BALLPREVISION_FACTOR_LIMIT 0.15f

QString Skill_Kick::name() {
    return "Skill_Kick";
}

Skill_Kick::Skill_Kick() {
    _isPass = false;
    _zPower = 0.0;
    _aimPosition = Position(false, 0.0, 0.0, 0.0);

    _aimPosition.setUnknown();

    _state = STATE_POS;
}

void Skill_Kick::run() {
    if(_aimPosition.isUnknown())
        return;

    // Calc behind ball
    Position behindBall = WR::Utils::threePoints(loc()->ball(), _aimPosition, 0.16f, GEARSystem::Angle::pi);

    if(loc()->ballVelocity().abs() > BALLPREVISION_MINVELOCITY){
        // Calc unitary vector of velocity
        const Position velUni(true, loc()->ballVelocity().x()/loc()->ballVelocity().abs(), loc()->ballVelocity().y()/loc()->ballVelocity().abs(), 0.0);

        // Calc velocity factor
        float factor = BALLPREVISION_VELOCITY_FACTOR*loc()->ballVelocity().abs();
        WR::Utils::limitValue(&factor, 0.0f, BALLPREVISION_FACTOR_LIMIT);

        // Calc projected position
        const Position delta(true, factor*velUni.x(), factor*velUni.y(), 0.0);
        Position projectedPos(true, behindBall.x()+delta.x(), behindBall.y()+delta.y(), 0.0);

        if(isBehindBall(projectedPos)){
            behindBall = projectedPos;
        }
    }

    // Local parameters
    Position desiredPos;

    switch (_state) {
    case STATE_POS: {
        desiredPos = behindBall;

        if(player()->isNearbyPosition(behindBall, 0.1f))
            _state = STATE_KICK;
    }
        break;
    case STATE_KICK: {
        desiredPos = WR::Utils::threePoints(loc()->ball(), player()->position(), 0.2f, GEARSystem::Angle::pi);

        if(player()->distBall() > 0.35f){
            _state = STATE_POS;
        }

        if(isBallInFront() == false){
            _state = STATE_POS;
        }
    }
        break;
    }

    player()->goToLookTo(desiredPos, _aimPosition, 0.01);
    player()->kick(_isPass, _zPower);
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
