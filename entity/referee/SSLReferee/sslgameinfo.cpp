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
#include <iostream>

#include <entity/coachview/coachview.h>
#include <entity/coachview/mainwindow.h>

const int SSLGameInfo::GAME_ON;
const int SSLGameInfo::GAME_OFF;
const int SSLGameInfo::HALTED;

const int SSLGameInfo::KICKOFF ;
const int SSLGameInfo::PENALTY;
const int SSLGameInfo::DIRECT ;
const int SSLGameInfo::INDIRECT ;
const int SSLGameInfo::RESTART;

const int SSLGameInfo::BLUE;
const int SSLGameInfo::YELLOW ;

const int SSLGameInfo::READY ;
const int SSLGameInfo::NOTREADY ;

const int SSLGameInfo::TIMEOUT ;

SSLGameInfo::SSLGameInfo(Colors::Color color) {
    _color = color;
    setState(HALTED);
//    setState(GAME_ON);
//    setState(GAME_OFF);

    _UIMutex = new QMutex();

    (color==Colors::BLUE)? _stateColor = BLUE : _stateColor = YELLOW;

    _goalie = 200;
}

void SSLGameInfo::updateGameInfo(SSL_Referee &ref) {
    bool procCmd = false;

    mLastRefPack.lock();

    // Get goalie
    if(_color==Colors::YELLOW) {
        if(lastRefPack.yellow().has_goalie())
            _goalie = lastRefPack.yellow().goalie();
    } else {
        if(lastRefPack.blue().has_goalie())
            _goalie = lastRefPack.blue().goalie();
    }
    procCmd = (ref.command_counter() != lastRefPack.command_counter());

    lastRefPack.CopyFrom(ref);
    mLastRefPack.unlock();

    if(procCmd)
        processCommand();
}

int SSLGameInfo::getYellowCards(Colors::Color colorAt){
    if(colorAt==Colors::YELLOW) {
        return lastRefPack.yellow().yellow_cards();
    } else {
        return lastRefPack.blue().yellow_cards();
    }
}

int SSLGameInfo::getRedCards(Colors::Color colorAt){
    if(colorAt==Colors::YELLOW) {
        return lastRefPack.yellow().red_cards();
    } else {
        return lastRefPack.blue().red_cards();
    }
}

Colors::Color SSLGameInfo::getColor() const {
    return _color;
}

bool SSLGameInfo::isYellow() const {
    return (_color==Colors::YELLOW);
}

bool SSLGameInfo::isBlue() const {
    return (_color==Colors::BLUE);
}

SSL_Referee::Stage SSLGameInfo::stage() {
    mLastRefPack.lock();
    SSL_Referee_Stage localStage = lastRefPack.stage();
    mLastRefPack.unlock();

    return localStage;
}

SSL_Referee_Command SSLGameInfo::command() {
    mLastRefPack.lock();
    SSL_Referee_Command localCommand = lastRefPack.command();
    mLastRefPack.unlock();

    return localCommand;
}

SSL_Referee_TeamInfo SSLGameInfo::ourTeamInfo() {
    SSL_Referee_TeamInfo localInfo;

    if (_color == Colors::BLUE) {
        mLastRefPack.lock();
        localInfo = lastRefPack.blue();
        mLastRefPack.unlock();
    } else {
        mLastRefPack.lock();
        localInfo = lastRefPack.yellow();
        mLastRefPack.unlock();
    }
    return localInfo;
}

SSL_Referee_TeamInfo SSLGameInfo::theirTeamInfo() {
    SSL_Referee_TeamInfo localInfo;

    if (_color != Colors::BLUE) {
        mLastRefPack.lock();
        localInfo = lastRefPack.blue();
        mLastRefPack.unlock();
    } else {
        mLastRefPack.lock();
        localInfo = lastRefPack.yellow();
        mLastRefPack.unlock();
    }
    return localInfo;
}

int SSLGameInfo::getState() {
    int state;

    mState.lock();
    state = _state;
    mState.unlock();

    return state;
}

void SSLGameInfo::setState(int state) {
    mState.lock();
    _state = state;
    mState.unlock();
}

void SSLGameInfo::setBallKicked() {
    if(getState() & READY) {
        setState(GAME_ON);
        if(_color==Colors::YELLOW) // Avoid printing for both yellow and blue
            std::cout << "[SSLGameInfo] Ball KICKED, game state set to GAMEON.\n";
    }
}

