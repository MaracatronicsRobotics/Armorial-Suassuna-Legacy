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

#include "behaviour_timeout.h"

QString Behaviour_TimeOut::name() {
    return "Behaviour_FollowBall";
}

Behaviour_TimeOut::Behaviour_TimeOut() {
    _skill_GoToLookTo = NULL;
    _offsetBall = 0.01;
    _desiredPosition = Position(true, 0.0, 0.0, 0.0);
}

void Behaviour_TimeOut::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_TimeOut::run() {
    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
    _skill_GoToLookTo->setOffsetToBall(_offsetBall);
}

void Behaviour_TimeOut::setSkillPosition(Position pos){
    _desiredPosition = pos;
}
