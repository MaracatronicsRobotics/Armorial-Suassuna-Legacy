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

#include "behaviour_receiver.h"
#include <utils/freeangles/freeangles.h>
#include <entity/player/playerbus.h>
#include <utils/line/line.hh>

#define DIST_TO_ATK 1.0f
#define NO_ATTACKER 200

QString Behaviour_Receiver::name() {
    return "Behaviour_Receiver";
}

Behaviour_Receiver::Behaviour_Receiver() {

    _skill_GoToLookTo = NULL;
    _attackerId = NO_ATTACKER;
    setQuadrant(NO_QUADRANT);
    setFollowAttacker(true);
    setActionRadius(1.5, 3.5);
}

void Behaviour_Receiver::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_Receiver = new Skill_InterceptBall());

    setInitialSkill(_skill_GoToLookTo);

    addTransition(SK_RECV, _skill_GoToLookTo, _skill_Receiver);
    addTransition(SK_GOTO, _skill_Receiver, _skill_GoToLookTo);

    _state = STATE_POSITION;

    // Initial config
    _skill_Receiver->setUseKickDevice(false);
    _skill_Receiver->setInterceptAdvance(false);
};

void Behaviour_Receiver::run() {
    if(ref()->getGameInfo(player()->team()->teamColor())->penaltyKick()){
        if(ref()->getGameInfo(player()->team()->teamColor())->ourPenaltyKick()){
            Position desiredPosition;
            if(_quadrant == QUADRANT_UP || _quadrant == QUADRANT_UPMID)
                desiredPosition = Position(true, loc()->theirPenaltyMark().x(), loc()->theirPenaltyMark().y() - 1.0f, 0.0);
            else
                desiredPosition = Position(true, loc()->theirPenaltyMark().x(), loc()->theirPenaltyMark().y() + 1.0f, 0.0);

            _skill_GoToLookTo->setDesiredPosition(desiredPosition);
            _skill_GoToLookTo->setAimPosition(loc()->ball());
        }
        else{
            Position desiredPosition;
            if(_quadrant == QUADRANT_UP || _quadrant == QUADRANT_UPMID)
                desiredPosition = Position(true, loc()->fieldCenter().x(), loc()->fieldCenter().y() - 1.0f, 0.0);
            else
                desiredPosition = Position(true, loc()->fieldCenter().x(), loc()->fieldCenter().y() + 1.0f, 0.0);

            _skill_GoToLookTo->setDesiredPosition(desiredPosition);
            _skill_GoToLookTo->setAimPosition(loc()->ball());
        }
    }
    else if(ref()->getGameInfo(player()->team()->teamColor())->kickoff()){
        // kickoff
        if(ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
            if(_quadrant == QUADRANT_UP || _quadrant == QUADRANT_UPMID)
                _skill_GoToLookTo->setDesiredPosition(Position(true, (loc()->ourSide().isRight()) ? 0.1f : -0.1f, 2.5f, 0.0f));
            else
                _skill_GoToLookTo->setDesiredPosition(Position(true, (loc()->ourSide().isRight()) ? 0.1f : -0.1f, -2.5f, 0.0f));
        }
        else{
            if(_quadrant == QUADRANT_UP || _quadrant == QUADRANT_UPMID)
                _skill_GoToLookTo->setDesiredPosition(Position(true, (loc()->ourSide().isRight()) ? 1.5f : -1.5f, 1.5f, 0.0f));
            else
                _skill_GoToLookTo->setDesiredPosition(Position(true, (loc()->ourSide().isRight()) ? 1.5f : -1.5f, -1.5f, 0.0f));
        }
        _skill_GoToLookTo->setAimPosition(loc()->theirGoal());
        enableTransition(SK_GOTO);
        return ;
    }
    else if(!ref()->getGameInfo(player()->team()->teamColor())->gameOn() && !ref()->getGameInfo(player()->team()->teamColor())->directKick() && !ref()->getGameInfo(player()->team()->teamColor())->indirectKick()){
        // stop
        if(PlayerBus::ourPlayerAvailable(_attackerId)){
            Position desiredPosition;

            if(_quadrant == QUADRANT_UP || _quadrant == QUADRANT_UPMID)
                if(loc()->ourSide().isRight()) desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi + GEARSystem::Angle::pi / 16.0f);
                else desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi - GEARSystem::Angle::pi / 16.0f);
            else
                if(loc()->ourSide().isRight()) desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi - GEARSystem::Angle::pi / 16.0f);
                else desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi + GEARSystem::Angle::pi / 16.0f);

            _skill_GoToLookTo->setDesiredPosition(desiredPosition);
            _skill_GoToLookTo->setAvoidTeammates(false);
            _skill_GoToLookTo->setAimPosition(loc()->theirGoal());
        }
        else{
            std::cout << MRCConstants::red << "[ERROR]" << MRCConstants::reset << MRCConstants::defaultBold << " attackerId not set to receiver at stop." << std::endl << MRCConstants::reset;
        }
        enableTransition(SK_GOTO);
        return ;
    }
    player()->dribble(false);

    if(isBallComing(0.2f, 1.0f) && !player()->team()->hasBallPossession()){
        _skill_Receiver->setPositionToLook(loc()->ball());
        enableTransition(SK_RECV);
    }
    else{
        enableTransition(SK_GOTO);
        if(_attackerId == NO_ATTACKER){
            // caso o atacante n esteja disponivel, posicionar da mesma forma que o attacker
            // ou seja, na projecao ortogonal da reta onde há angulação livre.
            //printf("[BEHAVIOUR RECEIVER] Attacker isn't available (Receiver ID: %u)\n", player()->playerId());
            Position _desiredPosition = getBestPositionWithoutAttacker(_quadrant);

            _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
            _skill_GoToLookTo->setAimPosition(loc()->ball());
        }
        else{
            Position _desiredPosition = getReceiverPosition(_quadrant, _attackerId);

            if(_desiredPosition.isUnknown() || loc()->isOutsideField(_desiredPosition)) _desiredPosition = WR::Utils::getQuadrantBarycenter(_quadrant);

            _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
            _skill_GoToLookTo->setAimPosition(loc()->ball());
        }
    }
}

