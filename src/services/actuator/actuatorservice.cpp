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
    grpc::ClientContext context;
    google::protobuf::Empty emptyRequest;

    grpc::Status requestStatus = _stub->SetControl(&context, cp, &emptyRequest);

    if (!requestStatus.ok()) {
        std::cout << Text::yellow("[WARNING] ", true) + Text::bold("ActuatorService::SetControl() received a not OK status from gRPC request.") + '\n';
    }
}

void ActuatorService::SetControls(QList<ControlPacket> cpList) {
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
        std::cout << Text::yellow("[WARNING] ", true) + Text::bold("ActuatorService::SetControls() received a not OK status from gRPC stream.") + '\n';
    }
}

QList<ControlPacket> ActuatorService::GetControls(){
    grpc::ClientContext context;
    google::protobuf::Empty emptyRequest;
    ControlPacket cp;
    QList<ControlPacket> cpList;

    std::unique_ptr<grpc::ClientReader<ControlPacket>> reader = _stub->GetControls(&context, emptyRequest);
    while (reader->Read(&cp)) {
        cpList.push_back(cp);
    }

    return cpList;
}

ControlPacket ActuatorService::getNewControlPacket(){
    ControlPacket cp;

    Velocity *vel = new Velocity();
    vel->set_vx(0.0f);
    vel->set_vy(0.0f);
    vel->set_vz(0.0f);
    vel->set_isinvalid(true);

    Velocity *robotVel = new Velocity();
    robotVel->set_vx(0.0f);
    robotVel->set_vy(0.0f);
    robotVel->set_vz(0.0f);
    robotVel->set_isinvalid(true);

    Color *robotColor = new Color();
    robotColor->set_isblue(false);

    RobotIdentifier *robotID = new RobotIdentifier();
    robotID->set_robotid(0);
    robotID->set_allocated_robotcolor(robotColor);

    AngularSpeed *angularSpeed = new AngularSpeed();
    angularSpeed->set_vw(0.0f);
    angularSpeed->set_isindegrees(false);
    angularSpeed->set_isinvalid(true);

    cp.set_w1(0.0f);
    cp.set_w2(0.0f);
    cp.set_w3(0.0f);
    cp.set_w4(0.0f);
    cp.set_dribbling(false);
    cp.set_allocated_kickspeed(vel);
    cp.set_allocated_robotvelocity(robotVel);
    cp.set_allocated_robotidentifier(robotID);
    cp.set_allocated_robotangularspeed(angularSpeed);

    return cp;
}

ControlPacket ActuatorService::setVelocity(int robotID, bool robotColor, float vx, float vy, float vz) {
    RobotIdentifier *ID = new RobotIdentifier();
    Color *color = new Color();
    color->set_isblue(robotColor);
    ControlPacket cp = getNewControlPacket();

    Velocity *robotVel = new Velocity();
    robotVel->set_vx(vx);
    robotVel->set_vy(vy);
    robotVel->set_vz(vz);
    robotVel->set_isinvalid(false);

    ID->set_robotid(robotID);
    ID->set_allocated_robotcolor(color);

    cp.set_allocated_robotidentifier(ID);
    cp.set_allocated_robotvelocity(robotVel);
    return cp;
}

ControlPacket ActuatorService::setAngularSpeed(int robotID, bool robotColor, float vw, bool isInDegrees) {
    RobotIdentifier *ID = new RobotIdentifier();
    Color *color = new Color();
    color->set_isblue(robotColor);
    ControlPacket cp;

    AngularSpeed *angularSpeed = new AngularSpeed();
    angularSpeed->set_vw(vw);
    angularSpeed->set_isindegrees(isInDegrees);
    angularSpeed->set_isinvalid(false);

    ID->set_robotid(robotID);
    ID->set_allocated_robotcolor(color);

    cp.set_allocated_robotidentifier(ID);
    cp.set_allocated_robotangularspeed(angularSpeed);

    return cp;
}

ControlPacket ActuatorService::setKickSpeed(int robotID, bool robotColor, float vx, float vy, float vz) {
    RobotIdentifier *ID = new RobotIdentifier();
    Color *color = new Color();
    color->set_isblue(robotColor);
    ControlPacket cp = getNewControlPacket();

    Velocity *robotKickSpeed = new Velocity();
    robotKickSpeed->set_vx(vx);
    robotKickSpeed->set_vy(vy);
    robotKickSpeed->set_vz(vz);
    robotKickSpeed->set_isinvalid(false);

    ID->set_robotid(robotID);
    ID->set_allocated_robotcolor(color);

    cp.set_allocated_robotidentifier(ID);
    cp.set_allocated_robotvelocity(robotKickSpeed);
    return cp;
}

ControlPacket ActuatorService::setDrible(int robotID, bool robotColor, bool dribleOn) {
    RobotIdentifier *ID = new RobotIdentifier();
    Color *color = new Color();
    color->set_isblue(robotColor);
    ControlPacket cp = getNewControlPacket();

    ID->set_robotid(robotID);
    ID->set_allocated_robotcolor(color);

    cp.set_allocated_robotidentifier(ID);
    cp.set_dribbling(dribleOn);

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
    _stub = Actuator::ActuatorService::NewStub(_channel);
}

Constants* ActuatorService::getConstants(){
    if(_constants == nullptr) {
        std::cout << Text::yellow("[WARNING] ", true) + Text::bold("Constants with nullptr value at ActuatorClient.\n");
    }

    return _constants;
}
