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

#include "behaviour_attacker.h"

#include <utils/freeangles/freeangles.h>
#include <utils/line/line.hh>

#define RECEIVER_INVALID_ID 200
#define MAX_DIST_KICK 1.5f

#define OUR_AREA_OFFSET 1.05f
#define THEIR_AREA_OFFSET 1.05f
#define OUT_FIELD_OFFSET 1.05f

QString Behaviour_Attacker::name() {
    return "Behaviour_Attacker";
}

Behaviour_Attacker::Behaviour_Attacker() {
}

void Behaviour_Attacker::configure() {
    usesSkill(_sk_kick = new Skill_Kick());
    usesSkill(_sk_goto = new Skill_GoToLookTo());
    usesSkill(_sk_push = new Skill_PushBall2());

    addTransition(SKT_KICK, _sk_goto, _sk_kick);
    addTransition(SKT_KICK, _sk_push, _sk_kick);

    addTransition(SKT_GOTO, _sk_push, _sk_goto);
    addTransition(SKT_GOTO, _sk_kick, _sk_goto);

    addTransition(SKT_PUSH, _sk_goto, _sk_push);
    addTransition(SKT_PUSH, _sk_kick, _sk_push);

    setInitialSkill(_sk_goto);

    _state = STATE_PUSH;
};

