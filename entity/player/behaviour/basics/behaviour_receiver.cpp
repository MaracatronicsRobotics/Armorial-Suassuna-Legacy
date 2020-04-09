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
    _skill_GoToLookTo = NULL;
    _attackerId = NO_ATTACKER;
    setQuadrant(NO_QUADRANT);
    setFollowAttacker(true);
    setActionRadius(2.5, 4.5);
}

void Behaviour_Receiver::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_Receiver = new Skill_InterceptBall());

    setInitialSkill(_skill_GoToLookTo);

    addTransition(SK_GOTO, _skill_GoToLookTo, _skill_Receiver);
    addTransition(SK_RECV, _skill_Receiver, _skill_GoToLookTo);

    _state = STATE_POSITION;
};

void Behaviour_Receiver::run() {
    setQuadrant(QUADRANT_UPMID);
    if(_attackerId == NO_ATTACKER){
        printf("[BEHAVIOUR RECEIVER] Attacker isn't set (Receiver ID: %u)\n", player()->playerId());
        return ;
    }
    if(!PlayerBus::ourPlayerAvailable(_attackerId)){
        printf("[BEHAVIOUR RECEIVER] Attacker isn't available (Receiver ID: %u)\n", player()->playerId());
        return ;
    }

    Position _desiredPosition = getReceiverBestPosition(_quadrant, _attackerId, _minRadius, _maxRadius);
    double modDistToAttacker = fabs(WR::Utils::distance(player()->position(), PlayerBus::ourPlayer(_attackerId)->position()));

    // fazer machine state aqui

    /*switch (_state) {
    case STATE_POSITION:
        enableTransition(SK_GOTO);
        _skill_GoToLookTo->setAimPosition(PlayerBus::ourPlayer(_attackerId)->position());
        _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
        break;
    case STATE_WAIT:
    case STATE_RECEIVE:
        enableTransition(SK_RECV);
        if (loc()->b)
    }*/
}

void Behaviour_Receiver::goingToReceive(quint8 id){
    if(id == player()->playerId()){
        _state = STATE_WAIT; // tem q ser estado de recepção (interceptBall)
    }
}

void Behaviour_Receiver::attackerShooted(quint8 id){
    if(id == player()->playerId()){
        _state = STATE_RECEIVE; // criar estado p verificar se ta em posicao boa p receber (mover caso n esteja)
    }
}

