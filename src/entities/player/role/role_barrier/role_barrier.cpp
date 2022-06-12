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

#include "role_barrier.h"

#define INTERCEPT_MINBALLVELOCITY 0.2f
#define INTERCEPT_MINBALLDIST 0.5f
#define INTERCEPT_MIN_SPEED 0.5f
#define ERROR_GOAL_OFFSET 0.15f
#define RECEIVER_INVALID_ID 200
#define ROBOT_RADIUS 0.11f

QString Role_Barrier::name() {
    return "Role_Barrier";
}

Role_Barrier::Role_Barrier() {
    setMarkBall();
    setDistanceFromGk(0.0f); // distance from our gk line to ball
    setRadius(1.6f); // radius from our goal center

    _behaviour_goToLookTo = nullptr;
//    _behaviour_interceptBall = nullptr;
//    _behaviour_pushBall = nullptr;

    _actualState = 10;
    _canTakeout = false;
    _avoidAllies = false;
    _markBall = true;
    _interceptRadius = 0.5f;
}

void Role_Barrier::initializeBehaviours() {
    addBehaviour(BEHAVIOUR_GOTOLOOKTO, _behaviour_goToLookTo = new Behaviour_GoToLookTo());
    //    addBehaviour(BEHAVIOUR_INTERCEPTBALL, _behaviour_interceptBall = new Behaviour_InterceptBall());
    //    addBehaviour(BEHAVIOUR_PUSHBALL, _behaviour_pushBall = new Behaviour_PushBall());
}

void Role_Barrier::configure() {
    _notAlreadyChosen = true;

    setBehaviour(BEHAVIOUR_GOTOLOOKTO);
    _actualState = STATE_FOLLOWBALL;
}

