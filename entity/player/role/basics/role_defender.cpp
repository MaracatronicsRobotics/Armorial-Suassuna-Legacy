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

#include "role_defender.h"

#define BARRIER_RADIUS 1.4f
#define INTERCEPTION_PREVISION 0.2f

QString Role_Defender::name(){
    return "Role_Defender";
}

Role_Defender::Role_Defender() {
    _bh_bar = NULL;
}

void Role_Defender::initializeBehaviours(){
    usesBehaviour(BEHAVIOUR_BARRIER, _bh_bar = new Behaviour_Barrier());
}

void Role_Defender::configure(){
}

void Role_Defender::run(){
    float distance = distanceFromMidGoalShoot(_barrierSide);
    if(loc()->isInsideOurField(loc()->ball())){
        if(_barrierSide == 'r') _bh_bar->setDistanceFromGk(distance);
        else if(_barrierSide == 'l') _bh_bar->setDistanceFromGk(-distance);
        else _bh_bar->setDistanceFromGk(0.0f);
    }
    else{
        if(_barrierSide == 'r') _bh_bar->setDistanceFromGk(1.0);
        else if(_barrierSide == 'l') _bh_bar->setDistanceFromGk(-1.0);
        else _bh_bar->setDistanceFromGk(0.0f);
    }

    setBehaviour(BEHAVIOUR_BARRIER);
}

float Role_Defender::distanceFromMidGoalShoot(char side) {
    // Gera-se as retas entre a bola e os postes para avaliar a distância das posições das
    // barreiras à elas com o objetivo de limitar o distanciamento entre as barreiras.

    // Definição dos ângulos aos postes
    float rightAngle = WR::Utils::getAngle(loc()->ball(), loc()->ourGoalRightPost());
    float leftAngle = WR::Utils::getAngle(loc()->ball(), loc()->ourGoalLeftPost());

    // Definicão dos coeficientes lineares
    float right_b = loc()->ourGoalRightPost().y() - tanf(rightAngle) * loc()->ourGoalRightPost().x() - 0.09f;
    float left_b = loc()->ourGoalLeftPost().y() - tanf(leftAngle) * loc()->ourGoalLeftPost().x() - 0.09f;

    Position barrierDivision = WR::Utils::threePoints(loc()->ourGoal(), loc()->ball(), BARRIER_RADIUS, 0.0);

    // Distância de ponto à reta
    float rightDistance = (tanf(rightAngle) * barrierDivision.x() - barrierDivision.y() + right_b) / sqrtf(pow(tanf(rightAngle), 2.0f) + 1);
    float leftDistance = (tanf(leftAngle) * barrierDivision.x() - barrierDivision.y() + left_b) / sqrtf(pow(tanf(leftAngle), 2.0f) + 1);

    float maxLimit = WR::Utils::distance(loc()->ball(), barrierDivision) * INTERCEPTION_PREVISION / 2.0f + 0.09f;

    // Limitação das distâncias
    WR::Utils::limitValue(&rightDistance, 0.09f, maxLimit);
    WR::Utils::limitValue(&leftDistance, 0.09f, maxLimit);

    if (side == 'r') return rightDistance;
    else if (side == 'l') return leftDistance;
}