QList<FreeAngles::Interval> Behaviour_Receiver::getGoalFreeAngles(quint8 quadrant, float radius){
    const Position posGoal = loc()->theirGoal();

    // Calc pos angles
    std::pair<Position,Position> positions = WR::Utils::getQuadrantPositions(quadrant);
    Position initialPos = positions.first;
    Position finalPos = positions.second;

    // Generates obstacle list, removing the calling player
    QList<Obstacle> obstacles = FreeAngles::getObstacles(posGoal, radius);

    for(int i=0; i<obstacles.size(); i++) {
        Obstacle obst = obstacles.at(i);
        if(loc()->isInsideTheirArea(obst.position()) || (obst.team()==player()->teamId() && (obst.id() == player()->playerId() || obst.id() == _attackerId))){
            obstacles.removeAt(i);
            i--;
        }
    }

    float angInit = WR::Utils::getAngle(posGoal, initialPos);
    float angEnd  = WR::Utils::getAngle(posGoal, finalPos);
    if(angInit > angEnd) std::swap(initialPos, finalPos);

    /*
    std::cout << "quadrant: " << int(quadrant) << std::endl;
    std::cout << "initQuadrantPos: " << initialPos.x() << " . " << initialPos.y() << std::endl;
    std::cout << "finalQuadrantPos: " << finalPos.x() << " . " << finalPos.y() << std::endl;
*/

    // Calc free angles
    return FreeAngles::getFreeAngles(posGoal, initialPos, finalPos, obstacles);
}

