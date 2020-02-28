#include "fastpathplanning.h"
#define RAIOROBO 0.09
#define RAIOBOLA 0.024
FastPathPlanning::FastPathPlanning()
{




}


void FastPathPlanning::reset(){
    _colisionPosition.clear();
};
void FastPathPlanning::addBall(const Position &pos, const Velocity &vel){
    this->addColisionPosition(pos, true);
};
void FastPathPlanning::addOwnRobot(const Position &pos, const Velocity &vel){
    this->addColisionPosition(pos, false);
};
void FastPathPlanning::addEnemyRobot(const Position &pos, const Velocity &vel){
    this->addColisionPosition(pos, false);
};
void FastPathPlanning::addGoalArea(const Position &pos){
    this->addColisionPosition(pos, true);
};

void FastPathPlanning::run(){ // pego o ponto inicial e final


};

Angle FastPathPlanning::getDirection()const{
    Position lastPosition = _colisionPosition.last().first;
    Angle angleToGoal(true, WR::Utils::getAngle(lastPosition, goalPos()));
    return angleToGoal;
};

float FastPathPlanning::getDistance() const{
    Position lastPosition = _colisionPosition.last().first;
    float distanceToGoal = WR::Utils::distance(lastPosition, goalPos());
    return distanceToGoal;
};


QLinkedList<Position> FastPathPlanning::getPath() const{
    return _path;
};

void FastPathPlanning::addColisionPosition(Position _pos, Boolean _isBall){
    _colisionPosition.push_back(std::make_pair(_pos, _isBall));
};

//retorna dois pontos de distancia iteratorPoints*40 com check se estao dentro do campo
std::pair<Position, Position> FastPathPlanning::findPoint(Position colisionPoint, int iteratorPoints){   //acha o novo ponto para desviar do obstáculo
   Position startPoint = _path.back();
   Position endPoint = goalPos();
   QList <Position> allPoints;
   float coefAngular = WR::Utils::getPerpendicularCoefficient(startPoint, endPoint);
   Position newPoint1(true, colisionPoint.x()+iteratorPoints*40*cos(coefAngular), colisionPoint.y()+iteratorPoints*40*sin(coefAngular),0.0);
   Position newPoint2(true, colisionPoint.x()-iteratorPoints*40*cos(coefAngular), colisionPoint.y()-iteratorPoints*40*sin(coefAngular),0.0);

   if(!loc()->isInsideField(newPoint1,0.10)) newPoint1.setInvalid();
   if(!loc()->isInsideField(newPoint2,0.10)) newPoint2.setInvalid();
   return std::make_pair(newPoint1, newPoint2);

}

Position FastPathPlanning::hasCollisionAtLine(){
    bool hasColision = false;
    for (int i=0; i < _colisionPosition.size() && !hasColision; ++i) {
        float distanceLine = WR::Utils::distanceToSegment(_path.back(),goalPos(), _colisionPosition[i].first); // calcula a distancia do ponto ao segmento
        if (!_colisionPosition[i].second && distanceLine<0.38f) { //se a distancia de qualquer jogador ate a reta for menor que 38cm, tem colisao
            hasColision = true;
            return Position(hasColision, _colisionPosition[i].first.x(), _colisionPosition[i].first.y(), 0.0);
        }
        if (_colisionPosition[i].second && distanceLine<0.20f) { // se for bola e a distancia dela ate a linha for menor que 20cm
            hasColision = true;
            return Position(hasColision, _colisionPosition[i].first.x(), _colisionPosition[i].first.y(), 0.0);
        }
    }

    return Position(hasColision, 0.0, 0.0, 0.0);

}













