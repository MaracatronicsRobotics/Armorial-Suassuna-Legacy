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

#include "behaviour_barrier.h"

#define INTERCEPT_MINBALLVELOCITY 0.2f
#define INTERCEPT_MINBALLDIST 0.5f
#define ERROR_GOAL_OFFSET 0.15f
#define RECEIVER_INVALID_ID 200

QString Behaviour_Barrier::name() {
    return "Behaviour_Barrier";
}

Behaviour_Barrier::Behaviour_Barrier() {

    setMarkBall();
    setDistanceFromGk(0.0); // distance from our gk line to ball
    setRadius(1.4f); // radius from our goal center

    _sk_goto = NULL;
    _sk_gk = NULL;
    _sk_kick = NULL;

    _canTakeout = false;
    _avoidAllies = false;
    _interceptRadius = 0.5f;
}

void Behaviour_Barrier::configure() {
    usesSkill(_sk_goto = new Skill_GoToLookTo());
    usesSkill(_sk_gk = new Skill_InterceptBall());
    usesSkill(_sk_kick = new Skill_Test());

    // Setting initial skill
    setInitialSkill(_sk_goto);

    // Transitions
    addTransition(STATE_GOTO, _sk_kick, _sk_goto);
    addTransition(STATE_GOTO, _sk_gk, _sk_goto);

    addTransition(STATE_GK, _sk_goto, _sk_gk);
    addTransition(STATE_GK, _sk_kick, _sk_gk);

    addTransition(STATE_KICK, _sk_goto, _sk_kick);
    addTransition(STATE_KICK, _sk_gk, _sk_kick);

    _notAlreadyChosen = true;
};

void Behaviour_Barrier::run() {
    if(ref()->getGameInfo(player()->team()->teamColor())->theirPenaltyKick()){
        Position desiredPosition = Position(true, loc()->ourGoal().x() + (loc()->ourSide().isLeft() ? 1.0 : -1.0), _distanceFromGK <= 0.0 ? loc()->ourGoal().y() -1.3 : loc()->ourGoal().y() + 1.3, 0.0);

        _sk_goto->setDesiredPosition(desiredPosition);
        _sk_goto->setAimPosition(loc()->ball());
        enableTransition(STATE_GOTO);

        return ;
    }

    Position markPosition;
    if(_markNearestPlayer){
        if(_markPlayerId != 200){
            if(PlayerBus::theirPlayerAvailable(_markPlayerId))
                markPosition = PlayerBus::theirPlayer(_markPlayerId)->position();
            else
                markPosition = loc()->ball();
        }
        else{
            markPosition = loc()->ball();
        }
    }
    else
        markPosition = loc()->ball();

    // Pos barrier
    Position goalProjection = WR::Utils::projectPointAtSegment(loc()->ourGoalRightMidPost(), loc()->ourGoalLeftMidPost(), markPosition);
    Position desiredPosition = WR::Utils::threePoints(loc()->ourGoal(), markPosition, _radius, 0.0f);

    // Position to look
    Position aimPosition = WR::Utils::threePoints(loc()->ourGoal(), markPosition, 1000.0f, 0.0); // high distance (always will look)

    // Adjust _d
    if(_distanceFromGK != 0.0f){
        Position vector2Ball(true, loc()->ball().x() - desiredPosition.x(), loc()->ball().y() - desiredPosition.y(), 0.0);
        float vectorMod = sqrt(powf(vector2Ball.x(), 2) + powf(vector2Ball.y(), 2));
        vector2Ball.setPosition(vector2Ball.x()/vectorMod, vector2Ball.y()/vectorMod, 0.0);

        float ang = acos(vector2Ball.x());
        if(vector2Ball.y() < 0.0f) ang = GEARSystem::Angle::twoPi - ang;
        ang += GEARSystem::Angle::pi / 2.0f;
        WR::Utils::angleLimitZeroTwoPi(&ang);

        vector2Ball.setPosition(cos(ang) * _distanceFromGK, sin(ang) * _distanceFromGK, 0.0);

        desiredPosition.setPosition(desiredPosition.x() + vector2Ball.x(), desiredPosition.y() + vector2Ball.y(), 0.0);
    }

    // Error goal (desiredPosition sometimes goes off the field)
    if(loc()->ourSide().isRight() && desiredPosition.x() > loc()->ourGoal().x()-ERROR_GOAL_OFFSET){
        desiredPosition.setPosition(loc()->ourGoal().x()-ERROR_GOAL_OFFSET, desiredPosition.y(), 0.0);
    }else if(loc()->ourSide().isLeft() && desiredPosition.x() < loc()->ourGoal().x()+ERROR_GOAL_OFFSET){
        desiredPosition.setPosition(loc()->ourGoal().x()+ERROR_GOAL_OFFSET, desiredPosition.y(), 0.0);
    }

    // settings of goto
    _sk_goto->setDesiredPosition(desiredPosition);
    _sk_goto->setAimPosition(aimPosition);
    _sk_goto->setAvoidBall(false);
    _sk_goto->setAvoidTeammates(_avoidAllies);
    _sk_goto->setAvoidOpponents(false);
    _sk_goto->setAvoidOurGoalArea(true);

    // dribble disable
    player()->dribble(false);

    // settings of intercept
    _sk_gk->setInterceptAdvance(true);
    _sk_gk->setPositionToLook(loc()->theirGoal());

    // kick parameters
    if(!player()->hasBallPossession() && loc()->ballVelocity().abs() >= 0.2f)
        _notAlreadyChosen = true;

    // Transitions
    if(!player()->team()->hasBallPossession() && player()->distBall() > INTERCEPT_MINBALLDIST && (isBallComing(INTERCEPT_MINBALLVELOCITY, 1.0f) || isBallComingToGoal(INTERCEPT_MINBALLDIST))) {
        enableTransition(STATE_GK);
    } else {
        if(player()->canKickBall() && player()->distBall() <= 0.4f && _canTakeout && !loc()->isInsideOurArea(loc()->ball(), 1.05f) && WR::Utils::distance(player()->position(), loc()->ourGoal()) <= (_radius + 1.5f)){
            quint8 bestAttacker = getBestAttacker();
            if(bestAttacker != RECEIVER_INVALID_ID){
                Position bestAttackerPos = PlayerBus::ourPlayer(bestAttacker)->position();
                QList<quint8> shootList = {player()->playerId(), bestAttacker};
                _sk_kick->setAim(bestAttackerPos);
                _sk_kick->setKickPower(std::min(6.0, 0.75 * sqrt((player()->distanceTo(bestAttackerPos) * 9.8) / sin(2 * GEARSystem::Angle::toRadians(65.0)))));
                _sk_kick->shootWhenAligned(true);
            }
            else{
                _sk_kick->setAim(loc()->theirGoal());
                _sk_kick->setKickPower(getConstants()->getMaxKickPower());
                _sk_kick->shootWhenAligned(true);
            }
            _sk_kick->setIsParabolic(true);
            enableTransition(STATE_KICK);
        }else{
            enableTransition(STATE_GOTO);
        }
    }
}

