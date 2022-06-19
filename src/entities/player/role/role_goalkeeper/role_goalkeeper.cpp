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

#define ATTACKER_MINBALLDIST 0.4f
#define GOALPOSTS_ERROR 0.1f
#define INTERCEPT_MINBALLVELOCITY 0.2f
#define RECEIVER_INVALID_ID 200
#define INTERCEPT_MIN_SPEED 0.5f
#define INTERCEPT_RADIUS 0.5f

#define BALL_MINDIST 0.115f

#define BALLPREVISION_MINVELOCITY 0.02f
#define BALLPREVISION_VELOCITY_FACTOR 3.0f
#define BALLPREVISION_FACTOR_LIMIT 0.3f

QString Role_Goalkeeper::name() {
    return "Role_Goalkeeper";
}

Role_Goalkeeper::Role_Goalkeeper()
{
    _behaviour_goToLookTo = nullptr;
    // _behaviour_interceptBall = nullptr;

    _done = false;
    setRadius(0.5f);
    setTakeoutEnabled(true);
    setTakeoutFactor(1.0f);
    useAttackerOri(true);
}

void Role_Goalkeeper::initializeBehaviours() {
    addBehaviour(BEHAVIOUR_GOTOLOOKTO, _behaviour_goToLookTo = new Behaviour_GoToLookTo());
    addBehaviour(BEHAVIOUR_INTERCEPTBALL, _behaviour_interceptBall = new Behaviour_InterceptBall());
    addBehaviour(BEHAVIOUR_PUSHBALL, _behaviour_pushBall = new Behaviour_PushBall());
}

void Role_Goalkeeper::configure() {
    _notAlreadyChosen = true;

    setBehaviour(BEHAVIOUR_GOTOLOOKTO);
    _actualState = STATE_FOLLOWBALL;
}

