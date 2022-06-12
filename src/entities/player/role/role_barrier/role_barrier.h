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

#ifndef ROLE_BARRIER_H
#define ROLE_BARRIER_H

#include <src/entities/player/role/role.h>
#include <src/entities/player/behaviour/behaviours.h>

class Role_Barrier : public Role
{
public:
    Role_Barrier();
    QString name();

    void setDistanceFromGk(float distanceFromGK) { _distanceFromGK = distanceFromGK; }
    void setRadius(float radius) { _radius = radius; }
    float getRadius() { return _radius; }
    void setAvoidAllies(boool avoidAllies) { _avoidAllies = avoidAllies; }
    void setMarkPlayer(int playerID) { _markNearestPlayer = true; _markPlayerID = playerID; }
    void setMarkBall() { _markNearestPlayer = false; }
    void setCanTakeout(bool canTakeout) { _canTakeout = canTakeout; }
    void setInterceptRadius(float interceptRadius) { _interceptRadius = interceptRadius; }
    void setDesiredPosition(Position desiredPosistion) { _desiredPosition = desiredPosistion; _markBall = false; }
    void setToMarkBall(float markBall) { _markBall = markBall; }

    void initializeBehaviours();

private:
    // Role inherit methods
    void configure();
    void run();
    double _offsetBall;

    // Parameters
    float _distanceFromGK;
    float _radius;
    bool _avoidAllies;
    bool _markNearestPlayer;
    bool _canTakeout;
    bool _markBall;
    float _interceptRadius;
    Position _desiredPosition;
    int _markPlayerID;
    int _actualState;

    // Behaviours enum
    enum {
        BEHAVIOUR_GOTOLOOKTO,
        BEHAVIOUR_INTERCEPTBALL,
        BEHAVIOUR_PUSHBALL
    };

    enum {
        STATE_FOLLOWBALL,
        STATE_INTERCEPT,
        STATE_TAKEOUT,
        STATE_PENALTY
    };

    // Behaviours pointers
    Behaviour_GoToLookTo *_behaviour_goToLookTo;
//    Behaviour_InterceptBall *_behaviour_interceptBall;
//    Behaviour_PushBall *_behaviour_pushBall;

    // Attacker
    int getBestAttacker();
    bool _notAlreadyChosen;
    int _bestAttacker;

    // Aux
    bool isBallComingToGoal(float minSpeed, float postsFactor = 1.0f);
    bool isBehindBall(Position posObjective);
    bool isBallComing(float minVelocity, float radius);
};

#endif // ROLE_BARRIER_H