void Behaviour_Attacker::run() {
    // Condiçoes de restrição para chute:
    /*
     * Bola na área inimiga
     * Bola na nossa área
     * Bola fora do campo
     * Referee não permite que chutemos (cobrança do time inimigo)
    */

    if(!player()->canKickBall() || loc()->isInsideTheirArea(loc()->ball(), THEIR_AREA_OFFSET)
            || loc()->isOutsideField(loc()->ball(), OUT_FIELD_OFFSET) || loc()->isInsideOurArea(loc()->ball(), OUR_AREA_OFFSET))
        _state = STATE_CANTKICK;

    Position bestKickPosition = getBestPosition(getBestQuadrant());
    Position bestAimPosition = getBestAimPosition();
    Position impactPos = calcImpactPositionInGoal();

    switch(_state){
    case STATE_CANTKICK:{
        Position waitPosition;
        if(loc()->isInsideOurArea(loc()->ball(), OUR_AREA_OFFSET)){ // Caso esteja na nossa área
            // Projeta ponto na reta entre o gol e a bola (2m atrás da bola)
            waitPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 2.0f, GEARSystem::Angle::pi);
        }
        else if(loc()->isInsideTheirArea(loc()->ball(), THEIR_AREA_OFFSET)){ // Caso esteja area deles
            // Projeta ponto na reta entre o gol e a bola (2m atrás da bola)
            waitPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 2.0f, GEARSystem::Angle::pi);
        }
        else if(loc()->isOutsideField(loc()->ball(), OUT_FIELD_OFFSET)){ // Caso esteja fora do campo
            // Tem que ver o que fazer nesse caso, by now apenas congela o robô
            waitPosition = player()->position();
        }
        else{
            // Projeta ponto na reta entre o gol e a bola (0.6m atrás da bola)
            waitPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 0.6f, GEARSystem::Angle::pi);
        }

        // Configurando goTo
        Position lookPosition = WR::Utils::threePoints(loc()->ball(), waitPosition, 1000.0f, GEARSystem::Angle::pi);
        _sk_goto->setAimPosition(lookPosition);
        _sk_goto->setDesiredPosition(waitPosition);

        // Habilita a transição
        enableTransition(SKT_GOTO);

        // Verificação para troca de estado
        if(player()->canKickBall() && !loc()->isInsideTheirArea(loc()->ball(), THEIR_AREA_OFFSET)
                && !loc()->isInsideOurArea(loc()->ball(), OUT_FIELD_OFFSET) && !loc()->isOutsideField(loc()->ball(), OUR_AREA_OFFSET))
            _state = STATE_PUSH;

    }
    break;
    case STATE_PUSH:{
        /*
        if(player()->playerId() == 1){
            _state = STATE_PASS;
            break;
        }
        */

        // A ideia daqui é fazer o free angles pra o quadrante onde o atacante está, pegar a reta gerada
        // E com essa reta calcular a reta ortogonal (entre o robo e essa reta gerada) e posicionar o robo
        // na intersecção entre essas duas retas, arrastando a bola com o drible.

        if(bestKickPosition.isUnknown() || bestAimPosition.isUnknown()){ // Nao existem aberturas para o gol
            _state = STATE_PASS;
        }
        else{                             // Abertura para chute
            _sk_push->setDestination(bestKickPosition);
            _sk_push->setAim(bestAimPosition);
        }

        enableTransition(SKT_PUSH);

        bool isInFront = isBallInFront();
        bool isAlignedToGoal = isBallAlignedToGoal();
        bool ballHasFreePathToGoal = (impactPos.y() >= -0.5 + (0.05*loc()->fieldDefenseWidth()/2) && impactPos.y() <= (0.5 - 0.05*loc()->fieldDefenseWidth()/2)) ? hasBallAnyPathTo(impactPos) : false;
        bool isSufficientlyAlignedToAim =  WR::Utils::angleDiff(player()->angleTo(bestAimPosition), player()->orientation()) <= GEARSystem::Angle::toRadians(3);
        bool isCloseEnoughToGoal = player()->distanceTo(loc()->ourGoal()) <= MAX_DIST_KICK;
/*
        std::cout << "isInFront: " << isInFront << std::endl;
        std::cout << "isAlignedToGoal: " << isAlignedToGoal << std::endl;
        std::cout << "ballHasFreePathToGoal: " << ballHasFreePathToGoal << std::endl;
        std::cout << "isSufficientlyAlignedToAim: " << isSufficientlyAlignedToAim << std::endl;
*/
        if(isInFront && isAlignedToGoal && isSufficientlyAlignedToAim && ballHasFreePathToGoal){
            _sk_kick->setState(1); // set state as kick
            _state = STATE_KICK;
        }
        else if(isCloseEnoughToGoal || (_sk_push->getPushedDistance() >= _sk_push->getMaxPushDistance())){
            // melhorar essa condição pra fazer passe
            _state = STATE_PASS;
        }
    }
    break;
    case STATE_KICK:{
        if(bestAimPosition.isUnknown()) bestAimPosition = loc()->ourGoal();
        _sk_kick->setAim(bestAimPosition);
        _sk_kick->setPower(MRCConstants::_maxKickPower);

        enableTransition(SKT_KICK);

        // Caso tenha se distanciado muito da bola (possivel chute realizado) volta para se posicionar
        if(player()->distBall() > 0.35f)
            _state = STATE_PUSH;

    }
    break;
    case STATE_PASS:{
        quint8 bestReceiverId;
        if(player()->playerId() != 5)
            bestReceiverId = 5;
        else
            bestReceiverId = RECEIVER_INVALID_ID;
        if(bestReceiverId == RECEIVER_INVALID_ID) // Se não houverem receptores disponiveis, chuta
            _state = STATE_KICK;
        else{
            if(PlayerBus::ourPlayerAvailable(bestReceiverId)){
                /// TODO:
                /// Driblar a bola até o nosso aliado e chutar
                /// Se habilitar apenas o kick, talvez possam acontecer alguns erros inesperados =c
                /// Criar heurística para definir força do chute
                Position recvPos = WR::Utils::getPlayerKickDevice(bestReceiverId);
                _sk_kick->setIsChip(false);
                /*
                if(!hasBallAnyPathTo(recvPos)){
                    _sk_kick->setIsChip(true);
                }
                */
                _sk_kick->setAim(recvPos);
                float kickPower = std::min(6.0f, std::max(4.0f, 2.0f * WR::Utils::distance(player()->position(), recvPos)));
                _sk_kick->setPower(kickPower);

                enableTransition(SKT_KICK);
            }
            else{
                _state = STATE_KICK; // Caso o jogador n esteja disponivel, chuta
            }
        }

        /// TODO:
        /// Remover isso (?)
        // Caso tenha se distanciado muito da bola (possivel chute realizado) volta para se posicionar
        if(player()->distBall() > 0.35f)
            _state = STATE_PUSH;
    }
    break;
    }
}

