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

#include "coachservice.h"

#include <src/utils/utils.h>
#include <src/utils/text/text.h>

CoachService::CoachService(Constants *constants) {
    _constants = constants;

    _coachServiceAddress = getConstants()->getGRPCAddress();
    _coachServicePort = getConstants()->getGRPCCoachPort();
    connectToServer();
}

Robot CoachService::getRobot(RobotIdentifier identifier) {
    if(!isConnectedToServer()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" requested gRPC server but it is disconnected."));
        return Utils::getInvalidRobotObject();
    }

    grpc::ClientContext context;
    Robot robot;

    grpc::Status requestStatus = _stub->GetRobot(&context, identifier, &robot);

    if(!requestStatus.ok()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true)
                     + Text::bold(QString(" received a not OK status from gRPC request. Passed RobotIdentifier ID = %1 and Color = %2").
                                arg(identifier.robotid()).
                                arg(identifier.robotcolor().isblue() ? "BLUE" : "YELLOW").toStdString()));
        // Creating an fake robot identifier and allocate it into the robot
        RobotIdentifier *robotIdentifier = new RobotIdentifier();
        robotIdentifier->set_robotid(ROBOT_INVALID_ID);

        robot.set_allocated_robotidentifier(robotIdentifier);
    }

    return robot;
}

QList<Robot> CoachService::getRobots(Color teamColor) {
    if(!isConnectedToServer()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" requested gRPC server but it is disconnected."));
        return QList<Robot>();
    }

    grpc::ClientContext context;

    std::unique_ptr<grpc::ClientReader<Robot>> reader = _stub->GetRobots(&context, teamColor);

    Robot robot;
    QList<Robot> robots;
    while(reader->Read(&robot)) {
        robots.push_back(robot);
    }

    return robots;
}

Ball CoachService::getBall() {
    if(!isConnectedToServer()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" requested gRPC server but it is disconnected."));
        return Ball();
    }

    grpc::ClientContext context;
    google::protobuf::Empty emptyRequest;
    Ball ball;

    grpc::Status requestStatus = _stub->GetBall(&context, emptyRequest, &ball);

    if(!requestStatus.ok()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" received a not OK status from gRPC request."));

        // Creating an invalid position and allocate it into the ball
        Position *position = new Position();
        position->set_isinvalid(true);

        ball.set_allocated_ballposition(position);
    }

    return ball;
}

Field CoachService::getField() {
    if(!isConnectedToServer()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" requested gRPC server but it is disconnected."));
        return Field();
    }

    grpc::ClientContext context;
    google::protobuf::Empty emptyRequest;
    Field field;

    grpc::Status requestStatus = _stub->GetField(&context, emptyRequest, &field);

    if(!requestStatus.ok()) {
        spdlog::warn(Text::purple(__PRETTY_FUNCTION__, true) + Text::bold(" received a not OK status from gRPC request."));
    }

    return field;
}

bool CoachService::isConnectedToServer() {
    return (   _channel->GetState(false) == GRPC_CHANNEL_READY
            || _channel->GetState(false) == GRPC_CHANNEL_IDLE
            || _channel->GetState(false) == GRPC_CHANNEL_CONNECTING);
}

void CoachService::connectToServer() {
    QString serviceAddress = QString("%1:%2").arg(_coachServiceAddress).arg(_coachServicePort);
    _channel = grpc::CreateChannel(serviceAddress.toStdString(), grpc::InsecureChannelCredentials());
    _stub = Armorial::Coach::CoachService::NewStub(_channel);
}

Constants* CoachService::getConstants() {
    if(_constants == nullptr) {
        spdlog::warn(Text::bold("Constants with nullptr value at CoachClient."));
    }

    return _constants;
}
