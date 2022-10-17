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

#include "role_goalkeeper.h"

#include <spdlog/spdlog.h>

Role_Goalkeeper::Role_Goalkeeper() {

}

void Role_Goalkeeper::configure() {
    // Starting behaviors
    _behavior_moveto = new Behavior_MoveTo();
    _behavior_catch = new Behavior_Catch();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_MOVETO, _behavior_moveto);
    addBehavior(BEHAVIOR_CATCH, _behavior_catch);
}

void Role_Goalkeeper::run() {
    setBehavior(BEHAVIOR_CATCH);
}
