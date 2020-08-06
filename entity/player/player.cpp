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

Player::Player(World *world, MRCTeam *team, Controller *ctr, quint8 playerID, Role *defaultRole, SSLReferee *ref, PID *vxPID, PID *vyPID, PID *vwPID, NavigationAlgorithm *navAlg, MRCConstants *mrcconstants) : Entity(Entity::ENT_PLAYER){
    _world = world;
    _team = team;
    _playerId = playerID;
    _ref = ref;
    _ctr = ctr;
    _role = NULL;
    _defaultRole = defaultRole;

    _mrcconstants=mrcconstants;
    if(_mrcconstants == NULL){
        std::cout<<"NULOOOOOOOOOOOOOOOOOOOO"<<std::endl;
    }
    _nav = new Navigation(this, navAlg, getConstants());

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
    _aError = Angle::toRadians(1.5);
}

/* player info methods */
double Player::aError(){
    return _aError;
}

double Player::lError(){
    return _lError;
}

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
    std::string teamColor = this->teamColor() == Colors::BLUE ? "BLUE" : "YELLOW";
    std::string teamColorCmd = this->teamColor() == Colors::BLUE ? MRCConstants::blue : MRCConstants::yellow;
    printf("%s[TEAM %s%s%s%s PLAYER %2d] %sThread started.\n%s", MRCConstants::defaultBold.c_str(), teamColorCmd.c_str(), teamColor.c_str(), MRCConstants::reset.c_str(), MRCConstants::defaultBold.c_str(), (int)playerId(), MRCConstants::green.c_str(), MRCConstants::reset.c_str());
}

