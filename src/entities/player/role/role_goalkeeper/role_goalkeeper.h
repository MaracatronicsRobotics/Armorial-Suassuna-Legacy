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

#ifndef ROLE_GOALKEEPER_H
#define ROLE_GOALKEEPER_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behaviour/behaviours.h>


class Role_Goalkeeper : public Role
{
public:
    Role_Goalkeeper();
    QString name();
    void setPositionToGo(Position positionToGo);
    void setPositionToLook(Position positionToLook);
    void setRadius(float radius) {_radius = radius;}
    void setTakeoutEnabled(bool enable) {_takeoutEnabled = enable;}
    void setTakeoutFactor(float takeoutFactor) {_takeoutFactor = takeoutFactor;}
    void useAttackerOri(bool useAttackerOri) {_useAttackerOri = useAttackerOri;}

    void initializeBehaviours();

private:
    // Role inherit methods
    void configure();
    void run();
    double _offsetBall;

    // Parameters
    float _radius;
    bool _takeoutEnabled;
    float _takeoutFactor;
    bool _useAttackerOri;

    // Auxiliary
    bool isBallComingToGoal(float minSpeed, float postsFactor = 1.0);
    Position getAttackerInterceptPosition();
    Position ballProjection();
    bool isBehindBall(Position posObjective);
    bool isBallInFront();
    bool isBallAlignedToGoal(int theirPlayerId);
    Position calcAttackerBallImpact();
    void push(Position aim, bool shootWhenAligned, bool isPenalty, bool isParabolic, float kickPower);

    // Attacker (?)
    int getBestAttacker();
    bool _notAlreadyChosen;
    int _bestAttacker;

    // Behaviours
    enum {
        BEHAVIOUR_GOTOLOOKTO,
        BEHAVIOUR_INTERCEPTBALL
    };

    Behaviour_GoToLookTo *_behaviour_goToLookTo;
    // Behaviour_InterceptBall *_behaviour_interceptBall;

    // Internal
    Position _positionToGo;
    Position _positionToLook;
    bool _done;
};

#endif // ROLE_GOALKEEPER_H
