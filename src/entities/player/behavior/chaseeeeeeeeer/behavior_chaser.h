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

#ifndef BEHAVIOR_CHASER_H
#define BEHAVIOR_CHASER_H

#include <src/entities/player/behavior/behavior.h>
#include <src/entities/player/skill/skills.h>
#include <Armorial/Geometry/Arc/Arc.h>
#include "math.h"


class Behavior_Chaser : public Behavior
{
public:
    Behavior_Chaser();
    QString name() { return "Behavior_CHASEEEEEEER!!!!"; }

    void setChase(Geometry::Vector2D chasePos) { _chasePos = chasePos; }
    void toCharge(bool charge) { _CHAAAAAAAAAAAAAAAAAAARGE = charge; }
    bool hammerTime() { return _CHAAAAAAAAAAAAAAAAAAARGE; }

private:
    void configure();
    void run();

    Geometry::Vector2D getChasePosition();
    bool inDangerZone(float alignementError);
    bool inDangerZoneOld();

    enum {
        SKILL_GOTO,
        SKILL_CHARGE
    };

    Skill_GoTo *_skill_goTo;
    Skill_Charge *_skill_charge;

    Geometry::Vector2D _chasePos;
    bool _CHAAAAAAAAAAAAAAAAAAARGE;
};

#endif // BEHAVIOR_CHASER_H
