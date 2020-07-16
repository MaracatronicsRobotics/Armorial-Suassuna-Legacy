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
    setActionRadius(2.0, 4.0);
}

void Behaviour_Receiver::configure() {
    usesSkill(_skill_GoToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_Receiver = new Skill_InterceptBall());

    setInitialSkill(_skill_GoToLookTo);

    addTransition(SK_RECV, _skill_GoToLookTo, _skill_Receiver);
    addTransition(SK_GOTO, _skill_Receiver, _skill_GoToLookTo);

    _state = STATE_POSITION;

    // Initial config
    _skill_Receiver->setUseKickDevice(true);
    _skill_Receiver->setInterceptAdvance(true);
};

void Behaviour_Receiver::run() {
    setQuadrant(getBestQuadrant());

    //_attackerId = 1;
    /*
    for(quint8 id = 0; id < MRCConstants::_qtPlayers; id++){
        if(PlayerBus::ourPlayerAvailable(id))
            if(PlayerBus::ourPlayer(id)->hasBallPossession())
                _attackerId = id;
    }
    */

    player()->dribble(true);
    Position bestAim = getBestAimPosition();

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
            _skill_GoToLookTo->setAimPosition(loc()->ourGoal());
        }
        else{
            Position _desiredPosition = getReceiverBestPosition(_quadrant, _attackerId, _minRadius, _maxRadius);
            _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
            //_skill_GoToLookTo->setAimPosition(bestAim);
            _skill_GoToLookTo->setAimPosition(loc()->ball());
            //_skill_GoToLookTo->setAimPosition(PlayerBus::ourPlayer(_attackerId)->position());
        }
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
        if((obstAt.team() == player()->teamId() && obstAt.id() == player()->playerId())){
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

Position Behaviour_Receiver::getBestPositionWithoutAttacker(int quadrant){
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR Receiver] Receiver with id %u: quadrant isn't set.\n", player()->playerId());
        return Position(false, 0.0, 0.0, 0.0);
    }

    const Position goalPosition = loc()->ourGoal(); // Fundo do gol (pra pegar o goleiro deles)
    const std::pair<Position, Position> quadrantPosition = getQuadrantInitialPosition(quadrant); // Par de posições do quadrante
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

    Line goalLine = Line::getLine(goalPosition, largestGoalAngle);
    double o_a, o_b;

    if(goalLine.a() == 0.0) return Position(false, 0.0, 0.0, 0.0);
    o_a = (-1)/goalLine.a();
    o_b = player()->position().y() - (o_a * player()->position().x());

    Line ortogonalLine(o_a, o_b);

    Position desiredPos = goalLine.interceptionWith(ortogonalLine);

    return desiredPos;
}

int Behaviour_Receiver::getBestQuadrant(){
    double bestDist = 999;
    int bestQuadrant = 0;
    for(int x = QUADRANT_UP; x <= QUADRANT_BOT; x++){
        Position quadrantBarycenter = getQuadrantBarycenter(x);
        double dist = WR::Utils::distance(player()->position(), quadrantBarycenter);
        if(dist < bestDist){
            bestDist = dist;
            bestQuadrant = x;
        }
    }

    return bestQuadrant;
}

Position Behaviour_Receiver::getQuadrantBarycenter(int quadrant){
    const float goal_x = loc()->ourGoal().x();
    const float goal_y = loc()->ourGoal().y();

    std::pair<Position, Position> points = getQuadrantInitialPosition(quadrant);

    const float barycenter_x = (points.first.x() + points.second.x() + goal_x) / 3.0;
    const float barycenter_y = (points.first.y() + points.second.y() + goal_y) / 3.0;

    return Position(true, barycenter_x, barycenter_y, 0.0);
}

