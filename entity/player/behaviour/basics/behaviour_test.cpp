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

#include "behaviour_test.h"

QString Behaviour_Test::name() {
    return "Behaviour_Test";
}

Behaviour_Test::Behaviour_Test() {
}

void Behaviour_Test::configure() {
    usesSkill(_skill_rotateTo = new Skill_RotateTo());
    usesSkill(_skill_goTo = new Skill_GoTo());
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());

    setInitialSkill(_skill_goToLookTo);
};

void Behaviour_Test::run() {
    // gtlt
    _skill_goToLookTo->setDesiredPosition(loc()->ball());
    _skill_goToLookTo->setAimPosition(loc()->ball());

    // kick
    _skill_kick->setPower(getConstants()->getMaxKickPower());
    _skill_kick->setAim(loc()->theirGoal());

    // goto
    _skill_goTo->setDesiredPosition(loc()->ball());

    // rotate
    _skill_rotateTo->setDesiredPosition(loc()->ball());
}
