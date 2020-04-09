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

#ifndef BEHAVIOUR_BARRIER_H
#define BEHAVIOUR_BARRIER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Barrier : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    // Param
    float _d;
    float _radius;

    // Sk
    Skill_GoToLookTo *_sk_goto;
    Skill_InterceptBall *_sk_gk;
    Skill_Kick *_sk_kick;

    // Transitions
    enum{
        STATE_GOTO,
        STATE_GK,
        STATE_KICK
    };

    bool isBallComingToGoal(float minSpeed, float postsFactor = 1.0);

public:
    Behaviour_Barrier();
    QString name();

    void setD(float d) { _d = d; }
    void setRadius(float radius) { _radius = radius; }
};

#endif // BEHAVIOUR_BARRIER_H
