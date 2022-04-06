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

#include "kick.h"

#define DIST_BEHINDBALL 0.16f
#define MINDISTBALL_AVOIDROBOTS 0.4f
#define SWITCHSTATE_MINDISTBALL 0.35f

QString Kick::name() {
    return "Skill_Kick";
}

Kick::Kick(WorldMap *worldMap) {
    _worldMap = worldMap;
    _isChip = false;
    // Create a default, invalid position
    _aimPosition = Utils::getPositionObject(0.0, 0.0, 0.0, true);
    _state = STATE_KICK;
}

void Kick::run() {
    if(_aimPosition.isinvalid())
        return;

    // Local parameters
    Position desiredPos;
    bool avoidBall = false, avoidOpp = false, avoidTeam = false;
    Position ballPosition = getWorldMap()->getBall().ballposition();
    const Position behindBall = Utils::threePoints(ballPosition, _aimPosition, DIST_BEHINDBALL, M_PI, false);

    switch(_state) {
        default:
        case STATE_POS: {

            // Desired position
            desiredPos = behindBall;

            // PID hacking
            desiredPos = Utils::threePoints(desiredPos, player()->getPlayerPos(), 0.03f, M_PI, false);

            // Avoids
            avoidBall = isBehindBall(desiredPos);

            if(player()->getPlayerDistanceTo(ballPosition) > MINDISTBALL_AVOIDROBOTS) {
                avoidTeam = true;
                avoidOpp = true;
            } else {
                avoidTeam = false;
                avoidOpp = false;
            }

            // Switch state condition: is at behindBall
            float distanceToKickPosition = player()->getPlayerDistanceTo(behindBall);
            // Values from last implementation
            float error = 1.5f * 0.015f;

            if (distanceToKickPosition - error <= 0){
                _state = STATE_KICK;
            }
            // TODO validate and erase this
//            Old function (deprecated)
//            if(player()->isNearbyPosition(behindBall, 1.5*player()->lError()))
//                _state = STATE_KICK;

        } break;
        case STATE_KICK: {
            desiredPos = getWorldMap()->getBall().ballposition();
            avoidBall = false;
            avoidTeam = false;
            avoidOpp = false;

            // Switch state condition: distance to ball
            if(player()->getPlayerDistanceTo(desiredPos) > SWITCHSTATE_MINDISTBALL)
                _state = STATE_POS;

            // Switch state condition: looking to ball
            if(isBallInFront() == false)
                _state = STATE_POS;
        } break;
    }

    // goToLookTo
    Position lookPosition = Utils::threePoints(_aimPosition, getWorldMap()->getBall().ballposition(), 999.0f, M_PI, false);
    player()->playerGoTo(desiredPos);
    player()->playerRotateTo(lookPosition);
    //player()->goToLookTo(desiredPos, lookPosition, avoidTeam, avoidOpp, avoidBall, false, false);

    // Enable kick
    if(player()->isLookingTo(_aimPosition))
        player()->playerKick(_power, _isChip);
}

bool Kick::isBehindBall(Position posObjective){
    Position posBall = getWorldMap()->getBall().ballposition();
    Position posPlayer = player()->getPlayerPos();
    float anglePlayer = Utils::getAngle(posBall, posPlayer);
    float angleDest = Utils::getAngle(posBall, posObjective);
    float diff = Utils::angleDiff(anglePlayer, angleDest);

    return (diff>(M_PI/2.0f));
}

bool Kick::isBallInFront(){
    float anglePlayerBall = player()->getPlayerAngleTo(getWorldMap()->getBall().ballposition());
    float diff = Utils::angleDiff(anglePlayerBall, player()->getPlayerOrientation());

    return (diff <= atan(0.55)); // atan(0.7) aprox = 35 degree
}

bool Kick::isInFrontOfObjective(){
    float anglePlayerObj = player()->getPlayerAngleTo(_aimPosition);
    float diff = Utils::angleDiff(anglePlayerObj, player()->getPlayerOrientation());

    // TODO control this value from constants
    float angle_error = 0.0523599f; // aprox 3 graus

    return (diff <= angle_error);
}

WorldMap* Kick::getWorldMap(){
    return _worldMap;
}
