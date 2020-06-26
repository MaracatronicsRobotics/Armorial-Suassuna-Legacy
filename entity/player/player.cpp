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

#include "player.h"
#include <entity/contromodule/mrcteam.h>
#include <entity/player/playeraccess.h>
#include <entity/player/role/role.h>
#include <entity/player/navigation/navalgorithm.h>
#include <entity/player/navigation/navigation.h>

QString Player::name(){
    return "Player #"+QString::number((int)_team->teamId())+":"+QString::number((int)_playerId);
}

Player::Player(World *world, MRCTeam *team, Controller *ctr, quint8 playerID, Role *defaultRole, SSLReferee *ref, PID *vxPID, PID *vyPID, PID *vwPID, NavigationAlgorithm *navAlg) : Entity(Entity::ENT_PLAYER){
    _world = world;
    _team = team;
    _playerId = playerID;
    _ref = ref;
    _ctr = ctr;
    _role = NULL;
    _defaultRole = defaultRole;
    _nav = new Navigation(this, navAlg);

    _playerAccessSelf = new PlayerAccess(true, this, team->loc());
    _playerAccessBus = new PlayerAccess(false, this, team->loc());

    _vxPID = vxPID;
    _vyPID = vyPID;
    _vwPID = vwPID;

    // Idle control
    _idleCount = 0;

    // Reset player
    reset();
    setPidActivated(true);

}

Player::~Player(){
    if(_defaultRole != NULL)
        delete _defaultRole;
    if(_playerAccessSelf != NULL)
        delete _playerAccessSelf;
    if(_playerAccessBus != NULL)
        delete _playerAccessBus;

    delete _nav;
    delete _vxPID;
    delete _vyPID;
    delete _vwPID;
}

PlayerAccess* Player::access() const {
    return _playerAccessBus;
}

Colors::Color Player::teamColor() const{
    return _team->teamColor();
}

void Player::reset(){
    // Errors
    _lError = 0.015;
    _aError = Angle::toRadians(4.0);
}

/* player info methods */
quint8 Player::playerId() const{
    return _playerId;
}

MRCTeam* Player::playerTeam(){
    return _team;
}

quint8 Player::teamId() const{
    return _team->teamId();
}

quint8 Player::opTeamId() const{
    return _team->teamId() ? 0 : 1;
}

/* Entity inherit virtual methods */
void Player::initialization(){
    printf("[TEAM %d PLAYER %2d] thread started.\n", (int)_team->teamId(), (int)playerId());
}

void Player::loop(){
    if(position().isUnknown()){
        if(_idleCount < IDLE_COUNT){
            _idleCount++;
            idle();
            setRole(NULL);
        }
    }
    else{
        _idleCount = 0;

        _mutexRole.lock();
        if(_role != NULL){
            if(_role->isInitialized() == false){
                _role->initialize(_team, _team->opTeam(), _team->loc(), _ref);
            }
            _role->setPlayer(this, _playerAccessSelf);
            _role->runRole();
        }else if(_defaultRole != NULL){
            if(_defaultRole->isInitialized() == false){
                _defaultRole->initialize(_team, _team->opTeam(), _team->loc(), _ref);
            }
            _defaultRole->setPlayer(this, _playerAccessSelf);
            _defaultRole->runRole();
        }else{
            std::cout << "[ERROR] No role found for player #" << (int)playerId() << "!" << std::endl;
        }
        _mutexRole.unlock();
    }
}

QString Player::getRoleName() {
    _mutexRole.lock();
    QString roleName;
    if(_role == NULL)
        roleName = "UNKNOWN";
    else
        roleName = _role->name();
    _mutexRole.unlock();
    return roleName;
}

QString Player::getActualBehaviourName() {
    _mutexRole.lock();
    QString behaviourName;
    if(_role == NULL)
        behaviourName = "UNKNOWN";
    else
        behaviourName = _role->getBehaviours().value(_role->getActualBehaviour())->name();
    _mutexRole.unlock();
    return behaviourName;
}

void Player::setRole(Role* b) {
    // Check same behavior
    if(b==_role)
        return;
    // Set new
    _mutexRole.lock();
    _role = b;
    this->reset();
    _mutexRole.unlock();
}

void Player::finalization(){
    if(isEnabled()){
        for(int i = 0; i < IDLE_COUNT; i++){
            this->idle();
            this->msleep(this->loopTime()/4);
        }
    }
    printf("[TEAM %d PLAYER %2d] thread ended.\n", (int)_team->teamId(), (int)playerId());
}

/* Auxiliary Methods */

Position Player::position() const{
    return _team->wm()->playerPosition(_team->teamId(), _playerId);
}

Position Player::nextPosition() const{
    return this->_nextPosition;
}