bool Behaviour_Barrier::isBallComing(float minVelocity, float radius) {
    const Position posBall = loc()->ball();
    const Position posPlayer = player()->position();

    // Check ball velocity
    if(loc()->ballVelocity().abs() < minVelocity)
        return false;

    // Angle player
    float angVel = loc()->ballVelocity().arg().value();
    float angPlayer = WR::Utils::getAngle(posBall, posPlayer);

    // Check angle difference
    float angDiff = WR::Utils::angleDiff(angVel, angPlayer);
    float angError = atan2(radius, player()->distBall());

    return (fabs(angDiff) < fabs(angError));
}

bool Behaviour_Barrier::isBallComingToGoal(float minSpeed, float postsFactor){
    // postFactor é caso queiramos alongar mais a posição y da barra (margem de erro)
    const Position posBall = loc()->ball();
    const Position posRightPost(true, loc()->ourGoalRightPost().x(), loc()->ourGoalRightPost().y()*postsFactor, 0.0);
    const Position posLeftPost(true, loc()->ourGoalLeftPost().x(), loc()->ourGoalLeftPost().y()*postsFactor, 0.0);

    // Check ball velocity
    if(loc()->ballVelocity().abs()<minSpeed)
         return false;

    // Angle ball velocity
    float angVel = loc()->ballVelocity().arg().value();
    float angRightPost = WR::Utils::getAngle(posBall, posRightPost);
    float angLeftPost = WR::Utils::getAngle(posBall, posLeftPost);
    float angDiffPosts = fabs(WR::Utils::angleDiff(angRightPost, angLeftPost));

    // Check angle difference with posts
    float angDiffRight = fabs(WR::Utils::angleDiff(angVel, angRightPost));
    float angDiffLeft = fabs(WR::Utils::angleDiff(angVel, angLeftPost));

    return (angDiffRight<angDiffPosts && angDiffLeft<angDiffPosts);
}

bool Behaviour_Barrier::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/1.5f);
}

quint8 Behaviour_Barrier::getBestAttacker(){
    if(!_notAlreadyChosen){         // prevents the function from running after the best attacker has already been chosen
        return _bestAtt;
    }
    else{
        quint8 bestId = RECEIVER_INVALID_ID;
        QList<Player*> attackers = loc()->getMRCPlayers().values();         // list of all allies
        QList<Player*> opPlayers = loc()->getOpPlayers().values();          // list of all opponents
        float menDist = 0;
        for(int x = 0; x < attackers.size(); x++){
            if(PlayerBus::ourPlayerAvailable(attackers.at(x)->playerId()) &&
                    WR::Utils::distance(PlayerBus::ourPlayer(attackers.at(x)->playerId())->position(), loc()->ourGoal()) > 2 &&
                    attackers.at(x)->playerId() != player()->playerId()){       // only considers allies that are 2 meters from our goal and isn't itself
                Position recPos = PlayerBus::ourPlayer(attackers.at(x)->playerId())->position();
                float menDistPlayer = 1000;
                for(int y = 0; y < opPlayers.size(); y++){
                    if(PlayerBus::theirPlayerAvailable(opPlayers.at(y)->playerId()) && opPlayers.at(y) != NULL){
                        Position opPos = opPlayers.at(y)->position();
                        float distPlayer = WR::Utils::distance(recPos, opPos);
                        if(distPlayer < menDistPlayer){         // finds the distance of the closest opponent from the ally
                            menDistPlayer = distPlayer;
                        }
                    }
                }
                if(menDistPlayer > menDist){                    // picks the ally with the biggest distance from it's closest opponent
                    menDist = menDistPlayer;
                    bestId = attackers.at(x)->playerId();
                }
            }
        }
        _notAlreadyChosen = false;
        _bestAtt = bestId;
        return bestId;
    }
}
