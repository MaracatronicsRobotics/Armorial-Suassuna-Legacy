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
#define NO_ATTACKER -1

QString Behaviour_Receiver::name() {
    return "Behaviour_Receiver";
}

Behaviour_Receiver::Behaviour_Receiver() {
    _mrcconstants=getConstants();
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
    if(ref()->getGameInfo(player()->team()->teamColor())->kickoff()){
        // kickoff
        enableTransition(SK_GOTO);
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

        return ;
    }
    else if(!ref()->getGameInfo(player()->team()->teamColor())->gameOn()){
        // stop
        enableTransition(SK_GOTO);

        if(PlayerBus::ourPlayerAvailable(_attackerId)){
            float neededDistance = 0.5f;
            Position desiredPosition;

            if(_quadrant == QUADRANT_UP || _quadrant == QUADRANT_UPMID)
                if(loc()->ourSide().isRight()) desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi + GEARSystem::Angle::pi / 16.0);
                else desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi - GEARSystem::Angle::pi / 16.0);
            else
                if(loc()->ourSide().isRight()) desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi - GEARSystem::Angle::pi / 16.0);
                else desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.9f, GEARSystem::Angle::pi + GEARSystem::Angle::pi / 16.0);

            _skill_GoToLookTo->setDesiredPosition(desiredPosition);
            _skill_GoToLookTo->setAvoidTeammates(false);
            _skill_GoToLookTo->setAimPosition(loc()->theirGoal());
        }
        else{
            std::cout << MRCConstants::red << "[ERROR]" << MRCConstants::reset << MRCConstants::defaultBold << " attackerId not set to receiver at stop." << std::endl << MRCConstants::reset;
        }

        return ;
    }
    player()->dribble(true);

    if(isBallComing(0.2f, 1.0f)){
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
            std::pair<Position,Position> positions = WR::Utils::getQuadrantPositions(_quadrant, loc()->theirSide(), loc()->theirGoal(), loc()->ourFieldTopCorner());
            Position _desiredPosition = getReceiverBestPosition(_quadrant, _attackerId, _minRadius, _maxRadius);

            _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
            _skill_GoToLookTo->setAimPosition(loc()->ball());
        }
    }
}

QList<FreeAngles::Interval> Behaviour_Receiver::getGoalFreeAngles(quint8 quadrant, float radius){
    const Position posGoal = loc()->theirGoal();

    // Calc pos angles
    std::pair<Position,Position> positions = WR::Utils::getQuadrantPositions(quadrant, loc()->theirSide(), loc()->theirGoal(), loc()->ourFieldTopCorner());
    Position initialPos = positions.first;
    Position finalPos = positions.second;

    // Generates obstacle list, removing the calling player
    QList<Obstacle> obstacles = FreeAngles::getObstacles(posGoal, radius);
    for(int i=0; i<obstacles.size(); i++) {
        Obstacle obst = obstacles.at(i);
        if(obst.team()==player()->teamId() && obst.id()==player()->playerId())
            obstacles.removeAt(i);
    }

    // Calc free angles
    return FreeAngles::getFreeAngles(posGoal, initialPos, finalPos, obstacles);
}