Angle Player::orientation() const{
    return _team->wm()->playerOrientation(_team->teamId(), _playerId);
}

Angle Player::nextOrientation() const{
    return this->_nextOrientation;
}

Velocity Player::velocity() const{
    return _team->wm()->playerVelocity(teamId(), _playerId);
}

AngularSpeed Player::angularSpeed() const{
    return _team->wm()->playerAngularSpeed(teamId(), _playerId);
}

float Player::lastSpeed() const{
    return this->_lastSpeedAbs;
}

Angle Player::nextDirection() const{
    return this->_nextDirection;
}

bool Player::isLookingTo(const Position &pos) const{
    return this->isLookingTo(pos, _aError);
}

bool Player::isLookingTo(const Position &pos, float error) const{
    Angle angle(true, WR::Utils::getAngle(position(), pos));

    // Calc diff
    float dif = fabs(WR::Utils::angleDiff(orientation(), angle));
    return (dif <= error);
}

bool Player::isAtPosition(const Position &pos) const{
    // Get distance
    const float distance = WR::Utils::distance(position(), pos);
    return (distance <= _lError);
}

bool Player::isNearbyPosition(const Position &pos, float error) const{
    const float distance = WR::Utils::distance(position(), pos);

    return (distance <= error);
}

bool Player::hasBallPossession() const{
    return _team->wm()->ballPossession(teamId(), _playerId);
}

bool Player::canKickBall() const{
    return _ref->getGameInfo(_team->teamColor())->canKickBall();
}

float Player::distanceTo(const Position &pos) const{
    return WR::Utils::distance(position(), pos);
}

float Player::distBall() const{
    return WR::Utils::distance(position(), _team->loc()->ball());
}

float Player::distOurGoal() const{
    return WR::Utils::distance(position(), _team->loc()->ourGoal());
}

float Player::distTheirGoal() const{
    return WR::Utils::distance(position(), _team->loc()->theirGoal());
}

Angle Player::angleTo(const Position &pos) const{
    return Angle(true, WR::Utils::getAngle(position(), pos));
}

/* Locomotion algorithms */

std::pair<Angle,float> Player::getNavDirectionDistance(const Position &destination, const Angle &positionToLook, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea) {
    _nav->setGoal(destination, positionToLook, avoidTeammates, avoidOpponents, avoidBall, avoidOurGoalArea, avoidTheirGoalArea);
    Angle direction = _nav->getDirection();
    float distance = _nav->getDistance();

    std::pair<Angle,float> movement = std::make_pair(direction, distance);
    movement.first.setValue(movement.first.value() - orientation().value());
    return movement;
}

void Player::idle(){
    // Set current position/orientation as desired
    //_nextPosition = position();
    //_nextOrientation = orientation();

    // Set speed to 0
    setSpeed(0.0, 0.0, 0.0);

    // Reset navigation algorithm
    //
}

void Player::setSpeed(float x, float y, float theta) {

    float currSpeedAbs = sqrt(pow(x, 2) + pow(y, 2));
    float incSpeedAbs = currSpeedAbs - _lastSpeedAbs;
    float maxAcc = 2.0;

    if(fabs(incSpeedAbs) > maxAcc && incSpeedAbs > 0){
        float newSpeed = _lastSpeedAbs + maxAcc;
        float angle = atan2(y, x);
        x = newSpeed * cos(angle);
        y = newSpeed * sin(angle);
    }

    _lastSpeedAbs = sqrt(pow(x, 2) + pow(y, 2));

    // watchdog on speed
    if(isnan(x)) x = 0.0;
    if(isnan(y)) y = 0.0;
    WR::Utils::limitValue(&x, -2.5, 2.5);
    WR::Utils::limitValue(&y, -2.5, 2.5);

    _ctr->setSpeed((int)_team->teamId(), (int)playerId(), x, y, theta);
    _ctr->kick(_team->teamId(), playerId(), 0.0);

}

