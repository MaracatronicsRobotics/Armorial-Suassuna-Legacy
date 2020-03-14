#include "suassuna.h"
#include <entity/player/role/mrcroles.h>
#include <entity/contromodule/strategy/strategy.h>
#include <entity/contromodule/strategy/strategystate.h>
#include <entity/contromodule/strategy/basics/mrcstrategy.h>
#include <entity/player/control/pid.h>
#include <utils/freeangles/freeangles.h>

#include <entity/contromodule/grsSimulator/grsSimulator.h>

Suassuna::Suassuna(quint8 teamId, Colors::Color teamColor, FieldSide fieldSide)
    : _teamId(teamId), _teamColor(teamColor), _fieldSide(fieldSide){
    // Create controller
    _ctr = new Controller();

    // Create GUI
    _ourGUI = new CoachView();
    
    // Default field setup
    _defaultField = new Fields::SSL2015();

    // Initialize default values
    this->setServerAddress("localhost", 0);
}

Suassuna::~Suassuna() {
    // Delete controller
    delete _ctr;
    // Delete defautl field
    delete _defaultField;
}

bool Suassuna::start() {
    // Op team infos parsing
    quint8 opTeamId = (_teamId==0? 1:0);
    Colors::Color opTeamColor = (_teamColor==Colors::YELLOW? Colors::BLUE : Colors::YELLOW);
    FieldSide opFieldSide = (_fieldSide.isRight()? Sides::LEFT : Sides::RIGHT);

    // Server connection
    if(connectToServer()==false)
        return false;

    // Create World
    _world = new World(_ctr, _defaultField, _ourGUI);

    // Create SSLReferee
    _ref = new SSLReferee();
    _ref->addGameInfo(Colors::YELLOW);
    _ref->addGameInfo(Colors::BLUE);
    _world->addEntity(_ref, 0);

    // Setup teams
    setupTeams(opTeamId, opTeamColor, opFieldSide);
    _world->setTeams(_ourTeam, _theirTeam);

    // Create GRSim Simulator
    _grSimulator = new grsSimulator();
    _world->addEntity(_grSimulator, 0);

    // Setup team players
    setupOurPlayers();
    setupOppPlayers(opTeamId);

    // Setup GUI
    _ourGUI->setTeams(_ourTeam, _theirTeam);
    _world->addEntity(_ourGUI, 2);

    // Create coach
    _coach = new Coach(_ref, _ourTeam, _theirTeam);
    _world->setControlModule(_coach);

    // Setup strategy for coach
    Strategy *strategy = NULL;
    strategy = new MRCStrategy();
    _coach->setStrategy(strategy);

    FreeAngles::initialize(_ourTeam, _theirTeam);

    // Start world
    _world->start();

    return true;
}

void Suassuna::stop() {
    // Stop world and wait for it
    _world->stopEntity();
    _world->wait();

    // Delete teams
    delete _ourTeam;
    delete _theirTeam;

    // Delete modules
    delete _world;
    delete _coach;

    // Disconnect controller
    _ctr->disconnect();
}

void Suassuna::setServerAddress(QString ipAddress, int port) {
    _serverAddress = ipAddress;
    _serverPort = port;
}

bool Suassuna::connectToServer() {
    // Server connection
    if(_ctr->connect(_serverAddress, _serverPort) == false) {
        std::cout << "[ERROR] failed to connect to GEARSystem's server!";
        return false;
    }
    // Check if GEARSystem's Sensor is connected
    if(_ctr->teams().size() < 2) {
        std::cout << "[ERROR] no teams on controller, GEARSystem's sensor is not connected!";
        return false;
    }
    return true;
}

void Suassuna::setupTeams(quint8 opTeamId, Colors::Color opTeamColor, FieldSide opFieldSide) {
    // Create teams
    _ourTeam = new MRCTeam(_teamId, _world->getWorldMap());
    _theirTeam = new MRCTeam(opTeamId, _world->getWorldMap());
    // Set opponent teams
    _ourTeam->setOpponentTeam(_theirTeam);
    _theirTeam->setOpponentTeam(_ourTeam);
    // Set our team info
    _ourTeam->setTeamColor(_teamColor);
    _ourTeam->setFieldSide(_fieldSide);
    // Set op team info
    _theirTeam->setTeamColor(opTeamColor);
    _theirTeam->setFieldSide(opFieldSide);
}

void Suassuna::setupOurPlayers() {
    // Create OUR PLAYERS
    QList<quint8> playerList = _world->getWorldMap()->players(_teamId);
    for(quint8 i=0; i<playerList.size() && i<=MAX_ROBOT_ID; i++) {
        // Create Player
        PID *vxPID = new PID(0.5, 0.01, 0.0, 2.5, -2.5);
        PID *vyPID = new PID(0.5, 0.01, 0.0, 2.5, -2.5);
        PID *vwPID = new PID(0.5, 0.01, 0.003, 3.0, -3.0);
        Player *player = new Player(_world, _ourTeam, _ctr, playerList.at(i), new Role_Default(), _ref, _grSimulator, vxPID, vyPID, vwPID);
        // Enable
        player->enable(true);
        // Add to team
        _ourTeam->addPlayer(player);
        // Add to world
        _world->addEntity(player, 2);
    }
}
void Suassuna::setupOppPlayers(quint8 opTeamId) {
    // Create opp. players
    const QList<quint8> opPlayerList = _world->getWorldMap()->players(opTeamId);
    for(quint8 i=0; i<opPlayerList.size() && i<=MAX_ROBOT_ID; i++) {
        // Create Player
        Player *opPlayer = new Player(_world, _theirTeam, _ctr, opPlayerList.at(i), NULL, _ref, NULL, NULL, NULL, NULL);
        // Disable (op team doesnt run)
        opPlayer->enable(false);
        // Add to team
        _theirTeam->addPlayer(opPlayer);
        // Add to world
        _world->addEntity(opPlayer, 2);
    }
}
