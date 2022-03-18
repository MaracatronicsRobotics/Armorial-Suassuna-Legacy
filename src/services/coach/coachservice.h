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


#ifndef COACHSERVICE_H
#define COACHSERVICE_H

#include <grpc/grpc.h>

#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <src/constants/constants.h>
#include <proto/coachservice.grpc.pb.h>

#include <spdlog/spdlog.h>

class CoachService
{
public:
    CoachService(Constants *constants);

    Robot getRobot(RobotIdentifier identifier);
    QList<Robot> getRobots(Color teamColor);
    Ball getBall();
    Field getField();

protected:
    bool isConnectedToServer();
    void connectToServer();

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // gRPC channel related network methods and vars
    std::unique_ptr<Armorial::Coach::CoachService::Stub> _stub;
    std::shared_ptr<grpc::Channel> _channel;

    // Network
    QString _coachServiceAddress;
    quint16 _coachServicePort;
};

#endif // COACHSERVICE_H
