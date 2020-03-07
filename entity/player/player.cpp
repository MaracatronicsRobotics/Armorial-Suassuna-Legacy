#include "player.h"
#include <entity/contromodule/mrcteam.h>
#include <entity/player/playeraccess.h>
#include <entity/player/role/role.h>

#include <entity/contromodule/grsSimulator/grsSimulator.h>

QString Player::name(){
    return "Player #"+QString::number((int)_team->teamId())+":"+QString::number((int)_playerId);
}

Player::Player(World *world, MRCTeam *team, Controller *ctr, quint8 playerID, Role *defaultRole, SSLReferee *ref, grsSimulator *grSim, PID *vxPID, PID *vyPID, PID *vwPID) : Entity(Entity::ENT_PLAYER){
    _world = world;
    _team = team;
    _playerId = playerID;
    _ref = ref;
    _ctr = ctr;
    _role = NULL;
    _defaultRole = defaultRole;

    _playerAccessSelf = new PlayerAccess(true, this, team->loc());
    _playerAccessBus = new PlayerAccess(false, this, team->loc());

    _vxPID = vxPID;
    _vyPID = vyPID;
    _vwPID = vwPID;

    // Idle control
    _idleCount = 0;

    // Reset player
    reset();

    // grSimulator for test
    _grSim = grSim;
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
    // Lock for read wm
    _world->wmLockRead();

    if(position().isUnknown()){
        if(_idleCount < IDLE_COUNT){
            _idleCount++;
            idle();
        }
    }
    else{
        _idleCount = 0;

        _mutexRole.lock();
        if(_role != NULL){
            if(_role->isInitialized() == false){
                _role->initialize(_world->ourTeam(), _world->theirTeam(), _team->loc(), _ref);
            }
            _role->setPlayer(this, _playerAccessSelf);
            _role->runRole();
        }else if(_defaultRole != NULL){
            if(_defaultRole->isInitialized() == false){
                _defaultRole->initialize(_world->ourTeam(), _world->theirTeam(), _team->loc(), _ref);
            }
            _defaultRole->setPlayer(this, _playerAccessSelf);
            _defaultRole->runRole();
        }else{
            std::cout << "[ERROR] No role found for player #" << (int)playerId() << "!" << std::endl;
        }
        _mutexRole.unlock();
    }

    // Unlock wm for read
    _world->wmUnlock();
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
    WR::Utils::limitValue(&x, -2.5, 2.5);
    WR::Utils::limitValue(&y, -2.5, 2.5);

    // tem que fazer ajustes com os pids
    _grSim->setSpeed((int)_team->teamId(), (int)playerId(), x, y, theta);
    _grSim->setKickSpeed(_team->teamId(), playerId(), 0.0, 0.0);

}

std::pair<float, float> Player::goTo(double robot_x, double robot_y, double point_x, double point_y, double robotAngle, double offset){
    // Define a velocidade do robô para chegar na bola
    long double Vx = (point_x - robot_x);
    long double Vy = (point_y - robot_y);
    long double theta = robotAngle;
    long double moduloDistancia = sqrt(pow(Vx,2)+pow(Vy,2));
    float vxSaida = (Vx * cos(theta) + Vy * sin(theta));
    float vySaida = (Vy * cos(theta) - Vx * sin(theta));
    double sinal_x = 1;
    double sinal_y = 1;

    if(vxSaida < 0) sinal_x = -1;
    if(vySaida < 0) sinal_y = -1;

    if(moduloDistancia <= offset){
        vxSaida = 0;
        vySaida = 0;

        return std::make_pair(0.0, 0.0);
    }

    float newVX = _vxPID->calculate(vxSaida, velocity().x());
    float newVY = _vyPID->calculate(vySaida, velocity().y());

    return std::make_pair(newVX, newVY);
}

std::pair<double, double> Player::rotateTo(double robot_x, double robot_y, double point_x, double point_y, double angleOrigin2Robot) {
    // Define a velocidade angular do robô para visualizar a bola
    double vectorRobot2BallX = (point_x - robot_x);
    double vectorRobot2BallY = (point_y - robot_y);
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

    return std::make_pair(angleRobot2Ball, newSpeed);
}

void Player::goToLookTo(double robot_x, double robot_y, double point_x, double point_y, double aim_x, double aim_y, double angleOrigin2Robot, double offset){
    // Configura o robô para ir até a bola e olhar para um alvo
    std::pair<float, float> a;
    double p_x, p_y, angle, moduloDist, final_x, final_y;

    if (point_x == aim_x) angle = 1.570796327;
    else angle = atan((point_y - aim_y)/(point_x - aim_x));
    if (aim_x > point_x) {
        p_y = point_y - offset * sin(angle);
        p_x = point_x - offset * cos(angle);
    } else {
        p_y = point_y + offset * sin(angle);
        p_x = point_x + offset * cos(angle);
    }
    moduloDist = sqrt(pow((p_x - robot_x), 2) + pow((p_y - robot_y), 2));
    final_x = (p_x - robot_x)/moduloDist;
    final_y = (p_y - robot_y)/moduloDist;
    a = goTo(robot_x, robot_y, p_x + offset * final_x, p_y + offset * final_y, angleOrigin2Robot, offset);
    double theta = rotateTo(robot_x, robot_y, aim_x, aim_y, angleOrigin2Robot).second;

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

void Player::aroundTheBall(double robot_x, double robot_y, double point_x, double point_y, double robotAngle, double offset){
    // Configura o robô para ir até a bola e girar em torno dela
    std::pair<float, float> a;
    long double moduloDistancia = sqrt(pow((point_x - robot_x),2)+pow((point_y - robot_y),2));
    a = goTo(robot_x, robot_y, point_x, point_y, robotAngle, offset);
    float theta = rotateTo(robot_x, robot_y, point_x, point_y, robotAngle).second;

    if (moduloDistancia < offset) setSpeed(0, 0.2, theta); //3% de diferença nas velocidades
    else setSpeed(a.first, a.second, theta);
}

void Player::kick(bool isPass, float kickZPower){
    if(isPass)
        _grSim->setKickSpeed(_team->teamId(), playerId(), 2.0, kickZPower);
    else
        _grSim->setKickSpeed(_team->teamId(), playerId(), 6.0, kickZPower);
}

void Player::dribble(bool isActive){
    _grSim->setDribble(_team->teamId(), playerId(), isActive);
}