void Role_Goalkeeper::run() {
    if (getReferee()->getGameInfo()->theirPenaltyKick()) {
        _actualState = STATE_PENALTY;
    }

    Position playerPos = player()->getPlayerPos();
    Position ballPos = getWorldMap()->getBall().ballposition();
    FieldSide teamSide = getConstants()->getTeamSide();

    switch (_actualState) {
    case STATE_FOLLOWBALL: {
        // Keeps following y of the ball, with x fixed.
        float fixedPosition = 4.3f;

        float yRightPost = getLocations()->ourGoalRightPost().y();
        float yLeftPost = getLocations()->ourGoalLeftPost().y();

        // Sidestep will only be turned on if the ball is dangerously close to the goal
        bool sideStep = false;

        FieldSide teamSide = getConstants()->getTeamSide();
        if (teamSide.isLeft()) {
            fixedPosition = fixedPosition * -1;
            if (ballPos.x() < fixedPosition) {
                fixedPosition = ballPos.x();
            }

            // Swap values because of perspective
            float aux = yRightPost;
            yRightPost = yLeftPost;
            yLeftPost = aux;

            if (ballPos.x() < 2.0f) {
                sideStep = true;
            }
        } else {
            if (ballPos.x() > fixedPosition) {
                fixedPosition = ballPos.x();
            }
            if (ballPos.x() > 2.0f) {
                sideStep = true;
            }
        }

        yRightPost += 0.07f;
        yLeftPost -= 0.07f;

        float yPos = ballPos.y();
        if (sideStep) {
            // GK Sidestep
            float sideStepLength = getConstants()->getRobotRadius() / 1.5f;
            int maxSideStepCounter = 100;
            int halfMaxSideStepCounter = maxSideStepCounter / 2;

            bool sideSelector;

            // If the counter is less than five, sidesteps to one side, if not, sidesteps to the other side
            if (_sideStepCounter < halfMaxSideStepCounter) {
                sideSelector = true;
            } else {
                sideSelector = false;
            }

            if (sideSelector) {
                sideStepLength *= -1;
            }

            yPos += sideStepLength;

            _sideStepCounter++;

            if (_sideStepCounter > maxSideStepCounter) {
                _sideStepCounter = 0;
            }
        }

        // Ensure GK not leaving the goal area
        if (yPos > yRightPost) {
            yPos = yRightPost;
        }
        if (yPos < yLeftPost) {
            yPos = yLeftPost;
        }

        Position positionToGo = Utils::getPositionObject(fixedPosition, yPos);

        _behaviour_goToLookTo->setPositionToGo(positionToGo);
        _behaviour_goToLookTo->setPositionToLook(ballPos);
        _behaviour_goToLookTo->setReferencePosition(playerPos);
//                _behaviour_goToLookTo->setAvoidBall(false);

        setBehaviour(BEHAVIOUR_GOTOLOOKTO);

        // If the ball is comming to goal
        float minVelocity = 0.5f; // Dangerous ball velocity if comming to goal

        // If GK has ball possession or it is inside our area, go to take it out
        if (player()->hasBallPossession() || Utils::isInsideOurArea(ballPos)) {
            _actualState = STATE_TAKEOUT;
        }

        if (isBallComingToGoal(minVelocity, 0.2f)) {
            _actualState = STATE_INTERCEPT;
        }
    } break;
    case STATE_INTERCEPT: {
        if (!isBallComingToGoal(INTERCEPT_MIN_SPEED)) {
            // If ball not comming to our goal but inside our area, take it out
            // Otherwise not inside area, just follow it
            if (Utils::isInsideOurArea(ballPos)) {
                _actualState = STATE_TAKEOUT;
            } else {
                _actualState = STATE_FOLLOWBALL;
            }
        }

        // Setup InterceptBall
        // Makes GK look straight ahead, minimizing chances of ball ricocheting to goal
        float xLook;

        if (teamSide.isLeft()) {
            xLook = playerPos.x() + 1.0f;
        } else {
            xLook = playerPos.x() - 1.0f;
        }

        Position positionToLook = Utils::getPositionObject(xLook, playerPos.y());

        _behaviour_interceptBall->setPosToLook(positionToLook);
        _behaviour_interceptBall->setSpeedFactor(2.5f);
        _behaviour_interceptBall->setUseKickDevice(false);
        setBehaviour(BEHAVIOUR_INTERCEPTBALL);
    } break;
    case STATE_TAKEOUT: {
        if (!Utils::isInsideOurArea(ballPos)) {
            _actualState = STATE_FOLLOWBALL;
        } else if (!player()->hasBallPossession()) {

            _behaviour_goToLookTo->setPositionToGo(ballPos);
            _behaviour_goToLookTo->setPositionToLook(ballPos);
            _behaviour_goToLookTo->setReferencePosition(playerPos);

            setBehaviour(BEHAVIOUR_GOTOLOOKTO);
        } else {
            Position fieldCenter = getLocations()->fieldCenter();

            _behaviour_goToLookTo->setPositionToGo(playerPos);
            _behaviour_goToLookTo->setPositionToLook(fieldCenter);
            _behaviour_goToLookTo->setReferencePosition(playerPos);

            player()->playerDribble(true);

            if (player()->isSufficientlyAlignedTo(fieldCenter)) {
                player()->playerDribble(false);
                player()->playerKick(getConstants()->maxChipKickPower(), true);
            }
        }
    } break;
    case STATE_PENALTY: {
        _actualState = STATE_FOLLOWBALL;
    } break;
    }
}