Position Behaviour_Attacker::getBestAimPosition(){
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

quint8 Behaviour_Attacker::getBestReceiver(){
    quint8 bestRcv = RECEIVER_INVALID_ID;
    double dist = INFINITY;
    for(int x = 0; x < _recvs.size(); x++){
        if(PlayerBus::ourPlayerAvailable(_recvs.at(x))){
            Position recvPos = PlayerBus::ourPlayer(_recvs.at(x))->position();
            double distToAtk = player()->distanceTo(recvPos);
            if(distToAtk < dist){
                dist = distToAtk;
                bestRcv = _recvs.at(x);
            }
        }
    }

    return bestRcv;
}

bool Behaviour_Attacker::isBallInFront(){
    Angle anglePlayerBall = player()->angleTo(loc()->ball());
    float diff = WR::Utils::angleDiff(anglePlayerBall, player()->orientation());

    return (diff <= atan(0.7)); // atan(0.7) aprox = 35 degree
}

Position Behaviour_Attacker::calcImpactPositionInGoal(){
    Angle angleAtk = player()->orientation(); // ALTERA AQUI ZILDAO
    float angleValue = angleAtk.value();

    Line playerLine = Line::getLine(player()->position(), angleValue);
    float ans_x = loc()->ourGoal().x();
    float ans_y = playerLine.a() * ans_x + playerLine.b();

    return Position(true, ans_x, ans_y, 0.0);
}

Position Behaviour_Attacker::getBestPosition(int quadrant){
    if(quadrant == NO_QUADRANT){
        printf("[BEHAVIOUR ATTACKER] Attacker with id %u: quadrant isn't set.\n", player()->playerId());
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

int Behaviour_Attacker::getBestQuadrant(){
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

Position Behaviour_Attacker::getQuadrantBarycenter(int quadrant){
    const float goal_x = loc()->ourGoal().x();
    const float goal_y = loc()->ourGoal().y();

    std::pair<Position, Position> points = getQuadrantInitialPosition(quadrant);

    const float barycenter_x = (points.first.x() + points.second.x() + goal_x) / 3.0;
    const float barycenter_y = (points.first.y() + points.second.y() + goal_y) / 3.0;

    return Position(true, barycenter_x, barycenter_y, 0.0);
}

std::pair<Position, Position> Behaviour_Attacker::getQuadrantInitialPosition(int quadrant){
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

bool Behaviour_Attacker::hasBallAnyPathTo(Position posObjective){
    if(posObjective.isUnknown()) return false;

    // Getting angles
    Obstacle objective;
    objective.position() = posObjective;
    objective.radius() = 0.025 * 2.0; // 2.0 * ballRadius
    objective.calcAnglesFrom(loc()->ball());

    // Generating obstacle list
    QList<Obstacle> obstacles = FreeAngles::getObstacles(loc()->ball());

    // Calc free angles and return if has any free angles
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(loc()->ball(), objective.initialAngle(), objective.finalAngle(), obstacles);

    return (freeAngles.empty() == false);
}

bool Behaviour_Attacker::isBallAlignedToGoal(){
    const Position posRightPost = loc()->ourGoalRightPost();
    const Position posLeftPost = loc()->ourGoalLeftPost();
    Angle angPlayerBall = player()->angleTo(loc()->ball());
    Angle angRightPost = player()->angleTo(posRightPost);
    Angle angLeftPost = player()->angleTo(posLeftPost);
    float angDiffPosts = WR::Utils::angleDiff(angRightPost, angLeftPost);

    // Check angle difference with posts
    float angDiffRight = WR::Utils::angleDiff(angPlayerBall, angRightPost);
    float angDiffLeft = WR::Utils::angleDiff(angPlayerBall, angLeftPost);

    return (fabs(angDiffRight)<angDiffPosts && fabs(angDiffLeft)<angDiffPosts);
}
