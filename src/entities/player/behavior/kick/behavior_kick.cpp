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

#include "behavior_kick.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Behavior_Kick::Behavior_Kick() {
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
}

void Behavior_Kick::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);

    // Defines initial target position
    _targetPosition = Geometry::Vector2D(0.5f, 0.5f);
}

void Behavior_Kick::run() {

}

Geometry::Vector2D Behavior_Kick::getKickPos(Geometry::Vector2D targetPos){
    Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
    Geometry::LineSegment targetLine(ballPos,targetPos);


}
