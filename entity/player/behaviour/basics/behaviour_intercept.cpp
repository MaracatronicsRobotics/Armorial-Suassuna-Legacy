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

#include "behaviour_intercept.h"

QString Behaviour_Intercept::name() {
    return "Behaviour_Intercept";
}

Behaviour_Intercept::Behaviour_Intercept() {
    _distanceToGoal = 100;
    _circleRadius = 200;
}

void Behaviour_Intercept::configure() {
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
};

void Behaviour_Intercept::run() {
    // Pegando as variáveis a serem utilizadas
    Point2d ballPosition(loc()->ball().x() * 1000.0, loc()->ball().y() * 1000.0);
    Point2d robotPosition(player()->position().x() * 1000.0, player()->position().y() * 1000.0);
    Point2d desiredKick(ballPosition.x() - (loc()->theirGoal().x() * 1000.0), ballPosition.y() - (loc()->theirGoal().y() * 1000.0));

    // Gerando os circulos ao redor da bola
    std::pair<Circle, Circle> ballCircles = generateCircles(ballPosition, desiredKick);

    // Gerando o circulo do robô para obtenção do vetor velocidade que ele deve assumir
    Circle robotCircle = generateRobotCircle(ballCircles.first, ballCircles.second, robotPosition);

    // Descobre qual dos circulos da bola o robô está mais próximo e faz a intersecção
    Point2d centerCircle1 = ballCircles.first.getC();
    Point2d centerCircle2 = ballCircles.second.getC();
    Point2d i1, i2; // intersection points
    Circle circleAux(0, 0, 0); // auxiliar circle
    size_t i_points; // qt of intersection points

    if(pointDistance(centerCircle1, robotPosition) > pointDistance(centerCircle2, robotPosition)){
        circleAux = ballCircles.second;
        i_points = circleAux.intersect(robotCircle, i1, i2);
    }
    else{
        circleAux = ballCircles.first;
        i_points = ballCircles.first.intersect(robotCircle, i1, i2);
    }

    Point2d desiredVelocityVector;

    if(i_points == 2){
        // Vetores para o centro
        Point2d vectorC1, vectorC2;
        // Vetores tangentes
        Point2d vectorTg1, vectorTg2;

        // Gerando vetores dos pontos de intersecção para os centros do circulo
        vectorC1.setCoords(i1.x() - circleAux.getC().x(), i1.y() - circleAux.getC().y());
        vectorC2.setCoords(i2.x() - circleAux.getC().x(), i2.y() - circleAux.getC().y());

        // Gerando os vetores tangentes
        vectorTg1.setCoords(i1.x() - robotPosition.x(), i1.y() - robotPosition.y());
        vectorTg2.setCoords(i2.x() - robotPosition.x(), i2.y() - robotPosition.y());

        // 1 => counter clockwise
        // 0 => clockwise
        bool direction = 1;
        if(pointDistance(centerCircle1, robotPosition) < pointDistance(centerCircle2, robotPosition)) direction = 0;

        if(direction){
            if(isCounterClockwise(vectorC1, vectorTg1)){
                desiredVelocityVector.setCoords(vectorTg1.x(), vectorTg1.y());
            }
           if(isCounterClockwise(vectorC2, vectorTg2)){
               desiredVelocityVector.setCoords(vectorTg2.x(), vectorTg2.y());
           }
        }
        else{
            if(isClockwise(vectorC1, vectorTg1)){
                desiredVelocityVector.setCoords(vectorTg1.x(), vectorTg1.y());
            }
            if(isClockwise(vectorC2, vectorTg2)){
                desiredVelocityVector.setCoords(vectorTg2.x(), vectorTg2.y());
            }
        }
    }
    else if(i_points == 1){
        // Vetor para o centro
        Point2d vectorC1;

        // Gerando vetor do ponto de intersecção para o centro do circulo
        vectorC1.setCoords(i1.x() - circleAux.getC().x(), i1.y() - circleAux.getC().y());

        // 1 => counter clockwise
        // 0 => clockwise
        bool direction = 1;
        if(pointDistance(centerCircle1, robotPosition) < pointDistance(centerCircle2, robotPosition)) direction = 0;

        // Raio
        double radius = vectorC1.norm();
        double alpha = acos(vectorC1.x() / radius);
        double pi = acos(-1);

        if(vectorC1.y() < 0.0) alpha = - alpha;
        Point2d pointAux(radius * cos(alpha + (pi / 2.0)), radius * sin(alpha + (pi / 2.0)));
        desiredVelocityVector.setCoords(pointAux.x(), pointAux.y());

        if(direction){
            if(!isCounterClockwise(vectorC1, desiredVelocityVector)) desiredVelocityVector.setCoords(-pointAux.x(), -pointAux.y());
        }
        else{
            if(!isClockwise(vectorC1, desiredVelocityVector)) desiredVelocityVector.setCoords(-pointAux.x(), -pointAux.y());
        }
    }
    else{
        // Vetor para o centro
        Point2d vectorC1;

        // Gerando vetor do ponto do robô até o centro do circulo
        double radius = circleAux.getR() * 2.0;
        vectorC1.setCoords(robotPosition.x() - circleAux.getC().x(), robotPosition.y() - circleAux.getC().y());
        vectorC1.setCoords((vectorC1.x()/vectorC1.norm())*radius, (vectorC1.y()/vectorC1.norm())*radius);

        // 1 => counter clockwise
        // 0 => clockwise
        bool direction = 1;
        if(pointDistance(centerCircle1, robotPosition) < pointDistance(centerCircle2, robotPosition)) direction = 0;

        double alpha = acos(vectorC1.x()/vectorC1.norm());
        double pi = acos(-1);

        if(vectorC1.y() < 0.0){
            alpha = (2.0 * (pi)) - alpha;
        }

        // Ajustando    angulo
        double angulo_ajuste = asin(0.5);
        if(direction){
            angulo_ajuste = -angulo_ajuste;
        }


        Point2d pointAux(radius * cos(angulo_ajuste + alpha + (pi / 2.0)), radius * sin(angulo_ajuste + alpha + (pi / 2.0)));
        desiredVelocityVector.setCoords(pointAux.x(), pointAux.y());

        if(direction){
            if(!isCounterClockwise(vectorC1, desiredVelocityVector)) desiredVelocityVector.setCoords(-pointAux.x(), -pointAux.y());
        }
        else{
            if(!isClockwise(vectorC1, desiredVelocityVector)) desiredVelocityVector.setCoords(-pointAux.x(), -pointAux.y());
        }
    }

    if(desiredVelocityVector.norm() < _circleRadius){
        printf("arrumei\n");
        desiredVelocityVector.setCoords(desiredVelocityVector.x()/desiredVelocityVector.norm(), desiredVelocityVector.y()/desiredVelocityVector.norm());
        desiredVelocityVector.setCoords(desiredVelocityVector.x() * _circleRadius, desiredVelocityVector.y() * _circleRadius);
    }
    // Setando parametros na skill de goToLookTo
    _skill_goToLookTo->setDesiredPosition(Position(true, (robotPosition.x() + desiredVelocityVector.x())/1000.0, (robotPosition.y() + desiredVelocityVector.y())/1000.0, 0.0));
    _skill_goToLookTo->setAimPosition(Position(true, (desiredKick.x())/1000.0, (desiredKick.y())/1000.0, 0.0));
    _skill_goToLookTo->setAvoidBall(false);
}

