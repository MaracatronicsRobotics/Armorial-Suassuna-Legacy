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


#include "ballplacement.h"

Behaviour_BallPlacement::Behaviour_BallPlacement() {
    _placementPosition = Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    _reachedTarget = false;
    _reachedAlignment = false;
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
}

QString Behaviour_BallPlacement::name() {
    return "Behaviour_BallPlacement";
}

void Behaviour_BallPlacement::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);
}

void Behaviour_BallPlacement::run() {
    if(_placementPosition.isinvalid()) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("Behavior_Placement::run() has no placement position set.") + '\n';
        return ;
    }

    // Ball position
    Position ballPosition = getWorldMap()->getBall().ballposition();

//    if(getGUI() != nullptr) {
//        // Optimal area to place
//        getGUI()->fieldView()->drawCircularArc(FieldCircularArc("optimalPlacementArea", _placementPosition, getConstants()->ballRadius(), 0, 2.0*Angle::pi), QColor(255, 165, 0));

//        // Acceptable area to place
//        getGUI()->fieldView()->drawCircularArc(FieldCircularArc("acceptablePlacementArea", _placementPosition, 0.15f, 0, 2.0*Angle::pi), QColor(0, 255, 0));

//        // Area that robots may avoid
//        getGUI()->fieldView()->drawCircularArc(FieldCircularArc("avoidPlacementArea", _placementPosition, 0.5f, 0, 2.0*Angle::pi), QColor(255, 0, 0));
//    }

    if(Utils::distance(ballPosition, _placementPosition) <= getConstants()->getBallRadius()) {
        // Mark as reached target
        _reachedTarget = true;

        // Disable dribbling
        player()->playerDribble(false);

        // Avoid ball
        Position avoidPosition = Utils::repulsedPosition(ballPosition, 0.5f + getConstants()->getRobotRadius(), player()->getPlayerOrientationTo(_placementPosition));
        _skill_goTo->setTargetPosition(avoidPosition);
        _skill_rotateTo->setTargetPosition(ballPosition);
        _skill_rotateTo->setReferencePosition(player()->getPlayerPos());
    }
    else {
        if(!_reachedTarget) {
            player()->playerDribble(true);

            if(!player()->hasBallPossession()) {
                Position repulsedPosition = Utils::threePoints(ballPosition, _placementPosition, 0.2f, M_PI);
                if(!_reachedAlignment && player()->getPlayerDistanceTo(repulsedPosition) > player()->getLinearError()) {
                    _skill_goTo->setTargetPosition(repulsedPosition);
                    _skill_rotateTo->setTargetPosition(ballPosition);
                    _skill_rotateTo->setReferencePosition(repulsedPosition);

                    // Mark if reached alignment
                    _reachedAlignment = (player()->getPlayerDistanceTo(repulsedPosition) <= 1.5*player()->getLinearError());
                }
                else {
                    _skill_goTo->setTargetPosition(ballPosition);
                    _skill_rotateTo->setTargetPosition(ballPosition);
                    _skill_rotateTo->setReferencePosition(player()->getPlayerPos());
                }
            }
            else {
                Position repulsedPosition = Utils::threePoints(_placementPosition, ballPosition, getConstants()->getRobotRadius() + getConstants()->getBallRadius(), 0.0);
                _skill_goTo->setTargetPosition(repulsedPosition);
                _skill_rotateTo->setTargetPosition(_placementPosition);
                _skill_rotateTo->setReferencePosition(repulsedPosition);
            }
        }
    }

    setSkill(SKILL_GOTO);
    setSkill(SKILL_ROTATETO);
}

void Behaviour_BallPlacement::setPlacementPosition(Position placementPosition) {
    if(_placementPosition.x() != placementPosition.x() || _placementPosition.y() != placementPosition.y()) {
        _placementPosition = placementPosition;
        _reachedTarget = false;
        _reachedAlignment = false;
    }
}
