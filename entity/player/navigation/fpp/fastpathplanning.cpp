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

#include "fastpathplanning.h"

FastPathPlanning::FastPathPlanning()
{
    _smoothPathResolution = MRCConstants::_FPPSmoothPathResolution;
}

FastPathPlanning::~FastPathPlanning(){

}

NavAlgorithm* FastPathPlanning::clone() const{
    return new FastPathPlanning(*this);
};

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

void FastPathPlanning::run(){ // chama p recursão
    QList<Position> list;
    list.push_back(originPos());
    _allPath.clear();

    // offset pra n dar merda
    if(loc()->isInsideOurArea(goalPos(), 1.1)){
        double offSet = loc()->ourSide().isLeft() ? (1.2 - abs(loc()->ourGoal().x() - goalPos().x())) : -(1.2 - abs(loc()->ourGoal().x() - goalPos().x()));
        setGoal(Position(true, goalPos().x() + offSet, goalPos().y(), 0.0), Angle(false, 0));
    }

    if(loc()->isInsideTheirArea(goalPos(), 1.1)){
        double offSet = loc()->theirSide().isLeft() ? (1.2 - abs(loc()->theirGoal().x() - goalPos().x())) : -(1.2 - abs(loc()->theirGoal().x() - goalPos().x()));
        setGoal(Position(true, goalPos().x() + offSet, goalPos().y(), 0.0), Angle(false, 0));
    }

    MRCTimer* timer = new MRCTimer(0.1);

    getPaths(list, 1, timer);

    double best_dist = 1e9;
    QList<Position> best_path;

    for(int x = 0; x < _allPath.size(); x++){
        double sumDist = 0.0;
        for(int y = 0; y < _allPath[x].size() - 1; y++){
            sumDist += WR::Utils::distance(_allPath[x][y], _allPath[x][y+1]);
        }
        if(sumDist < best_dist){
            best_dist = sumDist;
            best_path = _allPath[x];
        }
    }

    if(!best_path.isEmpty()){
        if(best_path.size() > 2){
            smoothPath(best_path);
            _path = _smoothPath;
        }else{
            _path = best_path;
        }
    }

};

bool FastPathPlanning::getPaths(QList<Position> &rec, int h, MRCTimer *timer){
    if(timer->checkTimerEnd()){
        _allPath.push_back(rec);
        return true;
    }
    Position collision = hasCollisionAtLine(rec.last());

    if(!collision.isUnknown() && !timer->checkTimerEnd()){ // se tiver colisao do ultimo ponto ate o objetivo
        std::pair<Position, Position> pp = findPoint(rec, collision, h); // pego os dois pontos
        if(loc()->isInsideOurArea(pp.first, 1.1) || loc()->isInsideTheirArea(pp.first, 1.1)){
            pp.first.setInvalid();
        }
        if(loc()->isInsideOurArea(pp.second, 1.1) || loc()->isInsideTheirArea(pp.second, 1.1)){
            pp.second.setInvalid();
        }

        if(!pp.first.isValid() && !pp.second.isValid()){
            for(int x = 1; x <= 3; x++){
                pp = findPoint(rec, collision, x);
                if(loc()->isInsideOurArea(pp.first, 1.1) || loc()->isInsideTheirArea(pp.first, 1.1)){
                    pp.first.setInvalid();
                }
                if(loc()->isInsideOurArea(pp.second, 1.1) || loc()->isInsideTheirArea(pp.second, 1.1)){
                    pp.second.setInvalid();
                }
                if(pp.first.isValid() || pp.second.isValid()){
                    break;
                }
            }
        }

        if(pp.first.isValid() && !timer->checkTimerEnd()){ // se o primeiro ponto for valido (em campo)
            QList<Position> aux = rec; // salvo o meu vetor ate agora
            aux.push_back(pp.first); // coloco no novo vetor o meu novo ponto
            getPaths(aux, h, timer); // chamo a recursao
        }

        if(pp.second.isValid() && !timer->checkTimerEnd()){ // se o segundo ponto for valido (em campo)
            QList<Position> aux = rec; // salvo o meu vetor ate agora
            aux.push_back(pp.second); // coloco no novo vetor o meu novo ponto
            getPaths(aux, h, timer); // chamo a recursão
        }

        if((!pp.first.isValid() && !pp.second.isValid()) || timer->checkTimerEnd()){
            return true;
        }

    }else{
        rec.push_back(goalPos()); // salvo goalPosition
        _allPath.push_back(rec); // salvo no vetor de _paths gerados

        return true;
    }

    return true;
}

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


QList<Position> FastPathPlanning::getPath() const{
    return _path;
};

QList<QList<Position>> FastPathPlanning::getAllPath() const{
    return _allPath;
}

void FastPathPlanning::addColisionPosition(Position _pos, Boolean _isBall){
    _colisionPosition.push_back(std::make_pair(_pos, _isBall));
};

