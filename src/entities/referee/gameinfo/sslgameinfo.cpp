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


#include "sslgameinfo.h"

SSLGameInfo::SSLGameInfo() {
    _ourStateColor = (Constants::teamColor()) ? BLUE : YELLOW;
    _goalieIdFromReferee = ROBOT_INVALID_ID; // invalid
    setState(HALTED);
}

void SSLGameInfo::updateGameInfo(const Referee &refereeMessage) {
    _stateMutex.lock();

    // Take goalie from packet
    if(_ourStateColor & BLUE) {
        if(refereeMessage.blue().has_goalkeeper()) {
            _goalieIdFromReferee = refereeMessage.blue().goalkeeper();
        }
    }
    else {
        if(refereeMessage.yellow().has_goalkeeper()) {
            _goalieIdFromReferee = refereeMessage.yellow().goalkeeper();
        }
    }

    _stateMutex.unlock();

    // Check if need to process command (!= from the previous)
    bool needsToProcessCommand = (refereeMessage.command_counter() != _lastRefereeCommand.command_counter());

    // Copy received message to last message
    _lastRefereeCommand.CopyFrom(refereeMessage);

    if(needsToProcessCommand) {
        processCommand();
    }
}

quint8 SSLGameInfo::getGoalieID() {
    _stateMutex.lock();
    quint8 goalieID = _goalieIdFromReferee;
    _stateMutex.unlock();

    return goalieID;
}

Referee::Stage SSLGameInfo::gameStage() {
    _stateMutex.lock();
    Referee::Stage stage = _lastRefereeCommand.stage();
    _stateMutex.unlock();

    return stage;
}

Referee_Command SSLGameInfo::refereeCommand() {
    _stateMutex.lock();
    Referee_Command command = _lastRefereeCommand.command();
    _stateMutex.unlock();

    return command;
}

Referee_TeamInfo SSLGameInfo::ourTeamInfo() {
    _stateMutex.lock();

    Referee_TeamInfo teamInfo;
    if(_ourStateColor & BLUE) teamInfo = _lastRefereeCommand.blue();
    else                      teamInfo = _lastRefereeCommand.yellow();

    _stateMutex.unlock();

    return teamInfo;
}

Referee_TeamInfo SSLGameInfo::theirTeamInfo() {
    _stateMutex.lock();

    Referee_TeamInfo teamInfo;
    if(_ourStateColor & BLUE) teamInfo = _lastRefereeCommand.yellow();
    else                      teamInfo = _lastRefereeCommand.blue();

    _stateMutex.unlock();

    return teamInfo;
}

Referee_Point SSLGameInfo::ballPlacementPosition() {
    _stateMutex.lock();
    Referee_Point placementPosition = _lastRefereeCommand.designated_position();
    _stateMutex.unlock();

    return placementPosition;
}

void SSLGameInfo::setBallKicked() {
    if(getState() & READY) {
        setState(GAME_ON);
        spdlog::info("[SSLReferee] Ball kicked, game state set to GAME_ON.");
    }
}

void SSLGameInfo::setState(int state) {
    _stateMutex.lock();
    _refereeState = state;
    _stateMutex.unlock();
}

int SSLGameInfo::getState() {
    _stateMutex.lock();
    int state = _refereeState;
    _stateMutex.unlock();

    return state;
}

void SSLGameInfo::processCommand() {
    Referee_Command lastRefereeCommand = refereeCommand();

    switch(lastRefereeCommand) {
        case Referee_Command_TIMEOUT_YELLOW:         setState(TIMEOUT);                         break;
        case Referee_Command_TIMEOUT_BLUE:           setState(TIMEOUT);                         break;
        case Referee_Command_HALT:                   setState(HALTED);                          break;

        case Referee_Command_GOAL_YELLOW:
        case Referee_Command_GOAL_BLUE:
        case Referee_Command_STOP:                   setState(GAME_OFF);                        break;

        case Referee_Command_FORCE_START:            setState(GAME_ON);                         break;

        case Referee_Command_PREPARE_KICKOFF_YELLOW: setState( KICKOFF | YELLOW | NOTREADY );  	break;
        case Referee_Command_PREPARE_KICKOFF_BLUE:   setState( KICKOFF | BLUE | NOTREADY);	  	break;

        case Referee_Command_PREPARE_PENALTY_YELLOW: setState( PENALTY | YELLOW | NOTREADY);	break;
        case Referee_Command_PREPARE_PENALTY_BLUE:	 setState( PENALTY | BLUE | NOTREADY );	  	break;

        case Referee_Command_DIRECT_FREE_YELLOW:	 setState( DIRECT | YELLOW | READY );	  	break;
        case Referee_Command_DIRECT_FREE_BLUE:		 setState( DIRECT | BLUE | READY);	  		break;

        case Referee_Command_INDIRECT_FREE_YELLOW:	 setState( INDIRECT | YELLOW | READY);	  	break;
        case Referee_Command_INDIRECT_FREE_BLUE:	 setState( INDIRECT | BLUE | READY);        break;

        case Referee_Command_BALL_PLACEMENT_BLUE:    setState( BALL_PLACEMENT | BLUE);          break;
        case Referee_Command_BALL_PLACEMENT_YELLOW:  setState( BALL_PLACEMENT | YELLOW);        break;

        case Referee_Command_NORMAL_START:
            if(getState() & NOTREADY) {
                setState(getState() & ~NOTREADY); // bit 11 -> 0
                setState(getState() | READY);     // bit 10 -> 1
            }
            else {
                // normal start without last command
                setState(GAME_ON);
            }

        break;
    }

    std::string_view parsedCommand = magic_enum::enum_name<Referee_Command>(lastRefereeCommand);
    std::string parsedCommandStr = {parsedCommand.begin(), parsedCommand.end()};
    QString commandName = QString(parsedCommandStr.c_str()).split("Referee_Command_").last();
    spdlog::info("[SSLReferee] Processed SSLReferee command: '{}'", commandName.toStdString());
}