bool Role_Goalkeeper::isBallComingToGoal(float minSpeed, float postsFactor){
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

Position Role_Goalkeeper::getAttackerInterceptPosition() {
    Position posBall = getWorldMap()->getBall().ballposition();

    Position interceptPosition = Utils::threePoints(getLocations()->ourGoal(), posBall, _radius, 0.0f);
    if (!_useAttackerOri) {
        return interceptPosition;
    }

    Position posImpact = calcAttackerBallImpact();
    if (posImpact.isinvalid()) {
        return interceptPosition;
    }

    float goalLeft = getLocations()->ourGoalLeftPost().y();
    float goalRight = getLocations()->ourGoalRightPost().y();

    if (getLocations()->ourSide().isRight()) {
        goalLeft += GOALPOSTS_ERROR;
        goalRight -= GOALPOSTS_ERROR;

        if (goalLeft >= posImpact.y() && posImpact.y() >= goalRight) {
            interceptPosition = Utils::threePoints(posImpact, posBall, _radius, 0.0f);
        }
    } else {
        goalLeft -= GOALPOSTS_ERROR;
        goalRight += GOALPOSTS_ERROR;

        if (goalRight >= posImpact.y() && posImpact.y() >= goalLeft) {
            interceptPosition = Utils::threePoints(posImpact, posBall, _radius, 0.0f);
        }
    }

    return interceptPosition;
}

Position Role_Goalkeeper::ballProjection() {
    int poss = -1;
    float projection;
    double coefAng;
    QList<int>::iterator it;
    QList<int> avPlayers;

    Color teamColor = getConstants()->teamColor();
    avPlayers = getWorldMap()->getRobotsIDs(teamColor);

    for (it = avPlayers.begin(); it != avPlayers.end(); it++) {
        // Not sure if it's the right approach
        Player *p = new Player(*it, getWorldMap(), getReferee(), getConstants());
        if ((*p).hasBallPossession()) {
            poss = (*it);
            break;
        }
    }

    if (poss == -1) {
        return Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    } else {
        if (!isBallAlignedToGoal(poss)) {
            return Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
        }

        Player *p = new Player(poss, getWorldMap(), getReferee(), getConstants());
        Position posBall = getWorldMap()->getBall().ballposition();

        Position playerPos = (*p).getPlayerPos();
        float anglePlayerBall = (*p).getPlayerAngleTo(posBall);

        coefAng = tan(anglePlayerBall);
        projection = coefAng*(getLocations()->ourGoal().x() - playerPos.x()) + playerPos.y();

        Utils::limitValue(&projection, -getLocations()->fieldDefenseWidth() / 2.0f, getLocations()->fieldDefenseWidth() / 2.0f);

        return Utils::getPositionObject(getLocations()->ourGoal().x(), projection);
    }
}

bool Role_Goalkeeper::isBehindBall(Position posObjective) {
    Position posBall = getWorldMap()->getBall().ballposition();
    Position posPlayer = player()->getPlayerPos();
    float anglePlayer = Utils::getAngle(posBall, posPlayer);
    float angleDest = Utils::getAngle(posBall, posObjective);
    float diff = Utils::angleDiff(anglePlayer, angleDest);

    return (diff > M_PI / 1.5f);
}

bool Role_Goalkeeper::isBallInFront() {
    Position posBall = getWorldMap()->getBall().ballposition();
    float anglePlayerBall = player()->getPlayerAngleTo(posBall);
    float diff = Utils::angleDiff(anglePlayerBall, player()->getPlayerOrientation());

    return (diff <= atan(0.55));
}

bool Role_Goalkeeper::isBallAlignedToGoal(int theirPlayerId) {
    const Position posRightPost = getLocations()->theirGoalRightPost();
    const Position posLeftPost = getLocations()->theirGoalLeftPost();

    Position posBall = getWorldMap()->getBall().ballposition();
    Player *p = new Player(theirPlayerId, getWorldMap(), getReferee(), getConstants());
    float angPlayerBall = (*p).getPlayerAngleTo(posBall);
    float angRightPost = (*p).getPlayerAngleTo(posRightPost);
    float angLeftPost = (*p).getPlayerAngleTo(posLeftPost);

    float angDiffPosts = Utils::angleDiff(angRightPost, angLeftPost);

    float angDiffRight = Utils::angleDiff(angPlayerBall, angRightPost);
    float angDiffLeft = Utils::angleDiff(angPlayerBall, angLeftPost);

    return (fabs(angDiffRight) < angDiffPosts && fabs(angDiffLeft) < angDiffPosts);
}

Position Role_Goalkeeper::calcAttackerBallImpact() {
    QList<int>::iterator it;
    QList<int> avPlayers;
    int poss = -1;

    Color teamColor = Utils::getColorObject(getConstants()->isTeamBlue());
    avPlayers = getWorldMap()->getRobotsIDs(teamColor);

    for (it = avPlayers.begin(); it != avPlayers.end(); it++) {
        // Not sure if it's the right approach
        Player *p = new Player(*it, getWorldMap(), getReferee(), getConstants());
        if ((*p).hasBallPossession()) {
            poss = (*it);
            break;
        }
    }

    if (poss == -1) {
        return Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    }

    Player *p = new Player(poss, getWorldMap(), getReferee(), getConstants());

    Position posBall = getWorldMap()->getBall().ballposition();
    float anglePlayerBall = (*p).getPlayerAngleTo(posBall);
    float orientation = (*p).getPlayerOrientation().value();
    float diff = Utils::angleDiff(anglePlayerBall, orientation);

    bool ans = (diff <= atan(0.7)); // atan(0.7) ≃ 35 degree
    if (!ans) {
        return Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    }

    float angleValue = orientation;
    if (getLocations()->ourSide().isLeft()) {
        angleValue = M_PI - angleValue;
    }

    if (angleValue == (2.0f * M_PI)) {
        angleValue = 0.0f;
    }

    if (fabs(angleValue) >= M_PI) {
        angleValue += (angleValue >= 0 ? -(2.0f * M_PI) : (2.0f * M_PI));
    }

    if (fabs(angleValue) >= (M_PI / 2.0f)) {
        return Utils::getPositionObject(0.0f, 0.0f, 0.0f, true);
    }

    float x = fabs(getLocations()->ourGoal().x() - posBall.x());
    float tg = tan(angleValue);
    float y = tg * x;

    float impactY = posBall.y() + y;
    float impactX = getLocations()->ourGoal().x();
    const Position posImpact = Utils::getPositionObject(impactX, impactY);

    // TODO: Check if path to impactPosition is clear

    return posImpact;
}

void Role_Goalkeeper::push(Position aim, bool shootWhenAligned, bool isPenalty, bool isParabolic, float kickPower) {
    if (aim.isinvalid()) {
        spdlog::error(Text::bold(QString("Aim Position invalid at push in %1").arg(name()).toStdString()));
        return;
    }
    Position posBall = getWorldMap()->getBall().ballposition();
    Position behindBall = Utils::threePoints(posBall, player()->getPlayerPos(), (isPenalty ? 0.0f : 0.25f), M_PI);
    Velocity velBall = getWorldMap()->getBall().ballvelocity();
    float velBallAbs = Utils::getVelocityAbs(velBall);

    if (velBallAbs > BALLPREVISION_MINVELOCITY) {
        const Position velUni = Utils::getPositionObject(velBall.vx() / velBallAbs, velBall.vy() / velBallAbs);

        float factor = BALLPREVISION_VELOCITY_FACTOR * velBallAbs;
        Utils::limitValue(&factor, 0.0f, BALLPREVISION_FACTOR_LIMIT);

        const Position delta = Utils::getPositionObject(factor * velUni.x(), factor * velUni.y());
        Position projectedPos = Utils::getPositionObject(behindBall.x() + delta.x(), behindBall.y() + delta.y());

        if (isBehindBall(projectedPos)) {
            behindBall = projectedPos;
        }
    }

    player()->playerDribble(true);
    Position playerPos = player()->getPlayerPos();

    if (player()->getPlayerDistanceTo(posBall) <= BALL_MINDIST && isBallInFront()) {
        player()->playerDribble(false);
        player()->playerKick(kickPower, isParabolic);
    } else {
        _behaviour_goToLookTo->setPositionToGo(behindBall);
        _behaviour_goToLookTo->setReferencePosition(playerPos);
        _behaviour_goToLookTo->setPositionToLook(posBall);
        setBehaviour(BEHAVIOUR_GOTOLOOKTO);
    }
}

int Role_Goalkeeper::getBestAttacker() {
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
