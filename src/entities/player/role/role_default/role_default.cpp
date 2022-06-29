#include "role_default.h"

Role_Default::Role_Default() {
    _bhv_gotolookto = nullptr;
    _testRunning = true;
}

QString Role_Default::name() {
    return "Role_Default";
}

void Role_Default::initializeBehaviours() {
    addBehaviour(BEHAVIOR_GOTOLOOKTO, _bhv_gotolookto = new Behaviour_GoToLookTo());
}

void Role_Default::configure() {
    _actualState = PID_TEST;
    _testType = VX;
    setBehaviour(BEHAVIOR_GOTOLOOKTO);
}

void Role_Default::run() {

    // To set up PID Test I recommend to previously set robot position to:
    // X: 0
    // Y: -2
    // Ang: 0
    // These are numbers for grSim

    Position playerPos = player()->getPlayerPos();

    switch (_testType) {
    case(VX): {
        _endPos = Utils::getPositionObject(4, -2);
    }break;
    case(VY): {
        _endPos = Utils::getPositionObject(0, 2);
    }break;
    }

    switch (_actualState) {
    case (PID_TEST): {
        if (_testRunning) {
            _playerVel = player()->getPlayerVelocity();
            _playerAcc = player()->getPlayerAcceleration();
            spdlog::info(QString("PID Test - Player: (%1, %2); Vel: %3 Acceleration: %4").arg(playerPos.x()).arg(playerPos.y()).arg(Utils::getVelocityAbs(_playerVel)).arg(Utils::getAccelerationSignedAbs(_playerAcc)).toStdString());

            _bhv_gotolookto->setPositionToGo(_endPos);
            _bhv_gotolookto->setReferencePosition(_endPos);
            _bhv_gotolookto->setPositionToLook(_endPos);

            setBehaviour(BEHAVIOR_GOTOLOOKTO);
        } else {

        }
    }break;
    }

    if (Utils::getVelocityAbs(player()->getPlayerVelocity()) != 0.0f) {
        _testRunning = true;
    } else {
        _testRunning = false;
    }

}
