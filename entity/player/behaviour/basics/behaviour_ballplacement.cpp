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

#include "behaviour_ballplacement.h"

QString Behaviour_BallPlacement::name() {
    return "Behaviour_BallPlacement";
}

Behaviour_BallPlacement::Behaviour_BallPlacement() {
    _desiredPosition = Position(false, 0.0, 0.0, 0.0);
}

void Behaviour_BallPlacement::configure() {
    usesSkill(_skill_doNothing = new Skill_Test());
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());

    addTransition(STATE_GOTO, _skill_doNothing, _skill_goToLookTo);
    addTransition(STATE_PUSH, _skill_goToLookTo, _skill_doNothing);

    setInitialSkill(_skill_doNothing);
};

void Behaviour_BallPlacement::run() {
    if(_desiredPosition.isUnknown()){
        std::cout << getConstants()->red << "[ERROR] " << getConstants()->reset << getConstants()->defaultBold << "Behaviour_Placement has no destination set.\n" << getConstants()->reset;
        return;
    }

    if(WR::Utils::distance(loc()->ball(), _desiredPosition) <= getConstants()->getBallRadius()){
        player()->dribble(false);
        placedSuccess = true;
        Position desiredPosition;
        // first avoid ball
        if(player()->distBall() < 0.3f && !avoidedPos){
            desiredPosition = WR::Utils::threePoints(player()->position(), loc()->ball(), 0.5f, GEARSystem::Angle::pi);
        }
        else{
            avoidedPos = true;
            desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.6f, GEARSystem::Angle::pi);
        }

        _skill_goToLookTo->setDesiredPosition(desiredPosition);
        _skill_goToLookTo->setAimPosition(loc()->ball());
        _skill_goToLookTo->setAvoidBall(true);
        _skill_goToLookTo->setAvoidOpponents(true);
        _skill_goToLookTo->setAvoidTeammates(true);
        enableTransition(STATE_GOTO);
    }
    else{
        _skill_doNothing->setMaxPushDistance(999.0f);
        _skill_doNothing->setDestination(_desiredPosition);
        _skill_doNothing->setAim(_desiredPosition);
        if(!placedSuccess) enableTransition(STATE_PUSH);
    }
}

void Behaviour_BallPlacement::setDesiredPosition(const Position pos){
    _desiredPosition = pos;
    avoidedPos       = false;
    placedSuccess    = false;
}