Position Behaviour_Receiver::getReceiverBestPosition(int quadrant, quint8 attackerId, float minRadius, float maxRadius){
    if(!PlayerBus::ourPlayerAvailable(attackerId)){
        printf("[BEHAVIOUR RECEIVER] Receiver with id %u: attacker id isn't available.\n", player()->playerId());
        return Position(false, 0.0, 0.0, 0.0);
    }
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR RECEIVER] Receiver with id %u: quadrant isn't set.\n", player()->playerId());
        return Position(false, 0.0, 0.0, 0.0);
    }

    const Position goalPosition = loc()->ourGoal(); // Fundo do gol (pra pegar o goleiro deles)
    const Position attackerPos = PlayerBus::ourPlayer(attackerId)->position(); // Pegar posicao do attacker
    const float distToAttacker = WR::Utils::distance(player()->position(), attackerPos); // Distancia até o atacante
    const std::pair<Position, Position> quadrantPosition = getQuadrantInitialPosition(_quadrant); // Par de posições do quadrante
    float radius = (minRadius + maxRadius)/2.0; // pegar raio médio pra atuação

    //
    //    PARTE DO GOL   //
                         //

    // Free Angles do gol até as posições do quadrante
    Position initialPos = quadrantPosition.first; // pega as posicoes dos quadrantes gerados
    Position finalPos = quadrantPosition.second;

    // Pega a lista de obstaculos pra remover o proprio receiver
    QList<Obstacle> obstaclesList = FreeAngles::getObstacles(goalPosition, radius);
    int size = obstaclesList.size();
    for(int x = 0; x < size; x++){
        Obstacle obstAt = obstaclesList.at(x);
        if(obstAt.team() == player()->teamId() && obstAt.id() == player()->playerId()){
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

    // Calculando a posicao em relação ao raio de atuação que o receiver vai ficar
    float posAngle = GEARSystem::Angle::pi - largestGoalAngle; // angulo suplementar (variar o y)
    float posX = radius * cos(posAngle);
    float posY = radius * sin(posAngle);
    Position goalLinePos(true, goalPosition.x() - posX, posY, 0.0);

    //
    //    PARTE DO ATACANTE   //
                              //

    // Gerando pontos colineares com a reta formada pela posição do gol com a posição acima, deslocada com o raio minimo e maximo declarados
    const Position posMinRadius = WR::Utils::threePoints(goalPosition, goalLinePos, minRadius, 0.0);
    const Position posMaxRadius = WR::Utils::threePoints(goalPosition, goalLinePos, maxRadius, 0.0);

    // Pegando obstáculos até o atacante para remover o receiver
    QList<Obstacle> attackerObstaclesList = FreeAngles::getObstacles(loc()->ball(), distToAttacker);
    size = attackerObstaclesList.size();
    for(int x = 0; x < size; x++){
        Obstacle obstAt = attackerObstaclesList.at(x);
        if(obstAt.team() == player()->teamId() && obstAt.id() == player()->playerId()){
            attackerObstaclesList.removeAt(x);
            break;
        }
    }

    // Pegando free angles a partir do atacante
    float posMinAngle = WR::Utils::getAngle(loc()->ball(), posMinRadius);
    float posMaxAngle = WR::Utils::getAngle(loc()->ball(), posMaxRadius);
    WR::Utils::angleLimitZeroTwoPi(&posMinAngle);
    WR::Utils::angleLimitZeroTwoPi(&posMaxAngle);

    // Switch as posicoes para que o free angles nao pegue um intervalo falso (a posicao menor tem q ser a inicial)
    if(posMaxAngle > posMinAngle){
        initialPos = posMinRadius;
        finalPos = posMaxRadius;
    }else{
        initialPos = posMaxRadius;
        finalPos = posMinRadius;
    }

    // Finalmente calculamos o free angles do atacante
    QList<FreeAngles::Interval> freeAnglesToAttacker = FreeAngles::getFreeAngles(loc()->ball(), initialPos, finalPos, attackerObstaclesList);
    float largestAttackerAngle = 0.0; // salvar aqui o maior angulo livre pra o atacante
    if(freeAnglesToAttacker.isEmpty()){ // free angles desativado(?)
        return Position(false, 0.0, 0.0, 0.0); // debugar dps
    }else{
        // descobrir o maior intervalo de ang livre, pegar o meio (mais provavel de estar certo (ruido?))
        float largestAngle = 0.0, largestMid = 0.0;
        QList<FreeAngles::Interval>::iterator it;
        for(it = freeAnglesToAttacker.begin(); it != freeAnglesToAttacker.end(); it++){
            float initialAngle = it->angInitial();
            float finalAngle = it->angFinal();
            float dif = WR::Utils::angleDiff(initialAngle, finalAngle);
            if(dif > largestAngle){
                // salvo o maior intervalo (dif) e salvo o meio desse intervalo (final - dif/2)
                largestAngle = dif;
                largestMid = finalAngle - (dif / 2.0);
            }
        }
        largestAttackerAngle = largestMid; // finalmente salvo o angulo (meio do maior intervalo)
    }

    // Agora intercepto as duas linhas geradas (A linha gerada a partir do free angles para o gol com a linha gerada com o free angles para o atk)
    Line goalLine = Line::getLine(goalPosition, largestGoalAngle);
    Line attackerLine = Line::getLine(loc()->ball(), largestAttackerAngle);

    return goalLine.interceptionWith(attackerLine);
}

std::pair<Position, Position> Behaviour_Receiver::getQuadrantInitialPosition(int quadrant){
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR RECEIVER] Receiver with id %u: quadrant isn't set.\n", player()->playerId());
        return std::make_pair(Position(false, 0.0, 0.0, 0.0), Position(false, 0.0, 0.0, 0.0));
    }

    // Calc some points
    const float x = fabs(loc()->ourGoal().x());
    const float y = fabs(loc()->ourFieldTopCorner().y());

    const Position upL(true, -x, y, 0.0);
    const Position up(true, 0.0, y, 0.0);
    const Position upR(true, x, y, 0.0);
    const Position botL(true, -x, -y, 0.0);
    const Position bot(true, 0.0, -y, 0.0);
    const Position botR(true, x, -y, 0.0);
    const Position cen(true, 0.0, 0.0, 0.0);

    // Depois trocar isso pra loc().theirSide() !!!!!!
    bool sideIsLeft = loc()->ourSide().isLeft();

    if(sideIsLeft){
        switch(quadrant){
        case QUADRANT_UP:
            return std::make_pair(up, upL);
        break;
        case QUADRANT_UPMID:
            return std::make_pair(cen, up);
        break;
        case QUADRANT_BOT:
            return std::make_pair(botL, bot);
        break;
        case QUADRANT_BOTMID:
            return std::make_pair(bot, cen);
        break;
        }
    }else{
        switch(quadrant){
        case QUADRANT_UP:
            return std::make_pair(upR, up);
        break;
        case QUADRANT_UPMID:
            return std::make_pair(up, cen);
        break;
        case QUADRANT_BOT:
            return std::make_pair(bot, botR);
        break;
        case QUADRANT_BOTMID:
            return std::make_pair(cen, bot);
        break;
        }
    }
}