void SSLGameInfo::processCommand() {
    mProcessCmd.lock();

    mLastRefPack.lock();
    SSL_Referee_Command ref_command = lastRefPack.command();
    mLastRefPack.unlock();

    switch(ref_command) {
        case SSL_Referee_Command_TIMEOUT_YELLOW:        setState(TIMEOUT);                          break;
        case SSL_Referee_Command_TIMEOUT_BLUE:          setState(TIMEOUT);                          break;
        case SSL_Referee_Command_HALT:                  setState(HALTED);                           break;

        case SSL_Referee_Command_GOAL_YELLOW:
        case SSL_Referee_Command_GOAL_BLUE:
        case SSL_Referee_Command_STOP:                  setState(GAME_OFF);                         break;

        case SSL_Referee_Command_FORCE_START:           setState(GAME_ON);                          break;

        case SSL_Referee_Command_PREPARE_KICKOFF_YELLOW:setState( KICKOFF | YELLOW | NOTREADY );  	break;
        case SSL_Referee_Command_PREPARE_KICKOFF_BLUE:  setState( KICKOFF | BLUE | NOTREADY);	  	break;

        case SSL_Referee_Command_PREPARE_PENALTY_YELLOW:setState( PENALTY | YELLOW | NOTREADY);	  	break;
        case SSL_Referee_Command_PREPARE_PENALTY_BLUE:	setState( PENALTY | BLUE | NOTREADY );	  	break;

        case SSL_Referee_Command_DIRECT_FREE_YELLOW:	setState( DIRECT | YELLOW | READY );	  	break;
        case SSL_Referee_Command_DIRECT_FREE_BLUE:		setState( DIRECT | BLUE | READY);	  		break;

        case SSL_Referee_Command_INDIRECT_FREE_YELLOW:	setState( INDIRECT | YELLOW | READY);	  	break;
        case SSL_Referee_Command_INDIRECT_FREE_BLUE:	setState(INDIRECT | BLUE | READY);	  		break;

        case SSL_Referee_Command_NORMAL_START:
            if(getState() & NOTREADY) {
                setState(getState() & ~NOTREADY); // bit 11 -> 0
                setState(getState() | READY);     // bit 10 -> 1
            } else // normal start without last command
                setState(GAME_ON);
        break;
    }
    mProcessCmd.unlock();

    if(_color==Colors::YELLOW) { // Avoid printing for both yellow and blue
        std::string strcommand = refCommandToString(ref_command);
        std::cout << "[SSLGameInfo] Processed SSLReferee command: " << strcommand.c_str() << "\n";
    }
}

std::string SSLGameInfo::refCommandToString(SSL_Referee_Command cmd) {
    switch(cmd) {
        case SSL_Referee_Command_TIMEOUT_YELLOW:        return "TIMEOUT YELLOW";
        case SSL_Referee_Command_TIMEOUT_BLUE:          return "TIMEOUT BLUE";
        case SSL_Referee_Command_HALT:                  return "HALT";
        case SSL_Referee_Command_GOAL_YELLOW:           return "GOAL YELLOW";
        case SSL_Referee_Command_GOAL_BLUE:             return "GOAL BLUE";
        case SSL_Referee_Command_STOP:                  return "STOP";
        case SSL_Referee_Command_FORCE_START:           return "FORCE START";
        case SSL_Referee_Command_PREPARE_KICKOFF_YELLOW:return "KICKOFF YELLOW";
        case SSL_Referee_Command_PREPARE_KICKOFF_BLUE:  return "KICKOFF BLUE";
        case SSL_Referee_Command_PREPARE_PENALTY_YELLOW:return "PENALTY YELLOW";
        case SSL_Referee_Command_PREPARE_PENALTY_BLUE:  return "PENALTY BLUE";
        case SSL_Referee_Command_DIRECT_FREE_YELLOW:    return "DIRECT FREE YELLOW";
        case SSL_Referee_Command_DIRECT_FREE_BLUE:      return "DIRECT FREE BLUE";
        case SSL_Referee_Command_INDIRECT_FREE_YELLOW:  return "INDIRECT FREE YELLOW";
        case SSL_Referee_Command_INDIRECT_FREE_BLUE:    return "INDIRECT FREE BLUE";
        case SSL_Referee_Command_NORMAL_START:          return "NORMAL START";
        default:                                        return "UNDEFINED!";
    }
}


