#include "player.h"
#include <entity/contromodule/mrcteam.h>
#include <entity/player/playeraccess.h>
#include <entity/player/behaviour/behaviour.h>

#include <entity/contromodule/grsSimulator/grsSimulator.h>

QString Player::name(){
    return "Player #"+QString::number((int)_team->teamId())+":"+QString::number((int)_playerId);
}

Player::Player(World *world, MRCTeam *team, Controller *ctr, quint8 playerID, Behaviour *defaultBehaviour, SSLReferee *ref, grsSimulator *grSim) : Entity(Entity::ENT_PLAYER){
    _world = world;
    _team = team;
    _playerId = playerID;
    _ref = ref;
    _ctr = ctr;
    _behaviour = NULL;
    _defaultBehaviour = defaultBehaviour;
    _playerAccessSelf = new PlayerAccess(true, this, team->loc());
    _playerAccessBus = new PlayerAccess(false, this, team->loc());

    // Idle control
    _idleCount = 0;

    // Reset player
    reset();

    // grSimulator for test
    _grSim = grSim;
}

Player::~Player(){
    if(_defaultBehaviour != NULL)
        delete _defaultBehaviour;
    if(_playerAccessSelf != NULL)
        delete _playerAccessSelf;
    if(_playerAccessBus != NULL)
        delete _playerAccessBus;
}

PlayerAccess* Player::access() const {
    return _playerAccessBus;
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

        _mutexBehaviour.lock();
        if(_behaviour != NULL){
            if(_behaviour->isInitialized() == false){
                _behaviour->initialize(_team->loc());
            }
            _behaviour->setPlayer(this, _playerAccessSelf);
            _behaviour->runBehaviour();
        }else if(_defaultBehaviour != NULL){
            if(_defaultBehaviour->isInitialized() == false){
                _defaultBehaviour->initialize(_team->loc());
            }
            _defaultBehaviour->setPlayer(this, _playerAccessSelf);
            _defaultBehaviour->runBehaviour();
        }else{
            std::cout << "[ERROR] No behaviour found for player #" << (int)playerId() << "!" << std::endl;
        }
        _mutexBehaviour.unlock();
    }

    // Unlock wm for read
    _world->wmUnlock();
}

QString Player::getBehaviourName() {
    _mutexBehaviour.lock();
    QString behaviourName;
    if(_behaviour == NULL)
        behaviourName = "UNKNOWN";
    else
        behaviourName = _behaviour->name();
    _mutexBehaviour.unlock();
    return behaviourName;
}

void Player::setBehaviour(Behaviour* b) {
    // Check same behavior
    if(b==_behaviour)
        return;
    // Set new
    _mutexBehaviour.lock();
    _behaviour = b;
    this->reset();
    _mutexBehaviour.unlock();
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
    //return _team->wm()->playerVelocity(teamId(), _playerId);
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
    //return _team->wm()->ballPossession(teamId(), _playerId);
}

bool Player::canKickBall() const{
    //return _ref->getGameInfo(_team->teamColor())->canKickBall();
}

float Player::distanceTo(const Position &pos) const{
    return WR::Utils::distance(position(), pos);
}

float Player::distBall() const{
    //return WR::Utils::distance(position(), _team->loc()->ball());
}

float Player::distOurGoal() const{
    //return WR::Utils::distance(position(), _team->loc()->ourGoal());
}

float Player::distTheirGoal() const{
    //return WR::Utils::distance(position(), _team->loc()->theirGoal());
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
    // tem que fazer ajustes com os pids
    _grSim->setSpeed((int)_team->teamId(), (int)playerId(), x, y, theta);
}

// exemplo de skill
void Player::goTo(/* parametros vao aqui dentro */){
    // o que faz
}