Position Behaviour_Receiver::getReceiverBestPosition(int quadrant, quint8 attackerId, float minRadius, float maxRadius){
    const Position posTheirGoal = loc()->theirGoal();
    const Position posAttacker = PlayerBus::ourPlayer(attackerId)->position();
    const float distAttacker = WR::Utils::distance(player()->position(), posAttacker);

    // Radius
    float radius = minRadius + (float)(maxRadius-minRadius)/2.0;

    // Get free angles in goal
    QList<FreeAngles::Interval> goalFreeAngles = getGoalFreeAngles(quadrant, radius+2*MRCConstants::_robotRadius);
    float largestGoalAngle = 0;

    if(goalFreeAngles.empty()) { // Without free angles
        return Position(false, 0.0, 0.0, 0.0);
    } else {
        float largestAngle=0, largestMid=0;
        for(int i=0; i<goalFreeAngles.size(); i++) {
            float angI = goalFreeAngles.at(i).angInitial();
            float angF = goalFreeAngles.at(i).angFinal();
            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);
            float dif = WR::Utils::angleDiff(angI, angF);
            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif>largestAngle) {
                largestAngle = dif;
                largestMid = angF - dif/2;
            }
        }
        largestGoalAngle = largestMid;
    }

    // Get position
    float posAngle = GEARSystem::Angle::pi - largestGoalAngle;
    float posX = radius*cos(posAngle);
    float posY = radius*sin(posAngle);

    Position goalLinePos(true, posTheirGoal.x()-posX, posY, 0.0);

    const Position posMinRadius = WR::Utils::threePoints(posTheirGoal, goalLinePos, minRadius, 0.0);
    const Position posMaxRadius = WR::Utils::threePoints(posTheirGoal, goalLinePos, maxRadius, 0.0);

    // Get obstacles from attacker
    QList<Obstacle> atkObstacles = FreeAngles::getObstacles(loc()->ball(), distAttacker);

    for(int i=0; i<atkObstacles.size(); i++) {
        Obstacle obst = atkObstacles.at(i);
        if(obst.team()==player()->teamId() && obst.id()==player()->playerId()) {
            atkObstacles.removeAt(i);
            i--;
        }
    }

    // Get free angles from attacker
    Position initialPos, finalPos;
    float posMinAngle = WR::Utils::getAngle(loc()->ball(), posMinRadius);
    float posMaxAngle = WR::Utils::getAngle(loc()->ball(), posMaxRadius);

    WR::Utils::angleLimitZeroTwoPi(&posMinAngle);
    WR::Utils::angleLimitZeroTwoPi(&posMaxAngle);

    if(posMaxAngle>posMinAngle) {
        initialPos = posMinRadius;
        finalPos = posMaxRadius;
    } else {
        initialPos = posMaxRadius;
        finalPos = posMinRadius;
    }

    QList<FreeAngles::Interval> atkFreeAngles = FreeAngles::getFreeAngles(loc()->ball(), initialPos, finalPos, atkObstacles);
    float largestAtkAngle=0;

    if(atkFreeAngles.empty()) {
        return goalLinePos;
    } else {
        float largestAngle=0, largestMid=0;
        for(int i=0; i<atkFreeAngles.size(); i++) {
            float angI = atkFreeAngles.at(i).angInitial();
            float angF = atkFreeAngles.at(i).angFinal();
            float dif = WR::Utils::angleDiff(angI, angF);

            if(dif>largestAngle) {
                largestAngle = dif;
                largestMid = angF - dif/2;
            }
        }
        largestAtkAngle = largestMid;
    }

    // Get intercept position
    Line goalLine = Line::getLine(posTheirGoal, largestGoalAngle);
    Line atkLine = Line::getLine(loc()->ball(), largestAtkAngle);

    return goalLine.interceptionWith(atkLine);
}

Position Behaviour_Receiver::getBestPositionWithoutAttacker(int quadrant){
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR Receiver] Receiver with id %u: quadrant isn't set.\n", player()->playerId());
        return Position(false, 0.0, 0.0, 0.0);
    }

    const Position goalPosition = loc()->theirGoal(); // Fundo do gol (pra pegar o goleiro deles)
    const std::pair<Position,Position> quadrantPosition = WR::Utils::getQuadrantPositions(quadrant, loc()->theirSide(), loc()->theirGoal(), loc()->ourFieldTopCorner());
    float radius = 4.0; // pegar raio médio pra atuação

    //
    //    PARTE DO GOL   //
                         //

    // Free Angles do gol até as posições do quadrante
    Position initialPos = quadrantPosition.first; // pega as posicoes dos quadrantes gerados
    Position finalPos = quadrantPosition.second;

    // Pega a lista de obstaculos pra remover o proprio attacker
    QList<Obstacle> obstaclesList = FreeAngles::getObstacles(goalPosition, radius);
    int size = obstaclesList.size();
    for(int x = 0; x < size; x++){
        Obstacle obstAt = obstaclesList.at(x);
        if(obstAt.team() == player()->teamId() && (obstAt.id() == player()->playerId() || obstAt.id() == _attackerId)){
            obstaclesList.removeAt(x);
            break;
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
                largestMid = finalAngle - (dif / 2.0);
            }
        }
        largestGoalAngle = largestMid; // finalmente salvo o angulo (meio do maior intervalo)
    }

    Line goalLine = Line::getLine(goalPosition, largestGoalAngle);
    double o_a, o_b;

    if(goalLine.a() == 0.0) return Position(false, 0.0, 0.0, 0.0);
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
