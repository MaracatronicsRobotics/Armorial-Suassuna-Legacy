#include "controller.h"

#include <spdlog/spdlog.h>

#include <src/constants/constants.h>

Controller::Controller(QString serviceAddress, quint16 servicePort) : Base::GRPC::Client<Armorial::Actuator::ActuatorService>(serviceAddress, servicePort) {
    initializeData();
}

void Controller::initializeData() {
    _controlData = (ControlData *) malloc(Constants::maxNumPlayers() * sizeof(ControlData));

    for(int i = 0; i < Constants::maxNumPlayers(); i++) {
        _controlData[i].playerId = i;
        _controlData[i].vx = 0.0f;
        _controlData[i].vy = 0.0f;
        _controlData[i].vw = 0.0f;
        _controlData[i].kickPower = 0.0f;
        _controlData[i].chipKickAngle = 0.0f;
        _controlData[i].kickAngle = 0.0f;
        _controlData[i].dribbling = false;
        _controlData[i].isUpdated = true;
    }
}

void Controller::setWheelsSpeed(const quint8 &playerId, const float &wheelLeft, const float &wheelRight) {
    _mutex.lock();
    _controlData[playerId].wl = wheelLeft;
    _controlData[playerId].wr = wheelRight;
    _controlData[playerId].isUpdated = false;
    _mutex.unlock();
}

void Controller::setAngularSpeed(const quint8& playerId, const float& angularSpeed) {
    _mutex.lock();
    _controlData[playerId].vw = angularSpeed;
    _controlData[playerId].isUpdated = false;
    _mutex.unlock();
}

void Controller::setKick(const quint8& playerId, const float& kickSpeed, const float& chipKickAngle, const float& kickAngle) {
    _mutex.lock();
    _controlData->kickPower = kickSpeed;
    _controlData->chipKickAngle = chipKickAngle;
    _controlData->kickAngle = kickAngle;
    _controlData[playerId].isUpdated = false;
    _mutex.unlock();
}

void Controller::setDribble(const quint8& playerId, const bool& dribbling) {
    _mutex.lock();
    _controlData[playerId].dribbling = dribbling;
    _controlData[playerId].isUpdated = false;
    _mutex.unlock();
}

Armorial::ControlPacket Controller::getControlPacketFromControlData(const ControlData &controlData) {
    // Create a control packet based on the received data
    Armorial::ControlPacket controlPacket;

    // Create identifier
    Armorial::RobotIdentifier* robotIdentifier = new Armorial::RobotIdentifier();
    robotIdentifier->set_robotid(controlData.playerId);

    Armorial::Color* robotTeamColor = new Armorial::Color();
    robotTeamColor->set_isblue(Constants::teamColor() == Common::Enums::Color::BLUE);
    robotIdentifier->set_allocated_robotcolor(robotTeamColor);

    controlPacket.set_allocated_robotidentifier(robotIdentifier);

    // Create Velocity (linear speed)
//    Armorial::Velocity* velocity = new Armorial::Velocity();
//    velocity->set_vx(controlData.vx);
//    velocity->set_vy(controlData.vy);
//    controlPacket.set_allocated_robotvelocity(velocity);

    controlPacket.set_w1(controlData.wr);
    controlPacket.set_w2(controlData.wl);

    // Create AngularSpeed (angular speed)
    Armorial::AngularSpeed* aSpeed = new Armorial::AngularSpeed();
    aSpeed->set_vw(controlData.vw);
    controlPacket.set_allocated_robotangularspeed(aSpeed);

    // Create KickSpeed (kick data)
    Armorial::KickSpeed* kSpeed = new Armorial::KickSpeed();
    kSpeed->set_kickspeed(controlData.kickPower);
    kSpeed->set_chipangle(controlData.chipKickAngle);
    kSpeed->set_kickangle(controlData.kickAngle);
    controlPacket.set_allocated_robotkick(kSpeed);

    // Set dribbling
    controlPacket.set_dribbling(controlData.dribbling);

    // Return
    return controlPacket;
}

void Controller::initialization() {
    // Try to connect to server
    if(connectToServer(true)) {
        spdlog::info("[{}] Connected to actuator service at address '{}' and port '{}'.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
    }
    else {
        spdlog::error("[{}] Failed to connect to actuator service at address '{}' and port '{}'.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        return ;
    }
}

void Controller::loop() {
    // Check if is disconnected from server and try to reconnect
    if(!isConnectedToServer()) {
        spdlog::warn("[{}] Disconnected from actuator service, trying to reconnect...", clientName().toStdString());
        bool couldReconnect = connectToServer(true);

        if(couldReconnect) {
            spdlog::info("[{}] Succesfully recconected to actuator service at address '{}' and port '{}'.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        }
        else {
            spdlog::warn("[{}] Reconnection attempt to actuator service at address '{}' and port '{}' failed.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
            return ;
        }
    }

    // Loop over the packets and send them forward to actuator
    _mutex.lock();

    // Get amount of not updated packets
    bool haveNonUpdatedPackets = false;
    for(int i = 0; i < Constants::maxNumPlayers(); i++) {
        if(!_controlData[i].isUpdated) {
            haveNonUpdatedPackets = true;
            break;
        }
    }

    if(haveNonUpdatedPackets) {
        // Create default data transfer objects
        grpc::ClientContext context;
        google::protobuf::Empty request;
        std::unique_ptr<grpc::ClientWriter<Armorial::ControlPacket>> writer = getStub()->SetControls(&context, &request);

        // Send packet for not updated players
        for(int i = 0; i < Constants::maxNumPlayers(); i++) {
            if(!_controlData[i].isUpdated) {
                Armorial::ControlPacket controlPacket = getControlPacketFromControlData(_controlData[i]);
                writer->Write(controlPacket);

                // Mark as updated
                _controlData[i].isUpdated = true;
            }
        }
    }

    _mutex.unlock();
}

void Controller::finalization() {
    spdlog::info("[{}] Disconnected from actuator service.", clientName().toStdString());
}
