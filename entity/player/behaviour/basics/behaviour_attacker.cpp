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
#define MAX_DIST_KICK 2.5f

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

    addTransition(STATE_KICK, _sk_goto, _sk_kick);
    addTransition(STATE_KICK, _sk_push, _sk_kick);

    addTransition(STATE_GOTO, _sk_push, _sk_goto);
    addTransition(STATE_GOTO, _sk_kick, _sk_goto);

    addTransition(STATE_PUSH, _sk_goto, _sk_push);
    addTransition(STATE_PUSH, _sk_kick, _sk_push);


    setInitialSkill(_sk_goto);

    _state = STATE_PUSH;

    _timer = new Timer();
};

void Behaviour_Attacker::run() {
    // Condiçoes de restrição para chute:
    /*
     * Bola na área inimiga
     * Bola na nossa área
     * Bola fora do campo
     * Referee não permite que chutemos (cobrança do time inimigo)
    */
/*
    if(!player()->canKickBall() || loc()->isInsideTheirArea(loc()->ball(), THEIR_AREA_OFFSET)
            || loc()->isOutsideField(loc()->ball(), OUT_FIELD_OFFSET) || loc()->isInsideOurArea(loc()->ball(), OUR_AREA_OFFSET))
        _state = STATE_CANTKICK;
*/
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
        enableTransition(STATE_GOTO);

        // Verificação para troca de estado
        if(player()->canKickBall() && !loc()->isInsideTheirArea(loc()->ball(), THEIR_AREA_OFFSET)
                && !loc()->isInsideOurArea(loc()->ball(), OUT_FIELD_OFFSET) && !loc()->isOutsideField(loc()->ball(), OUR_AREA_OFFSET))
            _state = STATE_PUSH;

    }
    break;
    case STATE_PUSH:{
        Position bestKickPosition = getBestKickPosition();

        if(bestKickPosition.isUnknown()){ // Nao existem aberturas para o gol

        }
        else{                             // Abertura para chute
            _sk_push->setDestination(bestKickPosition);
        }

        enableTransition(STATE_PUSH);

        // Se puxou a bola demais ou está na distancia maxima pra realizar um chute
        if(_sk_push->getPushedDistance() >= _sk_push->getMaxPushDistance() || WR::Utils::distance(player()->position(), bestKickPosition) < MAX_DIST_KICK){
            _state = STATE_KICK;
        }
    }
    break;
    case STATE_KICK:{
        Position bestKickPosition = getBestKickPosition();
        _sk_kick->setAim(bestKickPosition);
        _sk_kick->setIsPass(false);

        enableTransition(STATE_KICK);

        // Caso tenha se distanciado muito da bola (possivel chute realizado) volta para se posicionar
        if(player()->distBall() > 0.35f)
            _state = STATE_PUSH;

    }
    break;
    }
}

quint8 Behaviour_Attacker::getBestReceiver(){
    quint8 bestRcv = RECEIVER_INVALID_ID;
    double dist = INFINITY;
    for(int x = 0; x < _recvs.size(); x++){
        if(PlayerBus::ourPlayerAvailable(_recvs.at(x))){
            double distToAtk = sqrt(pow(player()->position().x() - PlayerBus::ourPlayer(_recvs.at(x))->position().x(), 2) + pow(player()->position().y() - PlayerBus::ourPlayer(_recvs.at(x))->position().y(), 2));
            if(distToAtk < dist){
                dist = distToAtk;
                bestRcv = _recvs.at(x);
            }
        }
    }

    return bestRcv;
}

bool Behaviour_Attacker::isBehindBall(Position posObjective){
    Position posBall = loc()->ball();
    Position posPlayer = player()->position();
    float anglePlayer = WR::Utils::getAngle(posBall, posPlayer);
    float angleDest = WR::Utils::getAngle(posBall, posObjective);
    float diff = WR::Utils::angleDiff(anglePlayer, angleDest);

    return (diff>GEARSystem::Angle::pi/2.0f);
}

Position Behaviour_Attacker::getBestKickPosition(){
    const Position goalRightPost = loc()->ourGoalRightPost();
    const Position goalLeftPost = loc()->ourGoalLeftPost();
    const Position goalCenter = loc()->ourGoal();

    // calculating angles
    float minAngle = WR::Utils::getAngle(loc()->ball(), goalRightPost);
    float maxAngle = WR::Utils::getAngle(loc()->ball(), goalLeftPost);

    // generating list of freeAngles to goal
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(loc()->ball(), minAngle, maxAngle);

    float largestAngle, largestMid;
    // get the largest interval
    if(freeAngles.size() == 0){
        return Position(false, 0.0, 0.0, 0.0); // debugar isso dps
    }else{
        QList<FreeAngles::Interval>::iterator it;
        for(it = freeAngles.begin(); it != freeAngles.end(); it++){
            if(it->obstructed()) continue;
            float initAngle = it->angInitial();
            float endAngle = it->angFinal();
            WR::Utils::angleLimitZeroTwoPi(&initAngle);
            WR::Utils::angleLimitZeroTwoPi(&endAngle);

            float dif = endAngle - initAngle;
            WR::Utils::angleLimitZeroTwoPi(&dif);
            if(dif > largestAngle){
                largestAngle = dif;
                largestMid = endAngle - dif/2;
            }
        }
    }

    // Triangularization
    float x = goalCenter.x() - loc()->ball().x();
    float tg = tan(largestMid);
    float y = tg * x;

    // Impact point
    float pos_y = loc()->ball().y() + y;
    Position impactPosition(true, goalCenter.x(), pos_y, 0.0);

    // Check if impact position has space for ball radius
    const float distImpactPos = WR::Utils::distance(loc()->ball(), impactPosition);
    const float radiusAngle = largestAngle/2.0;
    const float distR = radiusAngle * distImpactPos;

    if(distR < (1.5 * 0.025)){ // 1.5 * raioDaBola (ruido ft. tristeza)
        return Position(false, 0.0, 0.0, 0.0); // bola n passa, debugar isso dps
    }

    Line ballLineToGoal = Line::getLine(loc()->ball(), largestMid);

    double o_a = (-1)/ballLineToGoal.a();
    double o_b = player()->position().y() - (o_a * player()->position().x());

    Line ortogonalLineToBallLine(o_a, o_b);

    std::cout << "ballLine: " << ballLineToGoal.a() << " . " << ballLineToGoal.b() << std::endl;
    std::cout << "ortPlayerLine: " << ortogonalLineToBallLine.a() << " . " << ortogonalLineToBallLine.b() << std::endl;

    return impactPosition;
}