std::string SSLGameInfo::refStageToString(SSL_Referee::Stage stage){
    switch(stage){
      case SSL_Referee_Stage_NORMAL_FIRST_HALF_PRE:          return "First_Half_Pre";
      case SSL_Referee_Stage_NORMAL_FIRST_HALF:              return "First_Half";
      case SSL_Referee_Stage_NORMAL_HALF_TIME:               return "Half_Time";
      case SSL_Referee_Stage_NORMAL_SECOND_HALF_PRE:         return "Second_Half_Pre";
      case SSL_Referee_Stage_NORMAL_SECOND_HALF:             return "Second_Half";
      case SSL_Referee_Stage_EXTRA_TIME_BREAK:               return "Extra_Time_Break";
      case SSL_Referee_Stage_EXTRA_FIRST_HALF_PRE:           return "Extra_First_Half_Pre";
      case SSL_Referee_Stage_EXTRA_FIRST_HALF:               return "Extra_First_Half";
      case SSL_Referee_Stage_EXTRA_HALF_TIME:                return "Extra_Half_Time";
      case SSL_Referee_Stage_EXTRA_SECOND_HALF_PRE:          return "Extra_Second_Half_Pre";
      case SSL_Referee_Stage_EXTRA_SECOND_HALF:              return "Extra_Second_Half";
      case SSL_Referee_Stage_PENALTY_SHOOTOUT_BREAK:         return "Penalty_Shootout_Break";
      case SSL_Referee_Stage_PENALTY_SHOOTOUT:               return "Penalty_Shootout";
      case SSL_Referee_Stage_POST_GAME:                      return "Post_Game";
      default:                                               return "UNDEFINED!";
    }
}

std::string SSLGameInfo::refTimeLeftToString(){
    uint32_t timeLeft = lastRefPack.stage_time_left()/1e6;
    std::string str = std::to_string(timeLeft);
    str += " sec";

    return str;
}

bool SSLGameInfo::gameOn(){ return getState() == GAME_ON ;}

bool SSLGameInfo::restart() { return (getState() & RESTART); }
bool SSLGameInfo::ourRestart() { return restart() && (getState() & _stateColor); }
bool SSLGameInfo::theirRestart() { return restart() && ! (getState() & _stateColor); }

bool SSLGameInfo::kickoff() { return (getState() & KICKOFF); }
bool SSLGameInfo::ourKickoff() { return kickoff() && (getState() & _stateColor); }
bool SSLGameInfo::theirKickoff() { return kickoff() && ! (getState() & _stateColor); }

bool SSLGameInfo::penaltyKick() { return (getState() & PENALTY); }
bool SSLGameInfo::ourPenaltyKick() { return penaltyKick() && (getState() & _stateColor); }
bool SSLGameInfo::theirPenaltyKick() { return penaltyKick() && ! (getState() & _stateColor); }

bool SSLGameInfo::directKick() { return (getState() & DIRECT); }
bool SSLGameInfo::ourDirectKick() { return directKick() && (getState() & _stateColor); }
bool SSLGameInfo::theirDirectKick() { return directKick() && ! (getState() & _stateColor); }

bool SSLGameInfo::indirectKick() { return (getState() & INDIRECT); }
bool SSLGameInfo::ourIndirectKick() { return indirectKick() && (getState() & _stateColor); }
bool SSLGameInfo::theirIndirectKick() { return indirectKick() && ! (getState() & _stateColor); }

bool SSLGameInfo::freeKick() { return directKick() || indirectKick(); }
bool SSLGameInfo::ourFreeKick() { return ourDirectKick() || ourIndirectKick(); }
bool SSLGameInfo::theirFreeKick() { return theirDirectKick() || theirIndirectKick(); }

bool SSLGameInfo::timeOut() { return getState() == TIMEOUT; }

//Halt!//
bool SSLGameInfo::canMove() { return (getState() != HALTED); }

bool SSLGameInfo::allowedNearBall() {
    return gameOn() || (getState() & _stateColor); }


bool SSLGameInfo::canKickBall() {
    return gameOn() || (ourRestart() && (getState() & READY));
}

SSLGameInfo::RefProcessedState SSLGameInfo::processedState() {
    if(timeOut()){
        return STATE_TIMEOUT;
    }

    if(!canMove())
        return STATE_CANTMOVE;

    if(gameOn())
        return STATE_GAMEON;
    else {
        if(ourRestart()) {
            if(ourDirectKick())
                return STATE_OURDIRECTKICK;
            if(ourIndirectKick())
                return STATE_OURINDIRECTKICK;
            if(ourPenaltyKick())
                return STATE_OURPENALTY;
            if(ourKickoff())
                return STATE_OURKICKOFF;
        } else if(theirRestart()) {
            if(theirDirectKick())
                return STATE_THEIRDIRECTKICK;
            if(theirIndirectKick())
                return STATE_THEIRINDIRECTKICK;
            if(theirPenaltyKick())
                return STATE_THEIRPENALTY;
            if(theirKickoff())
                return STATE_THEIRKICKOFF;
        } else
            return STATE_GAMEOFF;
    }

    // Should never reach here
    std::cout << ">> SSLGameInfo: undefined state... something wrong!\n";

    return STATE_UNDEFINED;
}

