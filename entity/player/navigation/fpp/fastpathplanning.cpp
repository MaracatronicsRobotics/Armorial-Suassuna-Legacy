#include "fastpathplanning.h"

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

void FastPathPlanning::run(){


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


std::pair<Position, Position> FastPathPlanning::findAnotherPoint(Position colisionPoint){   //acha o novo ponto para desviar do obstáculo
   Position startPoint = _path.back();
   Position endPoint = goalPos();
   float coefAngular = WR::Utils::getPerpendicularCoefficient(startPoint, endPoint);

   Position newPoint1(true, colisionPoint.x()+40*cos(coefAngular), colisionPoint.y()+40*sin(coefAngular),0.0);
   Position newPoint2(true, colisionPoint.x()-40*cos(coefAngular), colisionPoint.y()-40*sin(coefAngular),0.0);
   return std::make_pair(newPoint1, newPoint2);

}

Position FastPathPlanning::hasCollision(){


    return Position(false, 0.0, 0.0, 0.0);

}













