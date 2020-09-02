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

#include "behaviour_markball.h"

#define BALLPREVISION_MINVELOCITY 0.02f
#define BALLPREVISION_VELOCITY_FACTOR 3.0f
#define BALLPREVISION_FACTOR_LIMIT 0.25f

QString Behaviour_MarkBall::name() {
    return "Behaviour_MarkBall";
}

Behaviour_MarkBall::Behaviour_MarkBall() {
    _sk_GoToLookTo = NULL;
}

void Behaviour_MarkBall::configure() {
    usesSkill(_sk_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_MarkBall::run() {
    if(loc()->isInsideTheirArea(loc()->ball(), 1.1f) || loc()->isInsideOurArea(loc()->ball(), 1.1f) || !loc()->isInsideField(loc()->ball())){
        if(loc()->isInsideTheirArea(loc()->ball(), 1.1f)) _sk_GoToLookTo->setDesiredPosition(WR::Utils::threePoints(loc()->theirGoal(), loc()->ball(), 2.0f, 0.0));
        else if(loc()->isInsideOurArea(loc()->ball(), 1.1f)) _sk_GoToLookTo->setDesiredPosition(WR::Utils::threePoints(loc()->ourGoal(), loc()->ball(), 2.0f, 0.0));
        else _sk_GoToLookTo->setDesiredPosition(player()->position());
        _sk_GoToLookTo->setAimPosition(loc()->ball());
        return ;
    }

    _aimPosition.setUnknown();

    // Discover their player that have poss
    for(quint8 x = 0; x < getConstants()->getQtPlayers(); x++){
        if(PlayerBus::theirPlayerAvailable(x)){
            if(PlayerBus::theirPlayer(x)->hasBallPossession()){
                _aimPosition = PlayerBus::theirPlayer(x)->position();
                break;
            }
        }
    }

    // Calc behind ball
    Position behindBall;

    // If they don't have poss, go in the ball directly, else go to the front of the player
    if(_aimPosition.isUnknown())
        behindBall = WR::Utils::threePoints(loc()->ball(), player()->position(), 0.8f, GEARSystem::Angle::pi);
    else
        behindBall = WR::Utils::threePoints(loc()->ball(), _aimPosition, 0.4f, GEARSystem::Angle::pi);

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

        // If one player have poss (aim isn't unknown), can go to TAKE
        if(!_aimPosition.isUnknown()){
            if(player()->isNearbyPosition(behindBall, 0.1f))
                _state = STATE_TAKE;
        }

        _sk_GoToLookTo->setAvoidOpponents(true);
    }
    break;
    case STATE_TAKE: {
        // If no player have poss, just position the player
        if(_aimPosition.isUnknown())
            _state = STATE_POS;

        desiredPos = WR::Utils::threePoints(loc()->ball(), player()->position(), 0.5f, GEARSystem::Angle::pi);

        if(player()->distBall() > 1.0f){
            _state = STATE_POS;
        }

        if(isBallInFront() == false){
            _state = STATE_POS;
        }

        _sk_GoToLookTo->setAvoidOpponents(false);
    }
    break;
    }

    // Maintain kick to takeout ball from them
    player()->kick(3.0f);

    _sk_GoToLookTo->setDesiredPosition(desiredPos);
    _sk_GoToLookTo->setAvoidBall(false);
    _sk_GoToLookTo->setAimPosition(loc()->ball());

}

bool Behaviour_MarkBall::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/2.0f);
}

bool Behaviour_MarkBall::isBallInFront(){
    Angle anglePlayerBall = player()->angleTo(loc()->ball());
    float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());

    return (diff <= atan(0.7)); // atan(0.7) aprox = 35 degree
}

bool Behaviour_MarkBall::isInFrontOfObjective(){
    Angle anglePlayerObj = player()->angleTo(_aimPosition);
    float diff = WR::Utils::angleDiff(anglePlayerObj, player()->orientation());

    return (diff <= GEARSystem::Angle::toRadians(3)); // aprox 3 graus
}
