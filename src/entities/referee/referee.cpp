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

#include "referee.h"

#define YELLOW 0
#define BLUE 1

SSLReferee::SSLReferee(Constants *constants, WorldMap *worldMap) : Entity() {
    // Take constants and worldmap
    _constants = constants;
    _worldMap = worldMap;

    // Set default data
    _remainingTime = 0;
    _lastCommand = Referee_Command_HALT;
    _lastStage = Referee_Stage_NORMAL_FIRST_HALF_PRE;

    // novo
    _lastTeamsInfo.insert(true, Referee_TeamInfo());
    _lastTeamsInfo.insert(false, Referee_TeamInfo());

    // antigo
    for(int i = YELLOW; i <= BLUE; i++) {
        _lastTeamsInfo.insert(i, Referee_TeamInfo());
    }

    // Create ballplay pointer
    _ballPlay = new BallPlay(getConstants(), getWorldMap());

    // Start gameinfo
    _gameInfo = new GameInfo(getConstants());
}

SSLReferee::~SSLReferee() {
    // Check if socket is open and close it
    if(_refereeSocket->isOpen()) {
        _refereeSocket->close();
    }

    // Delete socket pointer
    delete _refereeSocket;

    // Delete ballplay pointer
    delete _ballPlay;

    // Delete gameInfo pointer
    delete _gameInfo;
}

QString SSLReferee::name() {
    return "Referee";
}

void SSLReferee::initialization() {
    // Initialize network connection
    createAndInitializeSocket();
}

void SSLReferee::loop() {
    while(_refereeSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram;
        Referee packet;

        // Check if datagram is
        datagram = _refereeSocket->receiveDatagram();
        if(!datagram.isValid()) {
            continue;
        }

        // Parsing datagram and checking if it worked properly
        if(packet.ParseFromArray(datagram.data().data(), datagram.data().size()) == false) {
            std::cout << Text::red("[REFEREE] ", true) << Text::red("Referee packet parsing error.", true) + '\n';
            continue;
        }

        _packetMutex.lockForWrite();

        // Check if is to swap side
        if(packet.blue_team_on_positive_half()) {
            if(getConstants()->isTeamBlue()) {
                // If is blue team and is at left (negative) side, swap

                if(getConstants()->getTeamSide().isLeft()) {
                    getConstants()->swapTeamSide();
                }
            }
            else {
                // If is yellow team and is at right (positive) side, swap
                if(getConstants()->getTeamSide().isRight()) {
                    getConstants()->swapTeamSide();
                }
            }
        }
        else {
            // Case blue is at negative half (left)
            if(getConstants()->isTeamYellow()) {
                // If is yellow team and is at left (negative) side, swap
                if(getConstants()->getTeamSide().isLeft()) {
                    getConstants()->swapTeamSide();
                }
            }
            else {
                // If is blue and is at right (positive) side, swap
                if(getConstants()->getTeamSide().isRight()) {
                    getConstants()->swapTeamSide();
                }
            }
        }

        // Set remaining time
        _remainingTime = packet.stage_time_left()/1E6;

        // Fill blue team info
        if(packet.has_blue()) {
            _lastTeamsInfo.take(true);
            _lastTeamsInfo.insert(true, packet.blue());
        }

        // Fill yellow team info
        if(packet.has_yellow()) {
            _lastTeamsInfo.take(YELLOW);
            _lastTeamsInfo.insert(YELLOW, packet.yellow());
        }

        // Fill stage info
        if(packet.has_stage()) {
            if(_lastStage != packet.stage()) {
                std::cout << Text::purple("[REFEREE] ", true) << Text::bold("Received new stage: " + Referee_Stage_Name(packet.stage())) + '\n';
            }
            _lastStage = packet.stage();
        }

        // Fill command info
        if(packet.has_command()) {
            if(_lastCommand != packet.command()) {
                std::cout << Text::purple("[REFEREE] ", true) << Text::bold("Received new command: " + Referee_Command_Name(packet.command())) + '\n';

                // Process command in gameInfo
                _gameInfo->processCommand(packet.command());

                // If is an free kick, kickoff or penalty, reset ballPlay
                if(_gameInfo->freeKick() || _gameInfo->kickoff() || _gameInfo->penaltyKick()) {
                    _ballPlay->resetPlay();
                }
            }

            _lastCommand = packet.command();
        }

        // Fill placement position info
        if(packet.has_designated_position()) {
            _lastPlacementPosition = Position();
            _lastPlacementPosition.set_x(packet.designated_position().x()/1000.0f);
            _lastPlacementPosition.set_y(packet.designated_position().y()/1000.0f);
            _lastPlacementPosition.set_isinvalid(false);
        }

        _packetMutex.unlock();

        emit updateReferee();
    }

    // Check if ballplay has succeeded
    if(_ballPlay->isBallInPlay() && !getGameInfo()->ballPlacement()) {
        _gameInfo->setBallKicked();
    }
}