void Player::loop(){
    if(getConstants()==NULL) return;
    if(position().isUnknown()){
        if(_idleCount < IDLE_COUNT){
            _idleCount++;
            idle();
            setRole(NULL);
        }
    }
    else{
        // Reset idle count
        _idleCount = 0;
        // Disable kick
        if(_ctr != NULL){ // avoid set on enemy players
            _ctr->kickOnTouch(teamId(), playerId(), false, 0.0);
            _ctr->chipKickOnTouch(teamId(), playerId(), false, 0.0);
        }

        _mutexRole.lock();
        if(_role != NULL){
            if(_role->isInitialized() == false){
                _role->initialize(_team, _team->opTeam(), _team->loc(), _ref, getConstants());
            }
            _role->setPlayer(this, _playerAccessSelf);
            _role->runRole();
        }else if(_defaultRole != NULL){
            if(_defaultRole->isInitialized() == false){
                _defaultRole->initialize(_team, _team->opTeam(), _team->loc(), _ref, getConstants());
            }
            _defaultRole->setPlayer(this, _playerAccessSelf);
            _defaultRole->runRole();
        }else{
            std::cout << MRCConstants::red << "[ERROR] " << MRCConstants::reset << "No role found for player #" << (int)playerId() << "!" << std::endl;
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
    std::string teamColor = this->teamColor() == Colors::BLUE ? "BLUE" : "YELLOW";
    std::string teamColorCmd = this->teamColor() == Colors::BLUE ? MRCConstants::blue : MRCConstants::yellow;
    printf("%s[TEAM %s%s%s%s PLAYER %2d] %sThread ended.\n%s", MRCConstants::defaultBold.c_str(), teamColorCmd.c_str(), teamColor.c_str(), MRCConstants::reset.c_str(), MRCConstants::defaultBold.c_str(), (int)playerId(), MRCConstants::green.c_str(), MRCConstants::reset.c_str());
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

Position Player::limitFieldDimensions(Position destination) {
    const Locations *loc = playerTeam()->loc();

    // X min
    if(destination.x() < loc->fieldMinX()-getConstants()->getRobotRadius()) {
        destination.setPosition(loc->fieldMinX()-getConstants()->getRobotRadius(), destination.y(), 0.0);
    }
    // X max
    if(destination.x() > loc->fieldMaxX()+getConstants()->getRobotRadius()) {
        destination.setPosition(loc->fieldMaxX()+getConstants()->getRobotRadius(), destination.y(), 0.0);
    }

    // Y min
    if(destination.y() < loc->fieldMinY()-getConstants()->getRobotRadius()) {
        destination.setPosition(destination.x(), loc->fieldMinY()-getConstants()->getRobotRadius(), 0.0);
    }

    // Y max
    if(destination.y() > loc->fieldMaxY()+getConstants()->getRobotRadius()) {
        destination.setPosition(destination.x(), loc->fieldMaxY()+getConstants()->getRobotRadius(), 0.0);
    }

    return destination;
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
    _nextPosition = position();
    _nextOrientation = orientation();

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
    if(isnan(x)) x = 0.0;
    if(isnan(y)) y = 0.0;

    // Limit max speed
    float maxSpeed = 3.0;
    Velocity robotVel = Velocity(true, x, y);
    if(robotVel.abs() >= maxSpeed){
        // Transform in unitary vector
        robotVel.setVelocity(robotVel.x() / robotVel.abs(), robotVel.y() / robotVel.abs());
        // Multiply by minVel
        robotVel.setVelocity(robotVel.x() * maxSpeed, robotVel.y() * maxSpeed);
    }

    if(_ctr != NULL){ // avoid set on enemy players
        _ctr->setSpeed((int)_team->teamId(), (int)playerId(), robotVel.x(), robotVel.y(), theta);
    }
}

std::pair<float, float> Player::goTo(Position targetPosition, double offset, bool setHere, double minVel){
    // Update next pos
    _nextPosition = targetPosition;

    targetPosition = limitFieldDimensions(targetPosition);
    double robot_x, robot_y, robotAngle = orientation().value();
    robot_x = position().x();
    robot_y = position().y();

    // Define a velocidade do robô para chegar na bola
    long double Dx = (targetPosition.x() - robot_x);
    long double Dy = (targetPosition.y() - robot_y);
    long double theta = robotAngle;
    float vxSaida = (Dx * cos(theta) + Dy * sin(theta));
    float vySaida = (Dy * cos(theta) - Dx * sin(theta));

    Velocity robotVel = Velocity(true, vxSaida, vySaida);
    if(robotVel.abs() <= minVel){
        // Transform in unitary vector
        robotVel.setVelocity(robotVel.x() / robotVel.abs(), robotVel.y() / robotVel.abs());
        // Multiply by minVel
        robotVel.setVelocity(robotVel.x() * minVel, robotVel.y() * minVel);
    }

    /*
    // inverte pra dar frenagem
    // na simulação é bom colocar *= 0.0 pra ele realmente "parar" o robô
    if(moduloDistancia <= offset){
        vxSaida *= -1.0;
        vySaida *= -1.0;
    }
    */

    float newVX, newVY;
    if(isPidActivated()){
        newVX = _vxPID->calculate(robotVel.x(), velocity().x());
        newVY = _vyPID->calculate(robotVel.y(), velocity().y());
        robotVel.setVelocity(newVX, newVY);
    }

    if(setHere)
        setSpeed(robotVel.x(), robotVel.y(), 0.0);

    return std::make_pair(robotVel.x(), robotVel.y());
}

double Player::getPlayerRotateAngleTo(const Position &pos){
    double robot_x, robot_y, angleOrigin2Robot = orientation().value();
    robot_x = position().x();
    robot_y = position().y();

    // Define a velocidade angular do robô para visualizar a bola
    double vectorRobot2BallX = (pos.x() - robot_x);
    double vectorRobot2BallY = (pos.y() - robot_y);
    double modVectorRobot2Ball = sqrt(pow(vectorRobot2BallX, 2) + pow(vectorRobot2BallY, 2));

    vectorRobot2BallX = vectorRobot2BallX / modVectorRobot2Ball;

    if(isnan(vectorRobot2BallX)) return 0.0;

    double angleOrigin2ball;   //Ângulo que a bola faz com o eixo x em relação ao robô
    double angleRobot2Ball;    //Ângulo que a visão do robô faz com a posição da bola em relação ao robô

    if(vectorRobot2BallY < 0){ //terceiro e quadrante
        angleOrigin2ball = 2*M_PI - acos(vectorRobot2BallX); //angulo que a bola faz com o eixo x em relação ao robo
    }else{ //primeiro e segundo quadrante
        angleOrigin2ball = acos(vectorRobot2BallX); //angulo que a bola faz com o eixo x em relação ao robo
    }

    angleRobot2Ball = angleOrigin2ball - angleOrigin2Robot;

    if(angleRobot2Ball > M_PI) angleRobot2Ball -= 2.0 * M_PI;
    if(angleRobot2Ball < -M_PI) angleRobot2Ball += 2.0 * M_PI;

    return angleRobot2Ball;
}

std::pair<double, double> Player::rotateTo(Position targetPosition, double offset, bool setHere) {
    double angleRobotToObjective = getPlayerRotateAngleTo(targetPosition);
    double speed;

    speed = 5.0 * angleRobotToObjective;

    if(isPidActivated()){
        double newSpeed = _vwPID->calculate(speed, angularSpeed().value());
        if(setHere) setSpeed(0.0, 0.0, newSpeed);
        return std::make_pair(angleRobotToObjective, newSpeed);
    }
    else{
        if(setHere) setSpeed(0.0, 0.0, speed);
        return std::make_pair(angleRobotToObjective, speed);
    }
}

void Player::goToLookTo(Position targetPosition, Position lookToPosition, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidOurGoalArea, bool avoidTheirGoalArea, double minVel, bool isGk){
    // Update next pos
    _nextPosition = targetPosition;

    targetPosition = limitFieldDimensions(targetPosition);
    Angle anglePP;
    std::pair<double, double> help = rotateTo(targetPosition, 0.2, false);
    if(targetPosition.isUnknown())
        anglePP = Angle(false, 0.0);
    else
        anglePP = Angle(true, help.first);

    std::pair<double, double> rotateSpeed = rotateTo(lookToPosition, 0.2, false);
    std::pair<Angle, float> a = getNavDirectionDistance(targetPosition, anglePP, avoidTeammates, avoidOpponents, avoidBall, avoidOurGoalArea, avoidTheirGoalArea);
/*
    double vx = _vxPID->calculate(a.second * cos(a.first.value()), velocity().x());
    double vy = _vyPID->calculate(a.second * sin(a.first.value()), velocity().y());
*/

    if(a.second >= 2.0f)
        a.second *= 3.0;
    else
        a.second *= 1.5;

    double vx = a.second * cos(a.first.value());
    double vy = a.second * sin(a.first.value());
    double dist = WR::Utils::distance(position(), targetPosition);

    Velocity robotVel = Velocity(true, vx, vy);
    if(minVel != 0.0 && robotVel.abs() <= minVel){
        // Transform in unitary vector
        robotVel.setVelocity(robotVel.x() / robotVel.abs(), robotVel.y() / robotVel.abs());
        // Multiply by minVel
        robotVel.setVelocity(robotVel.x() * minVel, robotVel.y() * minVel);
    }

    vx = robotVel.x();
    vy = robotVel.y();

    if(isGk){
        setSpeed(vx, vy, rotateSpeed.second);
        return;
    }

    if(dist > 0.5f && dist <= 1.0f)     rotateSpeed.second *= 0.8f;
    else if(dist > 1.0f && dist < 2.0f) rotateSpeed.second *= 0.6f;
    else if(dist >= 2.0f)               rotateSpeed.second *= 0.4f;

    setSpeed(vx, vy, rotateSpeed.second);
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
    if(_ctr != NULL){ // avoid set on enemy players
        if(!isChipKick){
            _ctr->kickOnTouch(_team->teamId(), playerId(), true, power);
        }
        else{
            _ctr->chipKickOnTouch(_team->teamId(), playerId(), true, power); // rever esse power dps
        }
    }
}

void Player::setGoal(Position pos){
    _nav->setGoal(pos, orientation(), true, true, false, true, true);
}

QLinkedList<Position> Player::getPath() const {
    return _nav->getPath();
}

void Player::dribble(bool isActive){
    if(_ctr != NULL){ // avoid set on enemy players
        _ctr->holdBall(_team->teamId(), playerId(), isActive);
    }
}

MRCConstants *Player::getConstants() {
    if(_mrcconstants==NULL)
        std::cout << MRCConstants::red << "[ERROR] " << MRCConstants::reset << name().toStdString() << ", requesting getConstants(), _mrcconstants not initialized!\n";
    return _mrcconstants;
}
