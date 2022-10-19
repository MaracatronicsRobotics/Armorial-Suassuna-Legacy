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

#include "sslreferee.h"

#include <Armorial/Utils/Utils.h>

#include <include/proto/vssref_command.pb.h>
#include <src/constants/constants.h>

#define MIN_DIST_TO_CONSIDER_BALL_MOVEMENT 0.2f // 20cm

VSSReferee::VSSReferee(WorldMap *worldMap) : Base::UDP::Client(Constants::refereeNetworkAddress(), Constants::refereeNetworkPort(), Constants::refereeNetworkInterface()) {
    _worldMap = worldMap;
    _stopRobots = false;
}

VSSReferee::~VSSReferee() {

}

void VSSReferee::initialization() {
    bool connectedToRefereeNetwork = bindAndConnectToMulticastNetwork();
    if(connectedToRefereeNetwork) {
        spdlog::info("[{}] Connected to referee network at address '{}:{}' and interface '{}'.", clientName().toStdString(), getServerAddress().toStdString(),
                     getServerPort(), getServerNetworkInterface().toStdString());
    }
    else {
        spdlog::error("[{}} Could not connect to referee network at address '{}:{}' and interface '{}'.", clientName().toStdString(), getServerAddress().toStdString(),
                      getServerPort(), getServerNetworkInterface().toStdString());
        exit(-1);
    }
}

void VSSReferee::loop() {
    while(hasPendingDatagrams()) {
        // Take datagram
        auto datagram = receiveDatagram();

        // If no datagram / valid datagram has been received, just go to another packet
        if(!datagram) {
            continue;
        }

        // Try to convert datagram to protocol type
        VSSRef::ref_to_team::VSSRef_Command wrapperData;
        bool converted = Utils::Proto::convertDatagramToType(datagram.value(), wrapperData);

        // If could not convert the received datagram, just go to another packet
        if(!converted) {
            spdlog::warn("[{}] Failed to parse datagram.", clientName().toStdString());
            continue;
        }

        _stopRobots = !(wrapperData.foul() == VSSRef::Foul::GAME_ON);
        spdlog::info("[{}] Received the '{}' command for '{}' at quadrant '{}'.", clientName().toStdString(), VSSRef::Foul_Name(wrapperData.foul()), VSSRef::Color_Name(wrapperData.teamcolor()), VSSRef::Quadrant_Name(wrapperData.foulquadrant()));
    }
}

void VSSReferee::finalization() {
    // Disconnect from referee multicast network and wait
    disconnectFromNetwork();

    // Don't need to disconnect from service, so just show disconnection
    spdlog::info("[{}] Disconnected from referee network.", clientName().toStdString());
}
