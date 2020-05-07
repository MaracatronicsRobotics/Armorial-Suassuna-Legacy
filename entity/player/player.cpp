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

Player::Player(World *world, MRCTeam *team, Controller *ctr, quint8 playerID, Role *defaultRole, SSLReferee *ref, PID *vxPID, PID *vyPID, PID *vwPID, NavAlgorithm *navAlg) : Entity(Entity::ENT_PLAYER){
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

    _kalman = new KalmanFilter2D();
    _kalman->setEnabled(true);

    // Idle control
    _idleCount = 0;

    // Reset player
    reset();

}

Player::~Player(){
    if(_defaultRole != NULL)
        delete _defaultRole;
    if(_playerAccessSelf != NULL)
        delete _playerAccessSelf;
    if(_playerAccessBus != NULL)
        delete _playerAccessBus;
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
        // kalman for PID precision
        _kalman->iterate(position());
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
    float maxAcc = 0.5;

    if(fabs(incSpeedAbs) > maxAcc && incSpeedAbs > 0){
        float newSpeed = _lastSpeedAbs + maxAcc;
        float angle = atan2(y, x);
        x = newSpeed * cos(angle);
        y = newSpeed * sin(angle);
    }

    _lastSpeedAbs = sqrt(pow(x, 2) + pow(y, 2));

    // watchdog on speed
    WR::Utils::limitValue(&x, -2.5, 2.5);
    WR::Utils::limitValue(&y, -2.5, 2.5);

    _ctr->setSpeed((int)_team->teamId(), (int)playerId(), x, y, theta);
    _ctr->kick(_team->teamId(), playerId(), 0.0);

}

std::pair<float, float> Player::goTo(Position targetPosition, double offset){
    Position robot_pos_filtered = getKalmanPredict();
    double robot_x, robot_y, robotAngle = orientation().value();
    if(robot_pos_filtered.isUnknown()){
        robot_x = position().x();
        robot_y = position().y();
    }else{
        robot_x = robot_pos_filtered.x();
        robot_y = robot_pos_filtered.y();
    }
    // Define a velocidade do robô para chegar na bola
    long double Vx = (targetPosition.x() - robot_x);
    long double Vy = (targetPosition.y() - robot_y);
    long double theta = robotAngle;
    long double moduloDistancia = sqrt(pow(Vx,2)+pow(Vy,2));
    float vxSaida = (Vx * cos(theta) + Vy * sin(theta));
    float vySaida = (Vy * cos(theta) - Vx * sin(theta));
    double sinal_x = 1;
    double sinal_y = 1;

    if(vxSaida < 0) sinal_x = -1;
    if(vySaida < 0) sinal_y = -1;

    // inverte pra dar frenagem
    if(moduloDistancia <= offset){
        vxSaida *= -1;
        vySaida *= -1;
    }

    float newVX = _vxPID->calculate(vxSaida, velocity().x());
    float newVY = _vyPID->calculate(vySaida, velocity().y());

    return std::make_pair(newVX, newVY);
}

std::pair<double, double> Player::rotateTo(Position targetPosition, double offset) {
    Position robot_pos_filtered = getKalmanPredict();
    double robot_x, robot_y, angleOrigin2Robot = orientation().value();
    if(robot_pos_filtered.isUnknown()){
        robot_x = position().x();
        robot_y = position().y();
    }else{
        robot_x = robot_pos_filtered.x();
        robot_y = robot_pos_filtered.y();
    }

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

    double minValue = 1.5;
    double maxValue = 3.0;

    double speed = 0.0;

    angleRobot2Ball = angleOrigin2Robot - angleOrigin2ball;

    if(fabs(angleRobot2Ball) >= M_PI / 52.0){
        if(abs(angleRobot2Ball) < minValue){
            if(angleRobot2Ball < 0.0){
                if (speed != 0.0 && angleRobot2Ball < 0.2) speed = -minValue;    //Inverte a velocidade para frenagem
                else speed = minValue;
                speed = minValue;
            }else{
                if (speed != 0.0 && angleRobot2Ball < 0.2) speed = minValue;     //Inverte a velocidade para frenagem
                else speed = -minValue;
                speed = -minValue;
            }
        }else{
            if(angleRobot2Ball < 0.0){
                if(angleRobot2Ball < -M_PI) speed = -maxValue;
                else speed = maxValue;
            }else{
                if(angleRobot2Ball < M_PI) speed = -maxValue;
                else speed = maxValue;
            }
        }
    }else{
        speed = 0;
    }

    double newSpeed = _vwPID->calculate(speed, angularSpeed().value());

    if(angleRobot2Ball <= offset){
        return std::make_pair(angleRobot2Ball, -newSpeed);
    }

    return std::make_pair(angleRobot2Ball, newSpeed);
}

