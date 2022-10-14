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

#include "role_default.h"

Role_Default::Role_Default() {

}

void Role_Default::configure() {
    // Starting behaviors
    _behavior_default = new Behavior_Default();
    _behavior_moveTo = new Behavior_MoveTo();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_DEFAULT, _behavior_default);
    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);
}

void Role_Default::run() {
    _behavior_moveTo->setPosition(getWorldMap()->getBall().getPosition());
    _behavior_moveTo->keepDistance(true);
    setBehavior(BEHAVIOR_MOVETO);
}
