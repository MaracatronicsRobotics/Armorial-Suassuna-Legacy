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
    Color ourColor = getConstants()->teamColor();
    Color theirColor = Utils::getColorObject(!ourColor.isblue());
    QList<int> ourTeamPlayers = getWorldMap()->getRobotsIDs(ourColor);
    QList<int> theirTeamPlayers = getWorldMap()->getRobotsIDs(theirColor);

    if (!player()->hasBallPossession() && Utils::getVelocityAbs(ball.ballvelocity()) >= 0.2f) {
        _notAlreadyChosen = true;
    }

    // Improvement: This ball possesion check to be in a Higher Control Level, and available to every Role
    bool weHaveTheBall = false;
    for (auto p : ourTeamPlayers) {
        Player *currentPlayer = new Player(p, getWorldMap(), getReferee(), getConstants());
        if (currentPlayer->hasBallPossession()) {
            weHaveTheBall = true;
        }
    }

    bool theyHaveTheBall = false;
    bool iAmNearer = false;
    float nearestOpponentDist = 1000.0f;
    if (!weHaveTheBall) {
        Position advPos;
        for (auto p: theirTeamPlayers) {
            advPos = getWorldMap()->getRobot(theirColor, p).robotposition();
            if(Utils::distance(advPos, ballPos) <= ROBOT_RADIUS){
                theyHaveTheBall = true;
            }
            if (Utils::distance(advPos, ballPos) < nearestOpponentDist) {
                nearestOpponentDist = Utils::distance(advPos, ballPos);
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
                    Position bestAttackerPos = getWorldMap()->getRobot(ourColor, bestAttacker).robotposition();
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

                    if (player()->isSufficientlyAlignedTo(getLocations()->fieldCenter())) {
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
        setBehaviour(BEHAVIOUR_GOTOLOOKTO);
    } break;
    }
}

int Role_Barrier::getBestAttacker() {
    if (!_notAlreadyChosen) {
        return _bestAttacker;
    } else {
        int bestID = RECEIVER_INVALID_ID;
        QList<int> attackers;
        QList<int> opPlayers;

        if (getConstants()->isTeamBlue()) {
            attackers = getWorldMap()->getRobotsIDs(Utils::getColorObject(getConstants()->isTeamBlue()));
            opPlayers = getWorldMap()->getRobotsIDs(Utils::getColorObject(getConstants()->isTeamYellow()));
        } else {
            attackers = getWorldMap()->getRobotsIDs(Utils::getColorObject(getConstants()->isTeamYellow()));
            opPlayers = getWorldMap()->getRobotsIDs(Utils::getColorObject(getConstants()->isTeamBlue()));
        }

        float menDist = 0;
        for (int i = 0; i < attackers.size(); i++) {
            // Not sure if it's the best approach. Should we clean these pointers?
            Player *p = new Player(attackers.at(i), getWorldMap(), getReferee(), getConstants());
            Position playerPos = (*p).getPlayerPos();
            if (Utils::distance(playerPos, getLocations()->ourGoal()) > 2.0
                    && (*p).getPlayerID() != player()->getPlayerID()) {

                Position recPos = (*p).getPlayerPos();
                float menDistPlayer = 1000;
                for (int j = 0; j < opPlayers.size(); j++) {
                    Player *opPlayer = new Player(opPlayers.at(j), getWorldMap(), getReferee(), getConstants());
                    if ((*opPlayer).isEnabled()) {
                        Position opPos = (*opPlayer).getPlayerPos();
                        float distPlayer = Utils::distance(recPos, opPos);
                        if (distPlayer < menDistPlayer) {
                            menDistPlayer = distPlayer;
                        }
                    }
                }
                if (menDistPlayer > menDist) {
                    menDist = menDistPlayer;
                    bestID = attackers.at(i);
                }
            }
        }
        _notAlreadyChosen = false;
        _bestAttacker = bestID;

        return bestID;
    }
}

bool Role_Barrier::isBallComingToGoal(float minSpeed, float postsFactor) {
    const Position posBall = getWorldMap()->getBall().ballposition();
    const Position posRightPost = Utils::getPositionObject(getLocations()->ourGoalRightPost().x(), getLocations()->ourGoalRightPost().y()*postsFactor);
    const Position posLeftPost = Utils::getPositionObject(getLocations()->ourGoalLeftPost().x(), getLocations()->ourGoalLeftPost().y()*postsFactor);

    float velBall = Utils::getVelocityAbs(getWorldMap()->getBall().ballvelocity());
    if (velBall < minSpeed) {
        return false;
    }
    float angVel = Utils::getVelocityArg(getWorldMap()->getBall().ballvelocity());
    float angRightPost = Utils::getAngle(posBall, posRightPost);
    float angLeftPost = Utils::getAngle(posBall, posLeftPost);
    float angDiffPosts = fabs(Utils::angleDiff(angRightPost, angLeftPost));

    float angDiffRight = fabs(Utils::angleDiff(angVel, angRightPost));
    float angDiffLeft = fabs(Utils::angleDiff(angVel, angLeftPost));

    return (angDiffRight < angDiffPosts && angDiffLeft < angDiffPosts);
}

bool Role_Barrier::isBehindBall(Position posObjective) {
    Position posBall = getWorldMap()->getBall().ballposition();
    Position posPlayer = player()->getPlayerPos();
    float anglePlayer = Utils::getAngle(posBall, posPlayer);
    float angleDest = Utils::getAngle(posBall, posObjective);
    float diff = Utils::angleDiff(anglePlayer, angleDest);

    return (diff > M_PI / 1.5f);
}

bool Role_Barrier::isBallComing(float minSpeed, float radius) {
    Ball ball = getWorldMap()->getBall();
    Position ballPos = ball.ballposition();
    Position playerPos = player()->getPlayerPos();

    if (Utils::getVelocityAbs(ball.ballvelocity()) < minSpeed) {
        return false;
    }

    float angVel = Utils::getVelocityArg(ball.ballvelocity());
    float angPlayer = Utils::getAngle(ballPos, playerPos);

    float angDiff = Utils::angleDiff(angVel, angPlayer);
    float angError = atan2(radius, Utils::distance(playerPos, ballPos));

    return (fabs(angDiff) < fabs(angError));
}
