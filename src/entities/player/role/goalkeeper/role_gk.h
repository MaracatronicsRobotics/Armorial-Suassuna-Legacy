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

#ifndef ROLE_GK_H
#define ROLE_GK_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behavior/behaviors.h>
#include <src/constants/constants.h>

class Role_GK : public Role
{
public:
    Role_GK();
    QString name();

    void setDefenderEllipseCenter(Geometry::Vector2D ellipseCenter) { _ellipseCenter = ellipseCenter; }
    void setDefenderEllipseParameters(QPair<float, float> ellipseParameters) { _ellipseParameters = ellipseParameters; }

private:
    void configure();
    void run();

    enum {
        BEHAVIOR_MOVETO,
        BEHAVIOR_INTERCEPT
    };


    // Parameters
    bool _gkOverlap;
    Utils::Timer _overlapTimer;
    bool _isOverlapTimerInit;

    enum state {
        STATE_BLOCKBALL,
        STATE_PICKBALL
    };

    state _currState;

    Geometry::Vector2D _standardPos;

    Behavior_MoveTo *_behavior_moveTo;
    Behavior_Intercept *_behavior_intercept;

    // Role Parameters
    Geometry::Vector2D _ellipseCenter;
    QPair<float, float> _ellipseParameters;

    // Replacer
//    QPair<Geometry::Vector2D, Geometry::Angle> getPlacementPosition(VSSRef::Foul foul, VSSRef::Color forTeam, VSSRef::Quadrant atQuadrant);
};

#endif // ROLE_GK_H