//retorna dois pontos de distancia iteratorPoints*0.4 com check se estao dentro do campo
std::pair<Position, Position> FastPathPlanning::findPoint(QList<Position> &list, Position colisionPoint, int iteratorPoints){   //acha o novo ponto para desviar do obstáculo
   Position startPoint = list.back();
   Position endPoint = goalPos();
   QList <Position> allPoints;
   float coefAngular = WR::Utils::getPerpendicularCoefficient(startPoint, endPoint);
   Position newPoint1(true, colisionPoint.x()+iteratorPoints*MRCConstants::_FPPBreakDistance*cos(coefAngular), colisionPoint.y()+iteratorPoints*MRCConstants::_FPPBreakDistance*sin(coefAngular),0.0);
   Position newPoint2(true, colisionPoint.x()-iteratorPoints*MRCConstants::_FPPBreakDistance*cos(coefAngular), colisionPoint.y()-iteratorPoints*MRCConstants::_FPPBreakDistance*sin(coefAngular),0.0);

   if(!loc()->isInsideField(newPoint1,1.1)) newPoint1.setInvalid();
   if(!loc()->isInsideField(newPoint2,1.1)) newPoint2.setInvalid();

   return std::make_pair(newPoint1, newPoint2);
}

Position FastPathPlanning::hasCollisionAtLine(Position pos){
    bool hasColision = false;
    for (int i=0; i < _colisionPosition.size() && !hasColision; ++i) {
        float distanceLine = WR::Utils::distanceToSegment(pos, goalPos(), _colisionPosition[i].first); // calcula a distancia do ponto ao segmento
        if (!_colisionPosition[i].second && distanceLine<MRCConstants::_FPPRobotThreshHold) { //se a distancia de qualquer jogador ate a reta for menor que 38cm, tem colisao
            hasColision = true;
            return Position(hasColision, _colisionPosition[i].first.x(), _colisionPosition[i].first.y(), 0.0);
        }
        if (_colisionPosition[i].second && distanceLine<MRCConstants::_FPPBallThreshHold) { // se for bola e a distancia dela ate a linha for menor que 20cm
            hasColision = true;
            return Position(hasColision, _colisionPosition[i].first.x(), _colisionPosition[i].first.y(), 0.0);
        }
    }

    return Position(hasColision, 0.0, 0.0, 0.0);
}

Position FastPathPlanning::getVector(const Position &near, const Position &rand, float vectorLength) {
    float theta = WR::Utils::getAngle(near, rand);
    return Position(true, vectorLength*cos(theta), vectorLength*sin(theta), 0.0);
}

void FastPathPlanning::smoothPath(QList<Position> &path) {
    QList<Position> tmp;
    // Check line smoothing (trivial case)
    if(path.size()==2 && WR::Utils::distance(path.first(), path.last()) > 2*_smoothPathResolution) {
        // Calc num points to add
        int numPoints = WR::Utils::distance(path.first(), path.last())/_smoothPathResolution;
        Position vec = getVector(path.first(), path.last(), _smoothPathResolution);
        // Add points
        for(int i=0; i<numPoints; i++) {
            Position lastPos = path.at(path.size()-2);
            Position newPos(true, lastPos.x() + vec.x(), lastPos.y() + vec.y(), 0.0);
            path.insert(path.size()-1, newPos);
        }
        return;
    }
    // Clear _smoothPath
    if(!_smoothPath.isEmpty()) { _smoothPath.clear(); }
    // Copy path to smooth path
    for(int i=0;i<path.length();i++) {
        _smoothPath.append(path.at(i));
    }
    // Apply the smoothing algorithm
    bool keepRunning = true;
    while(keepRunning) {
        keepRunning = false;
        // Append first position to tmp
        tmp.append(_smoothPath.first());
        // Go through all point except the extremes
        for(int i=1; i<_smoothPath.size()-1; i++) {
            float distA = WR::Utils::distance(_smoothPath.at(i), _smoothPath.at(i-1));
            float distB = WR::Utils::distance(_smoothPath.at(i), _smoothPath.at(i+1));
            // If the segments are smaller than the smooth resolution, keep them
            if(distA <= _smoothPathResolution && distB <= _smoothPathResolution) {
                tmp.append(_smoothPath.at(i));
                continue;
            }
            // Keep running while still at least one segment larger than smooth path
            keepRunning = true;
            // Apply smoothing on the first segment (i-1 to i)
            if(distA > _smoothPathResolution) {
                // If the segment is too long, limit by smooth resolution, else limit by half of distance
                float smoothDistance = (distA > 2*_smoothPathResolution)? _smoothPathResolution : distA/2;
                // Create new point
                Position vec = getVector(_smoothPath.at(i), _smoothPath.at(i-1), smoothDistance);
                Position newPoint(true, _smoothPath.at(i).x() + vec.x(), _smoothPath.at(i).y() + vec.y(), 0.0);
                // Append
                tmp.append(newPoint);
            }
            // If one of the segments are too close, do
            if(distA <= _smoothPathResolution || distB <= _smoothPathResolution)
                tmp.append(_smoothPath.at(i));
            // Apply smoothing on the second segment (i to i+1)
            if(distB > _smoothPathResolution) {
                // If the segment is too long, limit by smooth resolution, else limit by half of distance
                float smoothDistance = (distB > 2*_smoothPathResolution)? _smoothPathResolution : distB/2;
                // Create new point
                Position vec = getVector(_smoothPath.at(i), _smoothPath.at(i+1), smoothDistance);
                Position newPoint(true, _smoothPath.at(i).x() + vec.x(), _smoothPath.at(i).y() + vec.y(), 0.0);
                // Append
                tmp.append(newPoint);
            }
        }
        // Append the last point to tmp
        tmp.append(_smoothPath.last());
        // Remove all points of path
        _smoothPath.clear();
        // Copy tmp to path
        for(int i=0;i<tmp.length();i++) {
            _smoothPath.append(tmp.at(i));
        }
        // Clear tmp
        tmp.clear();
    }
}










