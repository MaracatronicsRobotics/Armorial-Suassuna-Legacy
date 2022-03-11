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

#ifndef ACTUATORSERVICE_H
#define ACTUATORSERVICE_H

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
#include <src/utils/utils.h>
#include <proto/actuatorservice.grpc.pb.h>


class ActuatorService
{
public:
    ActuatorService(Constants *constants);

    void SetControl(ControlPacket cp);
    void SetControls(QList<ControlPacket> cpList);
    QList<ControlPacket> GetControls();

    ControlPacket setVelocity(int ID, bool robotColor, float vx, float vy, float vz);
    ControlPacket setAngularSpeed(int ID, bool robotColor, float vw, bool isInDegrees);
    ControlPacket setKickSpeed(int ID, bool robotColor, float vx, float vy, float vz);
    ControlPacket setDrible(int ID, bool robotColor, bool dribleOn);

protected:
    bool isConnectedToServer();
    void connectToServer();

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // gRPC channel related network methods and vars
    std::unique_ptr<Actuator::ActuatorService::Stub> _stub;
    std::shared_ptr<grpc::Channel> _channel;

    // Network
    QString _actuatorServiceAddress;
    quint16 _actuatorServicePort;
};

#endif // ACTUATORSERVICE_H
