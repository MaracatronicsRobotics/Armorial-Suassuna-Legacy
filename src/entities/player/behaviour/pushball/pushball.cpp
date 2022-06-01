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


#include "pushball.h"
//#include <src/utils/types/line/line.h>

Behaviour_PushBall::Behaviour_PushBall() {
    _positionToGo = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    _positionToLook = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    _possPosition = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
    _tryingToTakePoss = false;
}

QString Behaviour_PushBall::name() {
    return "Behaviour_PushBall";
}

void Behaviour_PushBall::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);
}

void Behaviour_PushBall::run() {
    if(_positionToGo.isinvalid()) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("Behaviour_PushBall::run() has no push position set.") + '\n';
        return ;
    }

    // Take ball position
    Position ballPosition = getWorldMap()->getBall().ballposition();

    // Enable dribbling
    player()->playerDribble(true);

    // If don't have possession, try to take ball
    if(!player()->hasBallPossession()) {

        setPossPosition(Utils::getPositionObject(0.0f, 0.0f, 0.0f, true));

        _skill_goTo->setTargetPosition(ballPosition);
        _skill_rotateTo->setTargetPosition(ballPosition);
        _skill_rotateTo->setReferencePosition(player()->getPlayerPos());

        _tryingToTakePoss = true;
    }
    // If has poss, try to push to desired area
    else {
        if(_tryingToTakePoss) {
            setPossPosition(ballPosition);
            _tryingToTakePoss = false;
        }

//        // GUI functions to draw movement information
//        if(getGUI() != nullptr) {
//            // Draw the maximum range
//            getGUI()->fieldView()->drawCircularArc(FieldCircularArc("MaxPushRange", _possPosition, 1.0f, 0, 2.0*Angle::pi), QColor(255, 0, 0));

//            // Draw the safe range
//            getGUI()->fieldView()->drawCircularArc(FieldCircularArc("SafePushRange", _possPosition, 0.9f, 0, 2.0*Angle::pi), QColor(0, 255, 0));
//        }

        // If distance to desired position is higher than safe range, project into the circle
        if(Utils::distance(_positionToGo, _possPosition) >= 0.9f) {
            Position projected = Utils::threePoints(_possPosition, _positionToGo, 0.9f - getConstants()->getRobotRadius(), 0.0f);
            setPositionToGo(projected);
        }

        _skill_goTo->setTargetPosition(_positionToGo);
        _skill_rotateTo->setTargetPosition(_positionToLook);
        _skill_rotateTo->setReferencePosition(_positionToGo);
    }

    setSkill(SKILL_GOTO);
    setSkill(SKILL_ROTATETO);
//    runSkill(SKILL_GOTO);
//    runSkill(SKILL_ROTATETO);
}

Position Behaviour_PushBall::getPossPosition() {
    return _possPosition;
}

float Behaviour_PushBall::distanceToPoss() {
    // If not have poss
    if(_possPosition.isinvalid() || !player()->hasBallPossession()) {
        return 0.0f;
    }

    Position ballPosition = getWorldMap()->getBall().ballposition();

    return Utils::distance(ballPosition, _possPosition);
}

void Behaviour_PushBall::setPositionToGo(Position &positionToGo) {
    _positionToGo = positionToGo;
}

void Behaviour_PushBall::setPositionToLook(Position &positionToLook) {
    _positionToLook = positionToLook;
}

void Behaviour_PushBall::setPossPosition(Position possPosition) {
    _possPosition = possPosition;
}
