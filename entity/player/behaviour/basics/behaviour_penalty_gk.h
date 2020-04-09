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

#ifndef BEHAVIOUR_PENALTY_GK_H
#define BEHAVIOUR_PENALTY_GK_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Penalty_GK : public Behaviour {
private:
    void configure();
    void run();
    Skill_InterceptBall *_skill_Goalkeeper;
    Skill_GoToLookTo *_skill_goToLookTo;
    double _offsetBall;

    // Parameters
    float _radius;
    bool _takeoutEnabled;
    float _takeoutFactor;
    bool _useAttackerOri;

    // Auxiliary
    Position getAttackerInterceptPosition();
    Position calcAttackerBallImpact();
    bool isBallComingToGoal(float minSpeed, float postsFactor = 1.0);

    // Skill transitions
    enum{
        STATE_GOTO,
        STATE_KICK,
        STATE_GK
    };


public:
    Behaviour_Penalty_GK();
    QString name();

    void setRadius(float radius) { _radius = radius; }
    void setTakeoutEnabled(bool enable) { _takeoutEnabled = enable; }
    void setTakeoutFactor(float takeoutFactor) { _takeoutFactor = takeoutFactor; }
    void useAttackerOri(bool useAttackerOri) { _useAttackerOri = useAttackerOri; }
};

#endif // BEHAVIOUR_PENALTY_GK_H
