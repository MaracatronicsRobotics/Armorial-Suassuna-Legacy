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

#include "behaviour_goalkeeper.h"
#include <utils/knn/knn.hh>
#include <entity/player/playerbus.h>
#include <utils/freeangles/freeangles.h>

#define ATTACKER_MINBALLDIST 0.4f
#define GOALPOSTS_ERROR 0.1f
#define INTERCEPT_MINBALLVELOCITY 0.2f
#define RECEIVER_INVALID_ID 200

QString Behaviour_Goalkeeper::name() {
    return "Behaviour_GoalKeeper";
}

Behaviour_Goalkeeper::Behaviour_Goalkeeper() {
    _skill_Goalkeeper = NULL;
    _skill_goToLookTo = NULL;
    _skill_push = NULL;


    setRadius(0.5); // raio que define posse de bola para o goleiro dar takeout
    setTakeoutEnabled(true); // avançar na bola quando ela estiver na margem de aceitação (takeout vai dar goto e kick na bola)
    setTakeoutFactor(1.0); // fator de erro pra largura do gol (avançar na bola)
    useAttackerOri(true); // pra levar o atacante em consideração na projeção no gol
}

void Behaviour_Goalkeeper::configure() {
    usesSkill(_skill_Goalkeeper = new Skill_InterceptBall());
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_push = new Skill_PushBall());

    // goto
    setInitialSkill(_skill_Goalkeeper);

    // todas as combinações
    addTransition(STATE_GK, _skill_goToLookTo, _skill_Goalkeeper);
    addTransition(STATE_GK, _skill_push, _skill_Goalkeeper);
    //
    addTransition(STATE_GOTO, _skill_Goalkeeper, _skill_goToLookTo);
    addTransition(STATE_GOTO, _skill_push, _skill_goToLookTo);
    //
    addTransition(STATE_PUSH, _skill_Goalkeeper, _skill_push);
    addTransition(STATE_PUSH, _skill_goToLookTo, _skill_push);

    _notAlreadyChosen = true;
};

void Behaviour_Goalkeeper::run() {

    _skill_Goalkeeper->setInterceptAdvance(false);
    _skill_Goalkeeper->setPositionToLook(loc()->ball());
    _skill_Goalkeeper->setSpeedFactor(1.15f);

    // goToLookTo (posicionamento do goleiro
    Position desiredPosition = getAttackerInterceptPosition();

    if(loc()->ourSide().isRight() && desiredPosition.x() > loc()->ourGoal().x()-GOALPOSTS_ERROR){        // cobrir angulos certos (manter goleiro na area do gol!)
        desiredPosition.setPosition(loc()->ourGoal().x()-GOALPOSTS_ERROR, desiredPosition.y(), 0.0);     // varia de lado pra lado, com erros nas barras p precisao
    }
    else if(loc()->ourSide().isLeft() && desiredPosition.x() < loc()->ourGoal().x()+GOALPOSTS_ERROR){
        desiredPosition.setPosition(loc()->ourGoal().x()+GOALPOSTS_ERROR, desiredPosition.y(), 0.0);
    }

    _skill_goToLookTo->setDesiredPosition(desiredPosition);
    _skill_goToLookTo->setAimPosition(loc()->ball());
    _skill_goToLookTo->setAvoidBall(false);
    _skill_goToLookTo->setAvoidOurGoalArea(false);
    _skill_goToLookTo->setMinVelocity(0.15f);

    // kick parameters
    if(!player()->hasBallPossession() && loc()->ballVelocity().abs() >= 0.2f)
        _notAlreadyChosen = true;

    // machine if state begins for transitionsss
    if(isBallComingToGoal(INTERCEPT_MINBALLVELOCITY, 1.1f) && !player()->hasBallPossession()){ // bola nao ta em posse do goleiro e ta indo pro gol
        enableTransition(STATE_GK); // defende!
    }else if(_takeoutEnabled && player()->canKickBall()){ // caso n esteja em posse, n esteja indo pro gol ou nenhum dos dois
        if(loc()->isInsideOurArea(loc()->ball(), _takeoutFactor)){ // ve se ta na nossa area com fator de takeout (uma area maiorzinha)
            quint8 bestAttacker = getBestAttacker();
            if(bestAttacker != RECEIVER_INVALID_ID){
                Position bestAttackerPos = PlayerBus::ourPlayer(bestAttacker)->position();
                QList<quint8> shootList = {player()->playerId(), bestAttacker};
                _skill_push->setAim(bestAttackerPos);
                _skill_push->setKickPower(std::min(getConstants()->getMaxKickPower(), 0.75f * sqrt((player()->distanceTo(bestAttackerPos) * 9.8f) / sin(2 * GEARSystem::Angle::toRadians(65.0)))));
                _skill_push->shootWhenAligned(true);
                _skill_push->setIsPenalty(true);
            }
            else{
                _skill_push->setAim(loc()->theirGoal());
                _skill_push->setKickPower(getConstants()->getMaxKickPower());
                _skill_push->shootWhenAligned(true);
                _skill_push->setIsPenalty(true);
            }
            _skill_push->setIsParabolic(true);
            enableTransition(STATE_PUSH);
        }
        else{ // evitar oscilação (ruido) do visao
            enableTransition(STATE_GOTO); // goTo na bolota se n estiver na area
        }
    }else{
        enableTransition(STATE_GOTO); // caso n usemos takeout, fica dando só goToLookTo mesmo (tentativa de dominar bola)
    }

}