void Role_Barrier::run() {
    if (getReferee()->getGameInfo()->theirPenaltyKick()) {
        _actualState = STATE_PENALTY;
    }

    Ball ball = getWorldMap()->getBall();
    Position ballPos = getWorldMap()->getBall().ballposition();
    Position playerPos = player()->getPlayerPos();
    Color ourColor = getConstants()->teamColor;
    Color theirColor = Utils::getColorObject(!ourColor.isblue());
    QList<int> ourTeamPlayers = getWorldMap()->getRobotsIDs(ourColor);
    QList<int> theirTeamPlayers = getWorldMap()->getRobotsIDs(theirColor);

    if (!player()->hasBallPossession() && Utils::getVelocityAbs(ball.ballvelocity()) >= 0.2f) {
        _notAlreadyChosen = true;
    }

    bool weHaveTheBall = false;
    for (auto p : ourTeamPlayers) {
        Player *currentPlayer = new Player(p, getWorldMap(), getReferee(), getConstants());
        if (currentPlayer->hasBallPossession()) {
            weHaveTheBall = true;
        }
    }

    bool theyHaveTheBall = false;
    bool theyAreInTheirField = true;
    bool iAmNearer = false;
    float nearestOpponentDist = 1000.0f;
    int nearestOpponent;
    if (!weHaveTheBall) {
        for (auto p: theirTeamPlayers) {
            if(Utils::distance(getWorldMap()->getPlayer(theirColor,p).getPosition(),ballPos) <= ROBOT_RADIUS){
                theyHaveTheBall = true;
            }
            if (!loc()->isInsideTheirField(getWorldMap()->getPlayer(theirColor,p).getPosition())) {
                theyAreInTheirField = false;
            }
            if (Utils::distance(getWorldMap()->getPlayer(theirColor, p).getPlayerPos(), ballPos) < nearestOpponentDist) {
                nearestOpponentDist = Utils::distance(getWorldMap()->getPlayer(theirColor, p).getPlayerPos(), ballPos);
                nearestOpponent = p;
                iAmNearer = Utils::distance(playerPos, ballPos) < nearestOpponentDist;
            }
        }
    }


    if (isBallComing(INTERCEPT_MINBALLVELOCITY, 1.0f) && (!theyHaveTheBall) && iAmNearer) {
        _canTakeout = true;
    }

    if (Utils::distance(player()->getPlayerPos(), ballPos) > INTERCEPT_MINBALLDIST || Utils::isInsideOurField(ballPos)) {
        _markBall = false;
    }

    if (getReferee()->getGameInfo()->canMove()) {
        _canTakeout = false;
        _markBall = true;
        _actualState = STATE_FOLLOWBALL;
    }

    switch (_actualState) {
    case STATE_FOLLOWBALL: {
        if (_canTakeout) {
            _actualState = STATE_TAKEOUT;
        } else {
            if (_markBall) {
                _desiredPosition = Utils::threePoints(getLocations()->ourGoal(), ballPos, _radius, 0.0f);
            }
            _behaviour_goToLookTo->setPositionToGo(_desiredPosition);
            _behaviour_goToLookTo->setPositionToLook(ballPos);
            _behaviour_goToLookTo->setReferencePosition(playerPos);
            setBehaviour(BEHAVIOUR_GOTOLOOKTO);
        }
    } break;
    case STATE_INTERCEPT: {
        if (!isBallComing(INTERCEPT_MIN_SPEED, _interceptRadius)) {
            if (Utils::isInsideOurField(ballPos) && (!weHaveTheBall) && (!theyHaveTheBall)) {
                _actualState = STATE_TAKEOUT;
            } else {
                _actualState = STATE_FOLLOWBALL;
            }
        }

//        _behaviour_interceptBall->setPosToLook(ballPos);
//        _behaviour_interceptBall->setSpeedFactor(2.5f);
        setBehaviour(BEHAVIOUR_INTERCEPTBALL);
    } break;
    case STATE_TAKEOUT: {
        if (!_canTakeout) {
            _actualState = STATE_FOLLOWBALL;
        } else if (isBallComing(INTERCEPT_MINBALLVELOCITY, _interceptRadius)) {
            _actualState = STATE_INTERCEPT;
        } else {
            if ((Utils::distance(player()->getPlayerPos(), ballPos) <= 2.0f)
                    && (!(Utils::isInsideOurArea(ballPos, 1.05f)))
                    && (Utils::distance(player()->getPlayerPos(), getLocations()->ourGoal()) <= (_radius + 1.5f))) {
                //A bola esta proxima do player
                int bestAttacker = getBestAttacker();

                if(bestAttacker != RECEIVER_INVALID_ID){
                    //Aqui tem algum aliado sem marcacao
                    Position bestAttackerPos = getWorldMap()->getPlayer(ourColor, bestAttacker).getPlayerPos();
                    _desiredPosition = bestAttackerPos;
                } else {
                    _desiredPosition = getLocations()->theirGoal();
                }

                if (!player()->hasBallPossession()) {
                    _behaviour_goToLookTo->setPositionToGo(ballPos);
                    _behaviour_goToLookTo->setPositionToLook(ballPos);
                    _behaviour_goToLookTo->setReferencePosition(playerPos);

                    setBehaviour(BEHAVIOUR_GOTOLOOKTO);
                } else {
                    _behaviour_goToLookTo->setPositionToGo(playerPos);
                    _behaviour_goToLookTo->setPositionToLook(_desiredPosition);
                    _behaviour_goToLookTo->setReferencePosition(playerPos);

                    if (player()->isSufficientlyAlignedTo(fieldCenter)) {
                        player()->playerKick(getConstants()->maxChipKickPower(), true);
                    }
                }
                player()->playerDribble(true);
            } else {
                _actualState = STATE_FOLLOWBALL;
            }
        }
    } break;
    case STATE_PENALTY: {
        int i = player()->getPlayerID();
        if (getWorldMap()->getLocations()->ourSide().isRight()) {
            if (i % 2 == 0) {
                _desiredPosition = Utils::getPositionObject(3.0f - (i/2), 1.5f);
            } else {
                _desiredPosition = Utils::getPositionObject(3.0f - (i/2), -1.5f);
            }
        } else {
            if (i % 2 == 0) {
                _desiredPosition = Utils::getPositionObject(-3.0f + (i/2), -1.5f);
            } else {
                _desiredPosition = Utils::getPositionObject(-3.0f + (i/2), 1.5f);
            }
        }
        _behaviour_goToLookTo->setPositionToGo(_desiredPosition);
        _behaviour_goToLookTo->setPositionToLook(ballPos);
        _behaviour_goToLookTo->setReferencePosition(playerPos);
        setBehavior(BEHAVIOUR_GOTOLOOKTO);
    } break;
    }
}