std::pair<float, float> Player::goTo(Position targetPosition, double offset, bool setHere, double minVel){
    double robot_x, robot_y, robotAngle = orientation().value();
    robot_x = position().x();
    robot_y = position().y();

    // Define a velocidade do robô para chegar na bola
    long double Vx = (targetPosition.x() - robot_x);
    long double Vy = (targetPosition.y() - robot_y);
    long double theta = robotAngle;
    long double moduloDistancia = sqrt(pow(Vx,2)+pow(Vy,2));
    float vxSaida = (Vx * cos(theta) + Vy * sin(theta));
    float vySaida = (Vy * cos(theta) - Vx * sin(theta));

    // inverte pra dar frenagem
    // na simulação é bom colocar *= 0.0 pra ele realmente "parar" o robô
    if(moduloDistancia <= offset){
        vxSaida *= -1.0;
        vySaida *= -1.0;
    }

    float newVX, newVY;
    if(isPidActivated()){
        newVX = _vxPID->calculate(vxSaida, velocity().x());
        newVY = _vyPID->calculate(vySaida, velocity().y());
    }

    if(isPidActivated()){
        if(setHere){
            // aplicar velocidade minima ( só no intercept que goTo vai ser chamado pra ativar aqui ? )
            if(fabs(newVX) <= minVel){
                if(newVX < 0) newVX = -minVel;
                else newVX = minVel;
            }

            if(fabs(newVY) <= minVel){
                if(newVY < 0) newVY = -minVel;
                else newVY = minVel;
            }

            setSpeed(newVX, newVY, 0.0);
        }
        return std::make_pair(newVX, newVY);
    }
    else{
        if(setHere){
            // aplicar velocidade minima ( só no intercept que goTo vai ser chamado pra ativar aqui ? )
            if(fabs(vxSaida) <= minVel){
                if(vxSaida < 0) vxSaida = -minVel;
                else vxSaida = minVel;
            }

            if(fabs(vySaida) <= minVel){
                if(vySaida < 0) vySaida = -minVel;
                else vySaida = minVel;
            }

            setSpeed(vxSaida, vySaida, 0.0);
        }
        return std::make_pair(vxSaida, vySaida);
    }
}

std::pair<double, double> Player::rotateTo(Position targetPosition, double offset, bool setHere) {
    double robot_x, robot_y, angleOrigin2Robot = orientation().value();
    robot_x = position().x();
    robot_y = position().y();

    // Define a velocidade angular do robô para visualizar a bola
    double vectorRobot2BallX = (targetPosition.x() - robot_x);
    double vectorRobot2BallY = (targetPosition.y() - robot_y);
    double modVectorRobot2Ball = sqrt(pow(vectorRobot2BallX, 2) + pow(vectorRobot2BallY, 2));

    vectorRobot2BallX = vectorRobot2BallX / modVectorRobot2Ball;

    double angleOrigin2ball;   //Ângulo que a bola faz com o eixo x em relação ao robô
    double angleRobot2Ball;    //Ângulo que a visão do robô faz com a posição da bola em relação ao robô

    if(vectorRobot2BallY < 0){ //terceiro e quadrante
        angleOrigin2ball = 2*M_PI - acos(vectorRobot2BallX); //angulo que a bola faz com o eixo x em relação ao robo
    }else{ //primeiro e segundo quadrante
        angleOrigin2ball = acos(vectorRobot2BallX); //angulo que a bola faz com o eixo x em relação ao robo
    }

    double minValue = 3.0;
    double maxValue = 6.0;

    double speed = 0.0;

    angleRobot2Ball = angleOrigin2Robot - angleOrigin2ball;

    if(fabs(angleRobot2Ball) >= GEARSystem::Angle::toRadians(1.5)){ // se a dif for de até 1.5 grau
        if(angleRobot2Ball < 0.0){
            if(angleRobot2Ball < -GEARSystem::Angle::pi) speed = -maxValue;
            else speed = maxValue;
        }else{
            if(angleRobot2Ball < GEARSystem::Angle::pi) speed = -maxValue;
            else speed = maxValue;
        }

        // Se estiver < 20 graus, seta uma minima para desalecerar um pouco
        if(fabs(angleRobot2Ball) < GEARSystem::Angle::toRadians(20)){
            if(angleRobot2Ball < 0.0) speed = (fabs(angleRobot2Ball) / GEARSystem::Angle::toRadians(20)) * minValue;
            else speed = (fabs(angleRobot2Ball) / GEARSystem::Angle::toRadians(20)) * -minValue;
        }
    }else{
        speed = 0.0;
    }


    if(isPidActivated()){
        double newSpeed = _vwPID->calculate(speed, angularSpeed().value());
        if(setHere) setSpeed(0.0, 0.0, newSpeed);
        return std::make_pair(angleRobot2Ball, newSpeed);
    }
    else{
        if(setHere) setSpeed(0.0, 0.0, speed);
        return std::make_pair(angleRobot2Ball, speed);
    }
}

