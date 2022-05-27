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

#include <src/entities/player/behaviour/gotolookto/gotolookto.h>

Behaviour_GoToLookTo::Behaviour_GoToLookTo() {
    _positionToGo = Utils::getPositionObject(0.0f, 0.0f, 0.0f, false);
    _positionToLook = Utils::getPositionObject(0.0f, 0.0f, 0.0f, false);
    setUseToGoAsReference(true);
}

QString Behaviour_GoToLookTo::name() {
    return "Behaviour_GoToLookTo";
}

void Behaviour_GoToLookTo::setPositionToGo(Position &positionToGo) {
    _positionToGo = positionToGo;
}

void Behaviour_GoToLookTo::setPositionToLook(Position &positionToLook) {
    _positionToLook = positionToLook;
}

void Behaviour_GoToLookTo::setUseToGoAsReference(bool useToGoAsReference) {
    _useToGoAsReference = useToGoAsReference;
}

void Behaviour_GoToLookTo::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);
}

void Behaviour_GoToLookTo::run() {
    if(!_positionToGo.isinvalid()) {
        _skill_goTo->setTargetPosition(_positionToGo);
    }
    else {
        Position playerPosition = player()->getPlayerPos();
        _skill_goTo->setTargetPosition(playerPosition);
    }
    setSkill(SKILL_GOTO);

    if(!_positionToLook.isinvalid()) {
        // Adjusting angular P Dinamically
        if(!_positionToGo.isinvalid()) {
            float maxDistPossible = sqrt(pow(loc()->fieldWidth(), 2) + pow(loc()->fieldLength(), 2));
            float prop = std::max(0.2f, (maxDistPossible - player()->getPlayerDistanceTo(_positionToGo)) / maxDistPossible);
            /// TODO: pid
            //player()->setAngularP(std::get<0>(getConstants()->playerAngularPID()) * prop);
        }

        _skill_rotateTo->setReferencePosition(_useToGoAsReference ? _positionToGo : player()->getPlayerPos());
        _skill_rotateTo->setTargetPosition(_positionToLook);
        setSkill(SKILL_ROTATETO);
    }
}