bool Behaviour_Goalkeeper::isBallComingToGoal(float minSpeed, float postsFactor){
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

Position Behaviour_Goalkeeper::getAttackerInterceptPosition() {
    // Defense position
    Position interceptPosition = WR::Utils::threePoints(loc()->ourGoal(), loc()->ball(), _radius, 0.0);
    if(_useAttackerOri==false)
        return interceptPosition;

    // Calc ball impact based on attacker ori and check if its going to the goal
    Position posImpact = calcAttackerBallImpact();
    if(posImpact.isUnknown())
        return interceptPosition;

    float goal_left = loc()->ourGoalLeftPost().y();
    float goal_right = loc()->ourGoalRightPost().y();

    if(loc()->ourSide().isRight()) {
        goal_left += GOALPOSTS_ERROR;
        goal_right -= GOALPOSTS_ERROR;
        if(goal_left >= posImpact.y() && posImpact.y() >= goal_right)
            interceptPosition = WR::Utils::threePoints(posImpact, loc()->ball(), _radius, 0.0);
    } else {
        goal_left -= GOALPOSTS_ERROR;
        goal_right += GOALPOSTS_ERROR;
        if(goal_right >= posImpact.y() && posImpact.y() >= goal_left)
            interceptPosition = WR::Utils::threePoints(posImpact, loc()->ball(), _radius, 0.0);
    }

    return interceptPosition;
}

Position Behaviour_Goalkeeper::ballProjection(){
    int poss = -1;
    float projection;
    double coefAng;
    QHash<quint8, Player*>::iterator it;
    QHash<quint8, Player*> avPlayers = loc()->getOpPlayers();

    for(it=avPlayers.begin(); it!=avPlayers.end(); it++){
        if((*it)->hasBallPossession()){
            poss = (*it)->playerId();
            break;
        }
    }

    if (poss == -1){
        return Position(false, 0.0, 0.0, 0.0);
    } else {
        if(!isBallAlignedToGoal(quint8(poss))) return Position(false, 0.0, 0.0, 0.0);

        Position playerPos = PlayerBus::theirPlayer(quint8(poss))->position();
        Angle anglePlayerBall = PlayerBus::theirPlayer(quint8(poss))->angleTo(loc()->ball());

        coefAng = tan(anglePlayerBall.value());
        projection = coefAng*(loc()->ourGoal().x() - playerPos.x()) + playerPos.y();

        WR::Utils::limitValue(&projection, -loc()->fieldDefenseWidth()/2.0, loc()->fieldDefenseWidth()/2.0);

        return Position(true, loc()->ourGoal().x(), projection, 0.0);  //coordenada em Y
    }
}

Position Behaviour_Goalkeeper::calcAttackerBallImpact() {
    QHash<quint8, Player*>::iterator it;
    QHash<quint8, Player*> avPlayers = loc()->getOpPlayers();      // ALTERA AQUI ZILDAO
    int poss = -1;

    for(it=avPlayers.begin(); it!=avPlayers.end(); it++){
        if((*it)->hasBallPossession()){
            poss = (*it)->playerId();
            break;
        }
    }

    if(poss == -1) // n tem ngm com a posse, mantem no centro do gol
        return Position(false, 0.0, 0.0, 0.0);

    // check if ball is in front of player (avoid y errors)
    Position playerPos = PlayerBus::theirPlayer(quint8(poss))->position(); // ALTERA AQUI ZILDAO
    Angle anglePlayerBall = PlayerBus::theirPlayer(quint8(poss))->angleTo(loc()->ball()); // ALTERA AQUI ZILDAO
    float diff = WR::Utils::angleDiff(anglePlayerBall, PlayerBus::theirPlayer(quint8(poss))->orientation()); // ALTERA AQUI ZILDAO
    bool ans = (diff <= atan(0.7)); // atan(0.7) aprox = 35 degree
    if(!ans) return Position(false, 0.0, 0.0, 0.0);

    /* calculando posicao de impacto no y */
    Angle angleAtk = PlayerBus::theirPlayer(quint8(poss))->orientation(); // ALTERA AQUI ZILDAO
    float angleValue = angleAtk.value();

    if(loc()->ourSide().isLeft()){ // ajustando pra o lado esquerdo
        angleValue = GEARSystem::Angle::pi - angleValue;
    }

    // Check 2pi
    if(angleValue==GEARSystem::Angle::twoPi)
        angleValue = 0.0f;

    // Check high angle
    if(fabs(angleValue)>=GEARSystem::Angle::pi)
        angleValue += (angleValue>=0? -GEARSystem::Angle::twoPi : GEARSystem::Angle::twoPi);

    // Check impossible impact (infinite tg)
    if(fabs(angleValue)>=GEARSystem::Angle::pi/2.0f)
        return Position(false, 0.0, 0.0, 0.0);

    // Triagulate
    float x = fabs(loc()->ourGoal().x() - loc()->ball().x());
    float tg = tan(angleValue);
    float y = tg*x;

    // Impact point
    float impact_y = loc()->ball().y() + y;
    float impact_x = loc()->ourGoal().x();
    const Position posImpact(true, impact_x, impact_y, 0.0); // posicao de impacto (mudando só o y em teoria)
                                                                      // verificar dps a ideia de mover ele pra frente e reduzir angulação
    // Check if path to impactPosition is clear
    QList<quint8> exceptions;
    exceptions.push_back(player()->playerId());   // Set gk exception

    // Get angles of destination
    Obstacle destination;
    destination.position() = posImpact;
    destination.radius() = 0.09f;
    destination.calcAnglesFrom(playerPos);

    // Generate obstacles and remove exceptions
    QList<Obstacle> obstacles = FreeAngles::getObstacles(playerPos);

    for(int i = 0; i < exceptions.size(); i++){
        const quint8 exception = exceptions.at(i);

        for(int j = 0; j < obstacles.size(); j++){
            Obstacle obst = obstacles.at(j);
            if(obst.id() == exception && obst.team() == player()->teamId()){
                obstacles.removeAt(j);
                j--;
            }
        }
    }

    // Calc free angles and check if has any free angles
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(playerPos, destination.initialAngle(), destination.finalAngle(), obstacles);
    bool hasAnyPath = (freeAngles.isEmpty() == false);

    if(!hasAnyPath)
        return Position(false, 0.0, 0.0, 0.0);

    return posImpact; // retorna o impacto, em caso de alguem ter posse da bola
}

bool Behaviour_Goalkeeper::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/1.5f);
}



bool Behaviour_Goalkeeper::isBallAlignedToGoal(quint8 theirPlayerId) {
    const Position posRightPost = loc()->theirGoalRightPost();
    const Position posLeftPost = loc()->theirGoalLeftPost();

    Angle angPlayerBall = PlayerBus::theirPlayer(theirPlayerId)->angleTo(loc()->ball());
    Angle angRightPost = PlayerBus::theirPlayer(theirPlayerId)->angleTo(posRightPost);
    Angle angLeftPost = PlayerBus::theirPlayer(theirPlayerId)->angleTo(posLeftPost);

    float angDiffPosts = WR::Utils::angleDiff(angRightPost, angLeftPost);
    // Check angle difference with posts
    float angDiffRight = WR::Utils::angleDiff(angPlayerBall, angRightPost);
    float angDiffLeft = WR::Utils::angleDiff(angPlayerBall, angLeftPost);

    return (fabs(angDiffRight)<angDiffPosts && fabs(angDiffLeft)<angDiffPosts);
}

quint8 Behaviour_Goalkeeper::getBestAttacker(){
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