void SSLReferee::finalization() {
    std::cout << Text::cyan("[REFEREE] ", true) + Text::bold("Client finished.") + '\n';
}

GameInfo* SSLReferee::getGameInfo() {
    return _gameInfo;
}

int SSLReferee::getRemainingTime() {
    _packetMutex.lockForRead();
    int remainingTime = _remainingTime;
    _packetMutex.unlock();

    return remainingTime;
}

Referee_Stage SSLReferee::getLastStage() {
    _packetMutex.lockForRead();
    Referee_Stage lastStage = _lastStage;
    _packetMutex.unlock();

    return lastStage;
}

Referee_Command SSLReferee::getLastCommand() {
    _packetMutex.lockForRead();
    Referee_Command lastCommand = _lastCommand;
    _packetMutex.unlock();

    return lastCommand;
}

Referee_TeamInfo SSLReferee::getLastTeamInfo(Color teamColor) {
    bool color = YELLOW;
    if (teamColor.isblue()){
        color = BLUE;
    }
    _packetMutex.lockForRead();
    Referee_TeamInfo lastTeamInfo = _lastTeamsInfo.value(color);
    _packetMutex.unlock();

    return lastTeamInfo;
}

Position SSLReferee::getLastPlacementPosition() {
    _packetMutex.lockForRead();
    Position lastPlacementPosition = _lastPlacementPosition;
    _packetMutex.unlock();

    return lastPlacementPosition;
}

bool SSLReferee::isBallInPlay() {

    Position ballPosition = getWorldMap()->getBall().ballposition();
    return !(!getGameInfo()->canKickBall()
             || Utils::isInsideOurArea(ballPosition)
             || Utils::isInsideTheirArea(ballPosition)
             || Utils::isOutsideField(ballPosition));
}

void SSLReferee::createAndInitializeSocket() {
    // Create pointer
    _refereeSocket = new QUdpSocket();

    // Connect to referee multicast
    // Bind socket
    if(_refereeSocket->bind(QHostAddress(getConstants()->getRefereeAddress()), getConstants()->getRefereePort(), QUdpSocket::ShareAddress) == false) {
        std::cout << Text::cyan("[REFEREE] ", true) + Text::bold("Failed to bind referee socket at address '" + getConstants()->getRefereeAddress().toStdString() + "' and port '" + std::to_string(getConstants()->getRefereePort()) + "'") + '\n';
        return ;
    }

    // Join multicast
    if(_refereeSocket->joinMulticastGroup(QHostAddress(getConstants()->getRefereeAddress())) == false) {
        std::cout << Text::cyan("[REFEREE] ", true) + Text::bold("Failed to bind referee socket at address '" + getConstants()->getRefereeAddress().toStdString() + "'") + '\n';
        return ;
    }

    std::cout << Text::cyan("[REFEREE] ", true) + Text::bold("Started at address '"
                                                             + getConstants()->getRefereeAddress().toStdString() + "' and port '"
                                                             + std::to_string(getConstants()->getRefereePort()) + "'") + '\n';
}

Constants* SSLReferee::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Referee") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

WorldMap* SSLReferee::getWorldMap() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("WorldMap with nullptr value at Referee") + '\n';
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
