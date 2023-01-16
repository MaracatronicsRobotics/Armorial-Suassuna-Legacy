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

#include "vision.h"

#include <Armorial/Utils/Utils.h>

#include <include/proto/packet.pb.h>

#include <src/common/constants/constants.h>

Vision::Vision() : Base::UDP::Client(Suassuna::Constants::visionAddress(), Suassuna::Constants::visionPort(), Suassuna::Constants::visionNetworkInterface())
{
    // Start robots list
    magic_enum::enum_for_each<Common::Enums::Color>([this] (Common::Enums::Color color) {
        if(color != Common::Enums::Color::UNDEFINED) {
            // Start if not contains color
            if(!_robots.contains(color)) {
                _robots.insert(color, new QMap<quint8, VisionObject*>());
            }

            // Take team map
            QMap<quint8, VisionObject*> *teamMap = _robots.value(color);

            for(int i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
                // Check if teamMap has the required player
                if(!teamMap->contains(i)) {
                    teamMap->insert(i, new VisionObject());
                }
            }
        }
    });
}

Vision::~Vision() {
    // Get teams list
    QList<Common::Enums::Color> teamList = _robots.keys();

    // For each team
    for(QList<Common::Enums::Color>::iterator it = teamList.begin(); it != teamList.end(); it++) {
        // Take team associated objects
        QMap<quint8, VisionObject*> *teamObjects = _robots.value((*it));
        QList<VisionObject*> objects = teamObjects->values();

        // For each object, delete it
        for(QList<VisionObject*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++) {
            delete (*it2);
        }

        // Clear team association
        teamObjects->clear();

        // Delete pointers
        delete teamObjects;
    }

    // Clear teams
    _robots.clear();
}

void Vision::initialization() {
    // Try to connect to the vision server address (ssl-vision / game controller autoreferee vision)
    bool couldConnectToVisionServer = bindAndConnectToMulticastNetwork();
    if(!couldConnectToVisionServer) {
        spdlog::error("[{}] Failed to connect to vision server in address '{}', port '{}' and interface '{}'.", Base::UDP::Client::clientName().toStdString(), getServerAddress().toStdString(), getServerPort(), getServerNetworkInterface().toStdString());
        exit(-1);
    }
    else {
        spdlog::info("[{}] Succesfully connected to vision server in address '{}', port '{}' and interface '{}'.", Base::UDP::Client::clientName().toStdString(), getServerAddress().toStdString(), getServerPort(), getServerNetworkInterface().toStdString());
    }
}

void Vision::loop() {
    // Process all pending datagrams
    while(hasPendingDatagrams()) {
        // Take datagram
        auto datagram = receiveDatagram();

        // If no datagram / valid datagram has been received, just go to another packet
        if(!datagram) {
            continue;
        }

        // Try to convert datagram to protocol type
        fira_message::sim_to_ref::Environment wrapperData;
        bool converted = Utils::Proto::convertDatagramToType(datagram.value(), wrapperData);

        // If could not convert the received datagram, just go to another packet
        if(!converted) {
            spdlog::warn("[{}] Failed to convert vision datagram.", Base::UDP::Client::clientName().toStdString());
            continue;
        }

        // Parse data from wrapper
        if(wrapperData.has_frame()) {
            fira_message::Frame visionFrame = wrapperData.frame();

            // Clear vision controls
            resetAppearedControl();

            // Update ball
            {
                fira_message::Ball ball = visionFrame.ball();
                _ball.updateObject(1.0f, Geometry::Vector2D(ball.x(), ball.y()), 0.0f);
            }

            // Update players
            {
                // Blue
                for(int i = 0; i < visionFrame.robots_blue_size(); i++) {
                    fira_message::Robot robot = visionFrame.robots_blue(i);
                    updatePlayer(Common::Enums::Color::BLUE, robot.robot_id(), Geometry::Vector2D(robot.x(), robot.y()), Geometry::Angle(robot.orientation()), 1.0f);
                }
                updateNotAppearedRobotsFromColor(Common::Enums::Color::BLUE);

                // Yellow
                for(int i = 0; i < visionFrame.robots_yellow_size(); i++) {
                    fira_message::Robot robot = visionFrame.robots_yellow(i);
                    updatePlayer(Common::Enums::Color::YELLOW, robot.robot_id(), Geometry::Vector2D(robot.x(), robot.y()), Geometry::Angle(robot.orientation()), 1.0f);
                }
                updateNotAppearedRobotsFromColor(Common::Enums::Color::YELLOW);
            }
        }

        // Mount packet to update robots
        QList<Armorial::Robot> robotsPacket;
        QList<Common::Enums::Color> colors = _robots.keys();
        for(auto color : colors) {
            QMap<quint8, VisionObject*>* robots = _robots.value(color);
            QList<quint8> ids = robots->keys();
            for(auto id : ids) {
                VisionObject* visionObject = robots->value(id);
                if(visionObject->isObjectValid()) {
                    robotsPacket.push_back(visionObject->convertToRobotDto(color, id));
                }
            }
        }
        emit sendRobots(robotsPacket);

        // Mount packet to update balls
        QList<Armorial::Ball> ballsPacket;
        ballsPacket.push_back(_ball.convertToBallDto());
        emit sendBalls(ballsPacket);
    }
}

void Vision::finalization() {
    // Disconnect from vision multicast network and wait
    disconnectFromNetwork();

    spdlog::info("[{}] Disconnected from vision server and vision service.", Base::UDP::Client::clientName().toStdString());
}

void Vision::resetAppearedControl() {
    // Reset appeared control
    _appeared.clear();

    // For each color, recreate the list
    magic_enum::enum_for_each<Common::Enums::Color>([this] (Common::Enums::Color color) {
        QMap<quint8, bool>* fieldObjectsControl = new QMap<quint8, bool>();
        for(int i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
            fieldObjectsControl->insert(i, false);
        }
        _appeared.insert(color, fieldObjectsControl);
    });
}

void Vision::updatePlayer(const Common::Enums::Color& teamColor, const quint8& playerId, const Geometry::Vector2D& pos, const Geometry::Angle& ori, const float& confidence) {
    // Check if team is registered in map
    if(!_robots.contains(teamColor)) {
        _robots.insert(teamColor, new QMap<quint8, VisionObject*>());
    }

    // Take team map
    QMap<quint8, VisionObject*> *teamMap = _robots.value(teamColor);

    // Check if teamMap has the required player
    if(!teamMap->contains(playerId)) {
        teamMap->insert(playerId, new VisionObject());
    }

    // Take object
    VisionObject* playerObject = teamMap->value(playerId);

    // Mark as appeared
    _appeared[teamColor]->insert(playerId, true);

    // Update
    playerObject->updateObject(confidence, pos, ori);
}

void Vision::updateNotAppearedRobotsFromColor(const Common::Enums::Color& teamColor) {
    // Run for every player
    for(int i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
        // Take control (true if appeared)
        bool appeared = _appeared.value(teamColor)->value(i);

        // If not appeared
        if(!appeared) {
            // Take team map
            QMap<quint8, VisionObject*> *teamMap = _robots.value(teamColor);

            // Take object and update it without args (loss)
            VisionObject* playerObject = teamMap->value(i);
            playerObject->updateObject();
        }
    }
}