void Player::goToLookTo(Position targetPosition, Position lookToPosition, double offset, double offsetAngular){
    Position robot_pos_filtered = getKalmanPredict();
    double robot_x, robot_y;
    if(robot_pos_filtered.isUnknown()){
        robot_x = position().x();
        robot_y = position().y();
    }else{
        robot_x = robot_pos_filtered.x();
        robot_y = robot_pos_filtered.y();
    }
    // Configura o robô para ir até a bola e olhar para um alvo
    std::pair<float, float> a;
    double p_x, p_y, angle, moduloDist, final_x, final_y;

    if (targetPosition.x() == lookToPosition.y()) angle = 1.570796327;
    else angle = atan((targetPosition.y() - lookToPosition.y())/(targetPosition.x() - lookToPosition.x()));
    if (lookToPosition.x() > targetPosition.x()) {
        p_y = targetPosition.y() - offset * sin(angle);
        p_x = targetPosition.x() - offset * cos(angle);
    } else {
        p_y = targetPosition.y() + offset * sin(angle);
        p_x = targetPosition.x() + offset * cos(angle);
    }
    moduloDist = sqrt(pow((p_x - robot_x), 2) + pow((p_y - robot_y), 2));
    final_x = (p_x - robot_x)/moduloDist;
    final_y = (p_y - robot_y)/moduloDist;
    a = goTo(Position(true, p_x + offset * final_x,p_y + offset * final_y, 0.0 ), offset);
    double theta = rotateTo(lookToPosition, offsetAngular).second;

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

    //if (moduloDist < offset) setSpeed(0, 0.2, theta); //3% de diferença nas velocidades
    setSpeed(a.first, a.second, theta);
}

void Player::aroundTheBall(Position targetPosition, double offset, double offsetAngular){
    Position robot_pos_filtered = getKalmanPredict();
    double robot_x, robot_y, robotAngle = orientation().value();
    if(robot_pos_filtered.isUnknown()){
        robot_x = position().x();
        robot_y = position().y();
    }else{
        robot_x = robot_pos_filtered.x();
        robot_y = robot_pos_filtered.y();
    }
    // Configura o robô para ir até a bola e girar em torno dela
    std::pair<float, float> a;
    long double moduloDistancia = sqrt(pow((targetPosition.x() - robot_x),2)+pow((targetPosition.y() - robot_y),2));
    a = goTo(targetPosition, offset);
    float theta = rotateTo(targetPosition, offsetAngular).second;

}

void Player::kick(bool isPass, float kickZPower){
    if(isPass){
        _ctr->kick(_team->teamId(), playerId(), 2.0);
        if(kickZPower > 0.0){
            _ctr->chipKick(_team->teamId(), playerId(), 2.0); // rever esse power dps
        }
    }
    else{
        _ctr->kick(_team->teamId(), playerId(), 6.0);
        if(kickZPower > 0.0){
            _ctr->chipKick(_team->teamId(), playerId(), 6.0); // rever esse power dps
        }
    }
}

void Player::setGoal(Position pos){
    _nav->setGoal(pos, orientation(), true, true, false, true, true);
}

QList<Position> Player::getPath() const {
    return _nav->getPath();
}

void Player::dribble(bool isActive){
    _ctr->holdBall(_team->teamId(), playerId(), isActive);
}

Position Player::getKalmanPredict(){
    _kalman->predict();
    return _kalman->getPosition();
}
