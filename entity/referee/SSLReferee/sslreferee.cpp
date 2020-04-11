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
#include <iostream>
#include <entity/referee/SSLReferee/sslgameinfo.h>

QString SSLReferee::name() {
    return "SSLReferee";
}

SSLReferee::SSLReferee(QString ipAddress, int port) {
    // from http://robocupssl.cpe.ku.ac.th/referee:protocol
    _ipAddress = QHostAddress(ipAddress);
    _port = port;
}

SSLReferee::~SSLReferee() {
    // Delete game infos
    QMap<Colors::Color, SSLGameInfo*>::Iterator it;
    for(it = _gameInfo.begin(); it != _gameInfo.end(); ++it )
        delete it.value();
}

SSLGameInfo* SSLReferee::getGameInfo(Colors::Color color) {
    // Return game info
    if(_gameInfo.contains(color))
        return _gameInfo.value(color);
    else {
        std::cout << "[ERROR] SSLReferee::getGameInfo(" << (color==Colors::YELLOW?"yellow":"blue") << "), color doesnt exists!\n";
        return NULL;
    }
}

void SSLReferee::addGameInfo(Colors::Color color) {
    // Add game info color
    if(_gameInfo.contains(color)==false) {
        SSLGameInfo *ginfo = new SSLGameInfo(color);
        _gameInfo.insert(color, ginfo);
    }
}

void SSLReferee::setBallKicked() {
    // Set ball kicked on game infos
    QList<SSLGameInfo*> gameInfos = _gameInfo.values();
    for(int i=0; i<_gameInfo.size(); i++) {
        SSLGameInfo *gInfo = gameInfos.at(i);
        gInfo->setBallKicked();
    }
}

void SSLReferee::initialization() {
    // Create socket
    _socket = new QUdpSocket();

    // Bind socket
    if(_socket->bind(QHostAddress::AnyIPv4, _port, QUdpSocket::ShareAddress)==false)
        std::cout << "[ERROR] SSLReferee: bind socket error (" << _socket->errorString().toStdString() << ")\n";

    // Join multicast group
    if(_socket->joinMulticastGroup(_ipAddress)==false)
        std::cout << "[ERROR] SSLReferee: failed to join multicast group (" << _socket->errorString().toStdString() << ")\n";

    std::cout << "[SSLREFEREE] thread started.\n";
}

void SSLReferee::loop() {
    SSL_Referee refereePacket;
    char buffer[65535];
    int len = 0;

    // While has datagrams to process
    while(_socket->hasPendingDatagrams()) {

        // Read datagram
        len = _socket->readDatagram(buffer, 65535);

        // Parse protobuf structure
        if(refereePacket.ParseFromArray(buffer, len)==false)
            std::cout << "[ERROR] SSLReferee: protobuf referee packet parsing error!\n";

        // Update game infos
        QList<SSLGameInfo*> gameInfos = _gameInfo.values();
        for(int i=0; i<_gameInfo.size(); i++) {
            SSLGameInfo *gInfo = gameInfos.at(i);
            gInfo->updateGameInfo(refereePacket);
        }
    }
}

void SSLReferee::finalization() {
    // Leave multicast group
    _socket->leaveMulticastGroup(_ipAddress);
    _socket->close();

    // Delete socket
    delete _socket;

    std::cout << "[SSLREFEREE] thread ended.\n";
}
