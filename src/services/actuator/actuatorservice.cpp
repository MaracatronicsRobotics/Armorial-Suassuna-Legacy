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

#include "actuatorservice.h"

#include <src/utils/text/text.h>

ActuatorService::ActuatorService(Constants *constants) {
    _constants = constants;

    _actuatorServiceAddress = getConstants()->getGRPCAddress();
    _actuatorServicePort = getConstants()->getGRPCActuatorPort();
    connectToServer();
}

void ActuatorService::SetControl(ControlPacket cp) {
    if(!isConnectedToServer()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" requested gRPC server but it is disconnected."));
        return ;
    }

    grpc::ClientContext context;
    google::protobuf::Empty emptyRequest;

    grpc::Status requestStatus = _stub->SetControl(&context, cp, &emptyRequest);

    if (!requestStatus.ok()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" received a not OK status from gRPC request."));
    }
}

void ActuatorService::SetControls(QList<ControlPacket> cpList) {
    if(!isConnectedToServer()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" requested gRPC server but it is disconnected."));
        return ;
    }

    grpc::ClientContext context;
    google::protobuf::Empty emptyRequest;

    std::unique_ptr<grpc::ClientWriter<ControlPacket>> writer = _stub->SetControls(&context, &emptyRequest);
    for(ControlPacket cp : cpList) {
        bool resp = writer->Write(cp);
        if (!resp) {
            break;
        }

        //Should we have here a little delay on the thread?
    }
    writer->WritesDone();
    grpc::Status streamStatus = writer->Finish();
    if (!streamStatus.ok()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" received a not OK status from gRPC request."));
    }
}

ControlPacket* ActuatorService::setVelocity(int ID, bool robotColor, float vx, float vy, float vz) {
    ControlPacket *cp = new ControlPacket();

    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(Utils::getRobotIdObject(ID, robotColor));

    Velocity *robotVel = new Velocity();
    robotVel->CopyFrom(Utils::getVelocityObject(vx, vy, vz, false));

    cp->set_allocated_robotidentifier(robotIdentifier);
    cp->set_allocated_robotvelocity(robotVel);

    return cp;
}

ControlPacket* ActuatorService::setAngularSpeed(int ID, bool robotColor, float vw, bool isInDegrees) {
    ControlPacket *cp = new ControlPacket();

    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(Utils::getRobotIdObject(ID, robotColor));

    AngularSpeed *angularSpeed = new AngularSpeed();
    angularSpeed->CopyFrom(Utils::getAngularSpeedObject(vw, isInDegrees, false));

    cp->set_allocated_robotidentifier(robotIdentifier);
    cp->set_allocated_robotangularspeed(angularSpeed);

    return cp;
}

ControlPacket* ActuatorService::setKickSpeed(int ID, bool robotColor, float vx, float vy, float vz) {
    ControlPacket *cp = new ControlPacket();

    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(Utils::getRobotIdObject(ID, robotColor));

    Velocity *robotKickSpeed = new Velocity();
    robotKickSpeed->CopyFrom(Utils::getVelocityObject(vx, vy, vz, false));

    cp->set_allocated_robotidentifier(robotIdentifier);
    cp->set_allocated_robotvelocity(robotKickSpeed);

    return cp;
}

ControlPacket* ActuatorService::setDrible(int ID, bool robotColor, bool dribleOn) {
    ControlPacket *cp = new ControlPacket();

    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(Utils::getRobotIdObject(ID, robotColor));

    cp->set_allocated_robotidentifier(robotIdentifier);
    cp->set_dribbling(dribleOn);

    return cp;
}

bool ActuatorService::isConnectedToServer(){
    return (   _channel->GetState(false) == GRPC_CHANNEL_READY
            || _channel->GetState(false) == GRPC_CHANNEL_IDLE
            || _channel->GetState(false) == GRPC_CHANNEL_CONNECTING);
}

void ActuatorService::connectToServer(){
    QString serviceAddress = QString("%1:%2").arg(_actuatorServiceAddress).arg(_actuatorServicePort);
    _channel = grpc::CreateChannel(serviceAddress.toStdString(), grpc::InsecureChannelCredentials());
    _stub = Armorial::Actuator::ActuatorService::NewStub(_channel);
}

Constants* ActuatorService::getConstants(){
    if(_constants == nullptr) {
        spdlog::warn(Text::bold("Constants with nullptr value at ActuatorClient."));
    }

    return _constants;
}
