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

#include "worldmap.h"

#include <src/entities/coach/sslteam/sslteam.h>
#include <src/constants/constants.h>

WorldMap::WorldMap(QString serviceAddress, quint16 servicePort) : Base::GRPC::Client<Armorial::Vision::VisionService>(serviceAddress, servicePort) {

}

void WorldMap::setupTeams(QMap<Common::Enums::Color, SSLTeam*> &teams) {
    _teams = teams;
}

Common::Types::Field WorldMap::getField() {
    _mutex.lockForRead();
    Common::Types::Field field = _field;
    _mutex.unlock();

    return field;
}

Common::Types::Object WorldMap::getBall() {
    _mutex.lockForRead();
    Common::Types::Object ball = _ball;
    _mutex.unlock();

    return ball;
}

SSLTeam* WorldMap::getTeam(Common::Enums::Color color) {
    _mutex.lockForRead();
    SSLTeam *team = _teams.value(color);
    _mutex.unlock();

    return team;
}

void WorldMap::initialization() {
    // Try to connect to server
    if(connectToServer(true)) {
        spdlog::info("[{}] Connected to vision service at address '{}' and port '{}'.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
    }
    else {
        spdlog::error("[{}] Failed to connect to vision service at address '{}' and port '{}'.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        return ;
    }
}

void WorldMap::loop() {
    // Check if is disconnected from server and try to reconnect
    if(!isConnectedToServer()) {
        spdlog::warn("[{}] Disconnected from vision service, trying to reconnect...", clientName().toStdString());
        bool couldReconnect = connectToServer(true);

        if(couldReconnect) {
            spdlog::info("[{}] Succesfully recconected to vision service at address '{}' and port '{}'.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
        }
        else {
            spdlog::warn("[{}] Reconnection attempt to vision service at address '{}' and port '{}' failed.", clientName().toStdString(), getServiceAddress().toStdString(), getServicePort());
            return ;
        }
    }

    // Update field and its objects
    updateField();
    updateBall();
    updatePlayers();
}

void WorldMap::finalization() {
    spdlog::info("[{}] Disconnected from vision service.", clientName().toStdString());
}

void WorldMap::updateField() {
    // Create default data transfer objects
    grpc::ClientContext context;
    google::protobuf::Empty request;
    Armorial::Field field;

    // Cast stub to get latest field packet
    getStub()->GetField(&context, request, &field);

    // Process to generate a Common::Types::Field object
    _mutex.lockForWrite();
    _field = Common::Types::Field(Constants::teamPlaySide(), field.centerradius(),
                                  field.fieldlength(), field.fieldwidth(),
                                  field.goaldepth(), field.goalwidth(),
                                  field.defenselength(), field.defensewidth(), 0.0);
    _mutex.unlock();
}

void WorldMap::updateBall() {
    // Create default data transfer objects
    grpc::ClientContext context;
    google::protobuf::Empty request;
    Armorial::Ball ball;

    // Cast stub to get latest field packet
    getStub()->GetBall(&context, request, &ball);

    _mutex.lockForWrite();
    _ball = Common::Types::Object({ball.ballposition().x(), ball.ballposition().y()},
                                  {ball.ballvelocity().vx(), ball.ballvelocity().vy()},
                                  {ball.ballacceleration().ax(), ball.ballacceleration().ay()});
    _mutex.unlock();
}

void WorldMap::updatePlayers() {
    _mutex.lockForWrite();

    // For each team color, get its robots
    magic_enum::enum_for_each<Common::Enums::Color>([this] (auto color) {
        if(color != Common::Enums::Color::UNDEFINED) {
            // Create default data transfer objects
            grpc::ClientContext context;
            google::protobuf::Empty request;

            // Create color
            Armorial::Color teamColor; teamColor.set_isblue(color == (Common::Enums::Color::BLUE));

            // Cast stub to get robots for the specific team color
            std::unique_ptr<grpc::ClientReader<Armorial::Robot>> robotsForTeam = getStub()->GetRobots(&context, teamColor);

            Armorial::Robot robot;
            while(robotsForTeam->Read(&robot)) {
                // Update robot if it is not invalid
                if(!robot.robotposition().isinvalid()) {
                    _teams[color]->updatePlayer(robot);
                }
            }
        }
    });

    _mutex.unlock();
}