Position Behaviour_Receiver::getBestPositionWithoutAttacker(int quadrant){
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR Receiver] Receiver with id %u: quadrant isn't set.\n", player()->playerId());
        return Position(false, 0.0, 0.0, 0.0);
    }

    const Position goalPosition = loc()->theirGoal(); // Fundo do gol (pra pegar o goleiro deles)
    const std::pair<Position,Position> quadrantPosition = WR::Utils::getQuadrantPositions(quadrant);
    float radius = 4.0; // pegar raio médio pra atuação

    //
    //    PARTE DO GOL   //
                         //

    // Free Angles do gol até as posições do quadrante
    Position initialPos = quadrantPosition.first; // pega as posicoes dos quadrantes gerados
    Position finalPos = quadrantPosition.second;

    // Pega a lista de obstaculos pra remover o proprio attacker
    QList<Obstacle> obstaclesList = FreeAngles::getObstacles(goalPosition, radius);
    for(int x = 0; x < obstaclesList.size(); x++){
        Obstacle obstAt = obstaclesList.at(x);
        if(obstAt.team() == player()->teamId() && (obstAt.id() == player()->playerId() || obstAt.id() == _attackerId)){
            obstaclesList.removeAt(x);
            x--;
        }
    }

    // Calcula free angles enviando a lista de obstaculos nova (receiver removido)
    QList<FreeAngles::Interval> freeAnglesToGoal = FreeAngles::getFreeAngles(goalPosition, initialPos, finalPos, obstaclesList);
    float largestGoalAngle = 0.0; // salvar aqui o maior angulo livre pra o gol
    if(freeAnglesToGoal.isEmpty()){ // free angles desativado(?)
        return Position(false, 0.0, 0.0, 0.0); // debugar dps
    }else{
        // descobrir o maior intervalo de ang livre, pegar o meio (mais provavel de estar certo (ruido?))
        float largestAngle = 0.0, largestMid = 0.0;
        QList<FreeAngles::Interval>::iterator it;
        for(it = freeAnglesToGoal.begin(); it != freeAnglesToGoal.end(); it++){
            float initialAngle = it->angInitial();
            float finalAngle = it->angFinal();
            float dif = WR::Utils::angleDiff(initialAngle, finalAngle);
            if(dif > largestAngle){
                // salvo o maior intervalo (dif) e salvo o meio desse intervalo (final - dif/2)
                largestAngle = dif;
                largestMid = finalAngle - (dif / 2.0f);
            }
        }
        largestGoalAngle = largestMid; // finalmente salvo o angulo (meio do maior intervalo)
    }

    Line goalLine = Line::getLine(goalPosition, largestGoalAngle);
    float o_a, o_b;

    if(goalLine.a() == 0.0f) return Position(false, 0.0f, 0.0f, 0.0f);
    o_a = (-1)/goalLine.a();
    o_b = player()->position().y() - (o_a * player()->position().x());

    Line ortogonalLine(o_a, o_b);

    Position desiredPos = goalLine.interceptionWith(ortogonalLine);

    return desiredPos;
}