std::pair<Position, Position> Behaviour_Receiver::getQuadrantInitialPosition(int quadrant){
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR ATTACKER] Attacker with id %u: quadrant isn't set.\n", player()->playerId());
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
        case QUADRANT_MID:
            return std::make_pair(up, bot);
        break;
        case QUADRANT_BOT:
            return std::make_pair(botL, bot);
        break;
        default:
            return std::make_pair(Position(false, 0.0, 0.0, 0.0), Position(false, 0.0, 0.0, 0.0));
        break;
        }
    }else{
        switch(quadrant){
        case QUADRANT_UP:
            return std::make_pair(upR, up);
        break;
        case QUADRANT_MID:
            return std::make_pair(up, bot);
        break;
        case QUADRANT_BOT:
            return std::make_pair(bot, botR);
        break;
        default:
            return std::make_pair(Position(false, 0.0, 0.0, 0.0), Position(false, 0.0, 0.0, 0.0));
        break;
        }
    }
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

Position Behaviour_Receiver::getBestAimPosition(){
    // Margin to avoid select the post as aim (or outside it)
    float postMargin = 0.05*loc()->fieldDefenseWidth()/2;

    // Adjust margin
    if(loc()->ourSide().isRight()) {
        postMargin = -postMargin;
    }

    // shift the position of the post to the center of the goal
    Position theirGoalRightPost = loc()->ourGoalRightPost();
    theirGoalRightPost.setPosition(theirGoalRightPost.x(),
                                   theirGoalRightPost.y() + postMargin,
                                   theirGoalRightPost.z());
    Position theirGoalLeftPost = loc()->ourGoalLeftPost();
    theirGoalLeftPost.setPosition(theirGoalLeftPost.x(),
                                  theirGoalLeftPost.y() - postMargin,
                                  theirGoalLeftPost.z());
    Position posTheirGoal = loc()->ourGoal();

    // get obstacles
    QList<Obstacle> obstacles = FreeAngles::getObstacles(loc()->ball());

    // Shift the obstacles
    QList<Obstacle>::iterator obst;

    for(obst = obstacles.begin(); obst != obstacles.end(); obst++) {
        obst->radius() = 1.2 * MRCConstants::_robotRadius;
        // access the robot=
        PlayerAccess *robot = NULL;

        if(player()->opTeamId() == obst->team()) {
            robot = PlayerBus::theirPlayer(obst->id());
        } else {
            robot = PlayerBus::ourPlayer(obst->id());
        }

        Velocity robotVel;
        if(robot != NULL) {
            robotVel = robot->velocity();
        }

        if(robotVel.abs() > 2*0.015) {
            float time = WR::Utils::distance(loc()->ball(), obst->position())/8.0f;
            if(robotVel.abs()*time > 0.2f) {
                time = 0.2f/robotVel.abs();
            }
            obst->position() = WR::Utils::vectorSum(obst->position(), robotVel, time);
        }
    }

    // get free angle with the shifted obstacles
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(loc()->ball(), theirGoalRightPost, theirGoalLeftPost, obstacles);

    // Get the largest

    float largestAngle=0, largestMid=0;

    if(freeAngles.size()==0) { // Without free angles
        return Position(false, 0.0, 0.0, 0.0);
    } else {
        for(int i=0; i<freeAngles.size(); i++) {
            float angI = freeAngles.at(i).angInitial();
            float angF = freeAngles.at(i).angFinal();
            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);
            float dif = angF - angI;
            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif>largestAngle) {
                largestAngle = dif;
                largestMid = angF - dif/2;
            }
        }
    }

    // With free angles

    // Triangle
    float x = posTheirGoal.x() - loc()->ball().x();
    float tg = tan(largestMid);
    float y = tg*x;

    // Impact point
    float pos_y = loc()->ball().y() + y;
    Position impactPos(true, posTheirGoal.x(), pos_y, 0.0);

    // Check if impact pos has enough space for the ball
    bool obstructedWay = loc()->isVectorObstructed(loc()->ball(), impactPos, player()->playerId(), MRCConstants::_ballRadius*1.5, false);

    if(obstructedWay) {
        return Position(false, 0.0, 0.0, 0.0);
    }

    return impactPos;
}