std::pair<Circle, Circle> Behaviour_Intercept::generateCircles(Point2d& ballPosition, Point2d& kickVector){
    // Invertendo o kickVector para obter o ponto para posicionamento
    kickVector.setCoords(-kickVector.x(), -kickVector.y());
    double angle = acos(_distanceToGoal/(2.0 * _circleRadius));

    // Tornando kickVector unitaŕio
    double vectorNorm = kickVector.norm();
    kickVector.setCoords(kickVector.x() / vectorNorm, kickVector.y()/vectorNorm);
    Point2d kickVectorAux = kickVector;

    // Projetando no ponto na distancia _distanceToGoal da bola
    kickVector.setCoords(kickVector.x() * _distanceToGoal, kickVector.y() * _distanceToGoal);
    Point2d predictedPosition(ballPosition.x() + kickVector.x(), ballPosition.y() + kickVector.y());

    // Pegando angulo do vetor de chute
    double alpha = acos(kickVectorAux.x());
    if(kickVector.y() < 0) alpha = -alpha;

    // Gerando vetor da bola para o centro do circulo 1
    Point2d vec(_circleRadius*cos(alpha + angle), _circleRadius * sin(alpha + angle));

    // Gerando o centro do circulo 1
    Point2d centerCircle1(vec.x() + ballPosition.x(), vec.y() + ballPosition.y());

    // Invertendo o angulo para pegar o proximo circulo
    angle = -angle;

    // Gerando o vetor da bola para o centro do circulo 2
    Point2d vec2(_circleRadius*cos(alpha + angle), _circleRadius * sin(alpha + angle));

    // Gerando o centro do circulo 2
    Point2d centerCircle2(vec2.x() + ballPosition.x(), vec2.y() + ballPosition.y());

    // Gerando os circulos
    Circle circle1(_circleRadius, centerCircle1);
    Circle circle2(_circleRadius, centerCircle2);

    return std::make_pair(circle1, circle2);
}

Circle Behaviour_Intercept::generateRobotCircle(Circle& circle1, Circle& circle2, Point2d& robotPosition){
    Point2d c1 = circle1.getC(), c2 = circle2.getC();
    double distCircle1 = pointDistance(c1, robotPosition);
    double distCircle2 = pointDistance(c2, robotPosition);

    if(distCircle1 < distCircle2)
        return robotCircleAux(c1, robotPosition);
    else
        return robotCircleAux(c2, robotPosition);
}

Circle Behaviour_Intercept::robotCircleAux(Point2d& circleCenter, Point2d& robotPosition){
    Point2d center((circleCenter.x() + robotPosition.x())/2.0, (circleCenter.y() + robotPosition.y())/2.0);
    double radius = (pointDistance(circleCenter, robotPosition))/2.0;
    Circle genCirc(radius, center);

    return genCirc;
}

double Behaviour_Intercept::pointDistance(Point2d& a, Point2d& b){
    return (sqrt(( (a.x()-b.x()) * (a.x()-b.x()) ) + (( a.y()-b.y() ) * ( a.y()-b.y())) ));
}

bool Behaviour_Intercept::isCounterClockwise(Point2d& circleCenterVector, Point2d& testVector){
    if(circleCenterVector.x()) return ((circleCenterVector.x() * testVector.y()) > 0.0);
    else return ((testVector.x() * circleCenterVector.y()) < 0.0);
}

bool Behaviour_Intercept::isClockwise(Point2d& circleCenterVector, Point2d& testVector){
    return isCounterClockwise(circleCenterVector, testVector);
}