void Player::goToLookTo(Position targetPosition, Position lookToPosition, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea){
    /*
    std::pair<float, float> a = goTo(targetPosition, offset, false);
    std::pair<double, double> b = rotateTo(lookToPosition, offsetAngular, false);

    if(fabs(a.first) <= 0.1){
        if(a.first < 0) a.first = -0.1;
        else a.first = 0.1;
    }

    if(fabs(a.second) <= 0.1){
        if(a.second < 0) a.second = -0.1;
        else a.second = 0.1;
    }

    WR::Utils::limitValue(&a.first, -2.5, 2.5);
    WR::Utils::limitValue(&a.second, -2.5, 2.5);

    double dist = WR::Utils::distance(position(), targetPosition);
    if(dist <= 0.5f){ // se estiver a menos de 50cm do alvo
        if(fabs(b.first) >= GEARSystem::Angle::toRadians(15)){ // se a diferença for maior que 15 deg
            setSpeed(0.0, 0.0, b.second); // zera a linear e espera girar
        }else{
            setSpeed(a.first, a.second, b.second); // caso esteja de boa, gogo
        }
    }
    else if(dist > 0.5f && dist <= 1.0f){ // se estiver entre 50cm a 1m do alvo
        if(fabs(b.first) >= GEARSystem::Angle::toRadians(45)){ // se a diferença for maior que 45 deg
            setSpeed(0.3 * a.first, 0.3 * a.second, b.second); // linear * 0.3 e gira
        }else{
            setSpeed(a.first, a.second, b.second); // caso esteja de boa, gogo
        }
    }
    else if(dist > 1.0f){ // se estiver a mais de 1m do alvo
        if(fabs(b.first) >= GEARSystem::Angle::toRadians(75)){ // se a diferença for maior que 75 deg
            setSpeed(0.5 * a.first, 0.5 * a.second, b.second); // linear * 0.5 e gira
        }else{
            setSpeed(a.first, a.second, b.second); // caso esteja de boa, gogo
        }
    }
    */

    Angle anglePP;
    std::pair<double, double> help = rotateTo(targetPosition);
    if(lookToPosition.isUnknown())
        anglePP = Angle(false, 0.0);
    else
        anglePP = Angle(true, help.first);

    std::pair<double, double> rotateSpeed = rotateTo(lookToPosition);
    std::pair<Angle, float> a = getNavDirectionDistance(targetPosition, anglePP, avoidTeammates, avoidOpponents, avoidBall, avoidOurGoalArea, avoidTheirGoalArea);
/*
    double vx = _vxPID->calculate(a.second * cos(a.first.value()), velocity().x());
    double vy = _vyPID->calculate(a.second * sin(a.first.value()), velocity().y());
*/
    double vx = a.second * cos(a.first.value());
    double vy = a.second * sin(a.first.value());
    double dist = WR::Utils::distance(position(), targetPosition);
    if(dist <= 0.5f){ // se estiver a menos de 50cm do alvo
        if(fabs(rotateSpeed.first) >= GEARSystem::Angle::toRadians(15)){ // se a diferença for maior que 15 deg
            setSpeed(0.0, 0.0, rotateSpeed.second); // zera a linear e espera girar
        }else{
            setSpeed(vx, vy, rotateSpeed.second); // caso esteja de boa, gogo
        }
    }
    else if(dist > 0.5f && dist <= 1.0f){ // se estiver entre 50cm a 1m do alvo
        if(fabs(rotateSpeed.first) >= GEARSystem::Angle::toRadians(45)){ // se a diferença for maior que 45 deg
            setSpeed(0.3 * vx, 0.3 * vy, rotateSpeed.second); // linear * 0.3 e gira
        }else{
            setSpeed(vx, vy, rotateSpeed.second); // caso esteja de boa, gogo
        }
    }
    else if(dist > 1.0f){ // se estiver a mais de 1m do alvo
        if(fabs(rotateSpeed.first) >= GEARSystem::Angle::toRadians(75)){ // se a diferença for maior que 75 deg
            setSpeed(0.5 * vx, 0.5 * vy, rotateSpeed.second); // linear * 0.5 e gira
        }else{
            setSpeed(vx, vy, rotateSpeed.second); // caso esteja de boa, gogo
        }
    }
}

void Player::aroundTheBall(Position targetPosition, double offset, double offsetAngular){
    double robot_x, robot_y;
    robot_x = position().x();
    robot_y = position().y();
    // Configura o robô para ir até a bola e girar em torno dela
    goTo(targetPosition, offset, true);
    rotateTo(targetPosition, offsetAngular, true);
}

void Player::kick(float power, bool isChipKick){
    if(!isChipKick){
        _ctr->kick(_team->teamId(), playerId(), power);
    }
    else{
        _ctr->chipKick(_team->teamId(), playerId(), power); // rever esse power dps
    }
}

void Player::setGoal(Position pos){
    _nav->setGoal(pos, orientation(), true, true, false, true, true);
}

QLinkedList<Position> Player::getPath() const {
    return _nav->getPath();
}

void Player::dribble(bool isActive){
    _ctr->holdBall(_team->teamId(), playerId(), isActive);
}
