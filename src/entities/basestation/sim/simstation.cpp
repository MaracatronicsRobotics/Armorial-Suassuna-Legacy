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

#include "simstation.h"

#include <include/proto/packet.pb.h>

#include <src/common/constants/constants.h>
#include <src/common/utils/utils.h>

SimStation::SimStation() : Base::UDP::Client(Suassuna::Constants::actuatorAddress(), Suassuna::Constants::actuatorPort(), Suassuna::Constants::actuatorNetworkInterface())
{
    // Initialize control packets
    for(int i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
        // Build control packet
        Armorial::ControlPacket controlPacket;
        Armorial::RobotIdentifier* identifier = new Armorial::RobotIdentifier();
        identifier->CopyFrom(Suassuna::Utils::buildIdentifierFromArgs(Suassuna::Constants::teamColor(), i));
        controlPacket.set_allocated_robotidentifier(identifier);

        // Add to map
        _controlPackets.insert(i, controlPacket);
    }
}

void SimStation::sendData(const Armorial::ControlPacket& packet) {
    _controlPacketsLock.lock();
    _controlPackets.insert(packet.robotidentifier().robotid(), packet);
    _controlPacketsLock.unlock();
}

void SimStation::sendZeroData(const Armorial::RobotIdentifier& robotIdentifier) {
    _controlPacketsLock.lock();

    // Build zero-control packet
    Armorial::ControlPacket controlPacket;
    Armorial::RobotIdentifier* identifier = new Armorial::RobotIdentifier();
    identifier->CopyFrom(robotIdentifier);
    controlPacket.set_allocated_robotidentifier(identifier);

    // Insert into control
    _controlPackets.insert(identifier->robotid(), controlPacket);

    _controlPacketsLock.unlock();
}

void SimStation::initialization() {
    // Try to connect to the vision server address (ssl-vision / game controller autoreferee vision)
    bool couldConnectToSimServer = connectToNetwork();
    if(!couldConnectToSimServer) {
        spdlog::error("[{}] Failed to connect to sim server in address '{}', port '{}' and interface '{}'.", Base::UDP::Client::clientName().toStdString(), getServerAddress().toStdString(), getServerPort(), getServerNetworkInterface().toStdString());
        exit(-1);
    }
    else {
        spdlog::info("[{}] Succesfully connected to sim server in address '{}', port '{}' and interface '{}'.", Base::UDP::Client::clientName().toStdString(), getServerAddress().toStdString(), getServerPort(), getServerNetworkInterface().toStdString());
    }
}

void SimStation::loop() {
    // Creating packet
    fira_message::sim_to_ref::Packet packet;

    // Reading from control packets and setting commands
    for (const auto& pkt : _controlPackets) {
        fira_message::sim_to_ref::Command *command = packet.mutable_cmd()->add_robot_commands();

        // Setting macro informations (team and robot id)
        command->set_id(pkt.robotidentifier().robotid());
        command->set_yellowteam(!pkt.robotidentifier().robotcolor().isblue());

        // Set wheels speed
        command->set_wheel_left(pkt.w1());
        command->set_wheel_right(pkt.w2());
    }

    // Parse to datagram
    QByteArray buffer(packet.ByteSize(), 0);
    packet.SerializeToArray(buffer.data(), buffer.size());
    QNetworkDatagram datagram(buffer);

    // Try to send data to simulator
    bool sentDatagram = sendDatagram(datagram);
    if(!sentDatagram) {
        spdlog::warn("[{}] Failed to sent datagram to simulator. Is simulator running?", clientName().toStdString());
    }
}

void SimStation::finalization() {
    // Disconnect from sim network and wait
    disconnectFromNetwork();

    spdlog::info("[{}] Disconnected from sim server and vision service.", Base::UDP::Client::clientName().toStdString());
}