bool Behaviour_Receiver::isBallComing(float minVelocity, float radius) {
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

Position Behaviour_Receiver::getReceiverPosition(int quadrant, quint8 attackerId){
    // Taking the largest angle interval for their goal
    QList<FreeAngles::Interval> goalInterval = getGoalFreeAngles(quadrant, 5.5f);

    float goalLargestAngle = 0.0f;
    float goalLargestMid = 0.0f;
    if(goalInterval.empty()){
        return Position(false, 0.0, 0.0, 0.0);
    }
    else{
        for(int x = 0; x < goalInterval.size(); x++){
            float angI = goalInterval.at(x).angInitial();
            float angF = goalInterval.at(x).angFinal();

            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);

            float dif  = angF - angI;

            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif > goalLargestAngle){
                goalLargestAngle = dif;
                goalLargestMid = (angF + angI) / 2.0f;
            }
        }
    }

    // Taking goalLine
    Line goalLine = Line::getLine(loc()->theirGoal(), goalLargestMid);

    // Taking points for ball free angles
    Position offGoalMinimumPosition = Position(true, loc()->theirGoal().x() + 1.5f * cos(goalLargestMid), loc()->theirGoal().y() + 1.5f * sin(goalLargestMid), 0.0);
    Position offGoalMaximumPosition = Position(true, loc()->theirGoal().x() + 4.0f * cos(goalLargestMid), loc()->theirGoal().y() + 4.0f * sin(goalLargestMid), 0.0);

    // Debug to UI
    CoachView::drawLine(offGoalMaximumPosition, offGoalMinimumPosition, RGBA(106, 90, 205, 1.0, MRCConstants::robotZ + 0.02));
    CoachView::drawTriangle(loc()->ball(), offGoalMaximumPosition, offGoalMinimumPosition, RGBA(178, 34, 34, 0.4, MRCConstants::robotZ + 0.01));

    // Taking obstacles from the ball, removing the receiver and the attacker
    QList<Obstacle> obstaclesFromBall = FreeAngles::getObstacles(loc()->ball());
    for(int x = 0; x < obstaclesFromBall.size(); x++){
        if(WR::Utils::distance(loc()->ball(), obstaclesFromBall[x].position()) <= 0.3f || (obstaclesFromBall[x].team() == player()->teamId() && (obstaclesFromBall[x].id() == player()->playerId() || obstaclesFromBall[x].id() == _attackerId))){
            obstaclesFromBall.removeAt(x);
            x--;
        }
    }

    // Free angles from ball to off goal points
    float ballLargestAngle = 0.0f;
    float ballLargestMid   = 0.0f;
    float largi = 0.0f;
    float largf = 0.0f;

    float angleMin = WR::Utils::getAngle(loc()->ball(), offGoalMinimumPosition);
    float angleMax = WR::Utils::getAngle(loc()->ball(), offGoalMaximumPosition);

    if(angleMin > angleMax) std::swap(offGoalMinimumPosition, offGoalMaximumPosition);

    QList<FreeAngles::Interval> ballInterval = FreeAngles::getFreeAngles(loc()->ball(), offGoalMinimumPosition, offGoalMaximumPosition, obstaclesFromBall);
    if(ballInterval.empty()){
        return Position(false, 0.0, 0.0, 0.0);
    }
    else{
        for(int x = 0; x < ballInterval.size(); x++){
            float angI = ballInterval.at(x).angInitial();
            float angF = ballInterval.at(x).angFinal();

            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);

            float dif  = angF - angI;

            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif > ballLargestAngle){
                largi = angI;
                largf = angF;
                ballLargestAngle = dif;
                ballLargestMid = (angF + angI) / 2.0f;
            }
        }
    }
/*
    std::cout << "largest: " << ballLargestAngle << std::endl;
    std::cout << "i: " << largi << std::endl;
    std::cout << "f: " << largf << std::endl;
*/
    // Getting ball line to line segment taken from the goal line
    Line ballLine = Line::getLine(loc()->ball(), ballLargestMid);
/*
    std::cout << "ballAngleMin: " << WR::Utils::getAngle(loc()->ball(), offGoalMinimumPosition) << std::endl;
    std::cout << "ballAngleMax: " << WR::Utils::getAngle(loc()->ball(), offGoalMaximumPosition) << std::endl;
    std::cout << "goalAngle: " << goalLargestAngle << std::endl;
    std::cout << "ballAngle: " << ballLargestMid << std::endl;
    std::cout << "ballPos  : " << loc()->ball().x() << " . " << loc()->ball().y() << std::endl;
    std::cout << "goalLine : " << goalLine.a() << " . " << goalLine.b() << std::endl;
    std::cout << "ballLine : " << ballLine.a() << " . " << ballLine.b() << std::endl;
    std::cout << "minPos   : " << offGoalMinimumPosition.x() << " . " << offGoalMinimumPosition.y() << std::endl;
    std::cout << "maxPos   : " << offGoalMaximumPosition.x() << " . " << offGoalMaximumPosition.y() << std::endl;
*/
    return goalLine.interceptionWith(ballLine);
}
