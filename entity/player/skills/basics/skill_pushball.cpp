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

#include "skill_pushball.h"
#include <entity/player/skills/skills_include.h>

#define BALL_MINDIST 0.12f

#define BALLPREVISION_MINVELOCITY 0.02f
#define BALLPREVISION_VELOCITY_FACTOR 3.0f
#define BALLPREVISION_FACTOR_LIMIT 0.3f

QString Skill_PushBall::name() {
    return "Skill_PushBall";
}

Skill_PushBall::Skill_PushBall() {
    _destination.setUnknown();
    _currPos.setUnknown();
    _lastPos.setUnknown();
    _state = STATE_POS;

    shootWhenAligned(false);
    setIsParabolic(false);
    setMaxPushDistance(1.0);
    setKickPower(6.0);

    setIsPenalty(false);

}

void Skill_PushBall::run(){
    if(_aim.isUnknown()){
        std::cout << MRCConstants::yellow << "[WARNING] " << MRCConstants::reset << name().toStdString() << ": aim not set!\n";
    }

    // Calc behind ball
    Position behindBall = WR::Utils::threePoints(loc()->ball(), player()->position(), _isPenalty ? 0.0f : 0.25f, GEARSystem::Angle::pi);

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

    player()->dribble(true);

    switch(_state){
    case STATE_POS:{
        _currPos.setUnknown();
        _pushedDistance = 0.0;

        if(player()->distBall() < BALL_MINDIST && isBallInFront())
            _state = STATE_PUSH;
        else
            player()->goToLookTo(behindBall, loc()->ball(), true, true, false, false, false);
    }
    break;
    case STATE_PUSH:{
        if(_destination.isUnknown())
            player()->rotateTo(_aim);
        else{
            if(_currPos.isUnknown()){
                _currPos = loc()->ball();
                _pushedDistance = 0.0;
            }
            _lastPos = _currPos;
            _currPos = loc()->ball();

            _pushedDistance += WR::Utils::distance(_lastPos, _currPos);

            if(_pushedDistance >= 0.8 * _maxPushDistance)
                _destination.setUnknown();
            else
                player()->goToLookTo(_destination, _aim, true, true, false, false, false);
        }

        if(_shootWhenAligned){
            double angleToObjective = fabs(player()->getPlayerRotateAngleTo(_aim));
            if(angleToObjective < 1.5 * player()->aError()){
                player()->dribble(false);
                player()->kick(_kickPower, _isParabolic);
            }
        }

        if(player()->distBall() > (BALL_MINDIST + 0.04f) || !isBallInFront())
            _state = STATE_POS;
    }
    break;
    }
}

bool Skill_PushBall::isBallInFront(){
    Angle anglePlayerBall = player()->angleTo(loc()->ball());
    float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());

    return (diff <= atan(0.7)); // atan(0.7) aprox = 35 degree
}

bool Skill_PushBall::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/2.0f);
}
