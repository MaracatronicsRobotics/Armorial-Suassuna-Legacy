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

#include "gameinfo.h"
#include <iostream>

const int GameInfo::GAME_ON;
const int GameInfo::GAME_OFF;
const int GameInfo::HALTED;

const int GameInfo::KICKOFF ;
const int GameInfo::PENALTY;
const int GameInfo::DIRECT ;
const int GameInfo::INDIRECT ;
const int GameInfo::RESTART;

const int GameInfo::BLUE;
const int GameInfo::YELLOW ;

const int GameInfo::READY ;
const int GameInfo::NOTREADY ;

const int GameInfo::TIMEOUT ;

const int GameInfo::BALL_PLACEMENT;

GameInfo::GameInfo(Color color) : Entity() {
    _color = color;
    setState(HALTED);
//    setState(GAME_ON);
//    setState(GAME_OFF);

    (color.isblue())? _stateColor = BLUE : _stateColor = YELLOW;

    _goalie = 200;
}

void GameInfo::updateGameInfo(SSLReferee &ref) {
    bool procCmd = false;

    mLastRefPack.lock();

    // Get goalie
    if(_color.isblue()) {
        if(lastRefPack.blue().has_goalkeeper())
            _goalie = lastRefPack.blue().goalkeeper();
    } else {
        if(lastRefPack.yellow().has_goalkeeper())
            _goalie = lastRefPack.yellow().goalkeeper();
    }
    procCmd = (ref.command_counter() != lastRefPack.command_counter());

    lastRefPack.CopyFrom(ref);
    mLastRefPack.unlock();

    if(procCmd)
        processCommand();
}

int GameInfo::getYellowCards(Color colorAt){
    if(colorAt.isblue()) {
        return lastRefPack.blue().yellow_cards();
    } else {
        return lastRefPack.yellow().yellow_cards();
    }
}

int GameInfo::getRedCards(Color colorAt){
    if(colorAt.isblue()) {
        return lastRefPack.blue().red_cards();
    } else {
        return lastRefPack.yellow().red_cards();
    }
}

Color GameInfo::getColor() const {
    return _color;
}

bool GameInfo::isYellow() const {
    return (!_color.isblue());
}

bool GameInfo::isBlue() const {
    return (_color.isblue());
}

Referee_Stage GameInfo::stage() {
    mLastRefPack.lock();
    Referee_Stage localStage = lastRefPack.stage();
    mLastRefPack.unlock();

    return localStage;
}

Referee_Command GameInfo::command() {
    mLastRefPack.lock();
    Referee_Command localCommand = lastRefPack.command();
    mLastRefPack.unlock();

    return localCommand;
}

Referee_TeamInfo GameInfo::ourTeamInfo() {
    Referee_TeamInfo localInfo;

    if (_color.isblue()) {
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

Referee_TeamInfo GameInfo::theirTeamInfo() {
    Referee_TeamInfo localInfo;

    if (!_color.isblue()) {
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

int GameInfo::getState() {
    int state;

    mState.lock();
    state = _state;
    mState.unlock();

    return state;
}

void GameInfo::setState(int state) {
    mState.lock();
    _state = state;
    mState.unlock();
}

void GameInfo::setBallKicked() {
    if(getState() & READY) {
        setState(GAME_ON);
        if(!_color.isblue()) // Avoid printing for both yellow and blue
            std::cout << Text::cyan("[GameInfo] ") << Text::bold("Ball KICKED, game state set to GAMEON.\n");
    }
}

void GameInfo::processCommand() {
    mProcessCmd.lock();

    mLastRefPack.lock();
    Referee_Command ref_command = lastRefPack.command();
    mLastRefPack.unlock();

    switch(ref_command) {
        case Referee_Command_TIMEOUT_YELLOW:        setState(TIMEOUT);                          break;
        case Referee_Command_TIMEOUT_BLUE:          setState(TIMEOUT);                          break;
        case Referee_Command_HALT:                  setState(HALTED);                           break;

        case Referee_Command_GOAL_YELLOW:
        case Referee_Command_GOAL_BLUE:
        case Referee_Command_STOP:                  setState(GAME_OFF);                         break;

        case Referee_Command_FORCE_START:           setState(GAME_ON);                          break;

        case Referee_Command_PREPARE_KICKOFF_YELLOW:setState( KICKOFF | YELLOW | NOTREADY );  	break;
        case Referee_Command_PREPARE_KICKOFF_BLUE:  setState( KICKOFF | BLUE | NOTREADY);	  	break;

        case Referee_Command_PREPARE_PENALTY_YELLOW:setState( PENALTY | YELLOW | NOTREADY);	  	break;
        case Referee_Command_PREPARE_PENALTY_BLUE:	setState( PENALTY | BLUE | NOTREADY );	  	break;

        case Referee_Command_DIRECT_FREE_YELLOW:	setState( DIRECT | YELLOW | READY );	  	break;
        case Referee_Command_DIRECT_FREE_BLUE:		setState( DIRECT | BLUE | READY);	  		break;

        case Referee_Command_INDIRECT_FREE_YELLOW:	setState( INDIRECT | YELLOW | READY);	  	break;
        case Referee_Command_INDIRECT_FREE_BLUE:	setState( INDIRECT | BLUE | READY);	  		break;

        case Referee_Command_BALL_PLACEMENT_BLUE:   setState( BALL_PLACEMENT | BLUE);           break;
        case Referee_Command_BALL_PLACEMENT_YELLOW: setState( BALL_PLACEMENT | YELLOW);         break;

        case Referee_Command_NORMAL_START:
            if(getState() & NOTREADY) {
                setState(getState() & ~NOTREADY); // bit 11 -> 0
                setState(getState() | READY);     // bit 10 -> 1
            } else // normal start without last command
                setState(GAME_ON);
        break;
    }
    mProcessCmd.unlock();

    if(!_color.isblue()) { // Avoid printing for both yellow and blue
        std::string strcommand = refCommandToString(ref_command);
        std::cout << Text::cyan("[GameInfo] ") << Text::bold("Processed SSLReferee command: ") << strcommand.c_str() << "\n";
        if(ref_command == Referee_Command_BALL_PLACEMENT_BLUE
           || ref_command == Referee_Command_BALL_PLACEMENT_YELLOW){
            std::cout << Text::cyan("[GameInfo] ") << Text::bold("Placement Position X: ") << desiredPosition().x() << Text::bold(" Y: ") << desiredPosition().y() << "\n";
        }
    }
}

std::string GameInfo::refCommandToString(Referee_Command cmd) {
    switch(cmd) {
        case Referee_Command_TIMEOUT_YELLOW:        return "TIMEOUT YELLOW";
        case Referee_Command_TIMEOUT_BLUE:          return "TIMEOUT BLUE";
        case Referee_Command_HALT:                  return "HALT";
        case Referee_Command_GOAL_YELLOW:           return "GOAL YELLOW";
        case Referee_Command_GOAL_BLUE:             return "GOAL BLUE";
        case Referee_Command_STOP:                  return "STOP";
        case Referee_Command_FORCE_START:           return "FORCE START";
        case Referee_Command_PREPARE_KICKOFF_YELLOW:return "KICKOFF YELLOW";
        case Referee_Command_PREPARE_KICKOFF_BLUE:  return "KICKOFF BLUE";
        case Referee_Command_PREPARE_PENALTY_YELLOW:return "PENALTY YELLOW";
        case Referee_Command_PREPARE_PENALTY_BLUE:  return "PENALTY BLUE";
        case Referee_Command_DIRECT_FREE_YELLOW:    return "DIRECT FREE YELLOW";
        case Referee_Command_DIRECT_FREE_BLUE:      return "DIRECT FREE BLUE";
        case Referee_Command_INDIRECT_FREE_YELLOW:  return "INDIRECT FREE YELLOW";
        case Referee_Command_INDIRECT_FREE_BLUE:    return "INDIRECT FREE BLUE";
        case Referee_Command_NORMAL_START:          return "NORMAL START";
        case Referee_Command_BALL_PLACEMENT_BLUE:   return "PLACEMENT BLUE" ;
        case Referee_Command_BALL_PLACEMENT_YELLOW: return "PLACEMENT YELLOW";
        default:                                    return "UNDEFINED!";
    }
}


std::string GameInfo::refStageToString(Referee::Stage stage){
    switch(stage){
      case Referee_Stage_NORMAL_FIRST_HALF_PRE:          return "First_Half_Pre";
      case Referee_Stage_NORMAL_FIRST_HALF:              return "First_Half";
      case Referee_Stage_NORMAL_HALF_TIME:               return "Half_Time";
      case Referee_Stage_NORMAL_SECOND_HALF_PRE:         return "Second_Half_Pre";
      case Referee_Stage_NORMAL_SECOND_HALF:             return "Second_Half";
      case Referee_Stage_EXTRA_TIME_BREAK:               return "Extra_Time_Break";
      case Referee_Stage_EXTRA_FIRST_HALF_PRE:           return "Extra_First_Half_Pre";
      case Referee_Stage_EXTRA_FIRST_HALF:               return "Extra_First_Half";
      case Referee_Stage_EXTRA_HALF_TIME:                return "Extra_Half_Time";
      case Referee_Stage_EXTRA_SECOND_HALF_PRE:          return "Extra_Second_Half_Pre";
      case Referee_Stage_EXTRA_SECOND_HALF:              return "Extra_Second_Half";
      case Referee_Stage_PENALTY_SHOOTOUT_BREAK:         return "Penalty_Shootout_Break";
      case Referee_Stage_PENALTY_SHOOTOUT:               return "Penalty_Shootout";
      case Referee_Stage_POST_GAME:                      return "Post_Game";
      default:                                           return "UNDEFINED!";
    }
}

std::string GameInfo::refTimeLeftToString(){
    uint32_t timeLeft = lastRefPack.stage_time_left()/1e6;
    std::string str = std::to_string(timeLeft);
    str += " sec";

    return str;
}

Referee_Point GameInfo::desiredPosition(){
    return lastRefPack.designated_position();
}

bool GameInfo::gameOn(){ return getState() == GAME_ON ;}

bool GameInfo::restart() { return (getState() & RESTART); }
bool GameInfo::ourRestart() { return restart() && (getState() & _stateColor); }
bool GameInfo::theirRestart() { return restart() && ! (getState() & _stateColor); }

bool GameInfo::kickoff() { return (getState() & KICKOFF); }
bool GameInfo::ourKickoff() { return kickoff() && (getState() & _stateColor); }
bool GameInfo::theirKickoff() { return kickoff() && ! (getState() & _stateColor); }

bool GameInfo::penaltyKick() { return (getState() & PENALTY); }
bool GameInfo::ourPenaltyKick() { return penaltyKick() && (getState() & _stateColor); }
bool GameInfo::theirPenaltyKick() { return penaltyKick() && ! (getState() & _stateColor); }

bool GameInfo::directKick() { return (getState() & DIRECT); }
bool GameInfo::ourDirectKick() { return directKick() && (getState() & _stateColor); }
bool GameInfo::theirDirectKick() { return directKick() && ! (getState() & _stateColor); }

bool GameInfo::indirectKick() { return (getState() & INDIRECT); }
bool GameInfo::ourIndirectKick() { return indirectKick() && (getState() & _stateColor); }
bool GameInfo::theirIndirectKick() { return indirectKick() && ! (getState() & _stateColor); }

bool GameInfo::freeKick() { return directKick() || indirectKick(); }
bool GameInfo::ourFreeKick() { return ourDirectKick() || ourIndirectKick(); }
bool GameInfo::theirFreeKick() { return theirDirectKick() || theirIndirectKick(); }

bool GameInfo::ballPlacement(){ return (getState() & BALL_PLACEMENT); }
bool GameInfo::ourBallPlacement(){ return ballPlacement() && (getState() & _stateColor); }
bool GameInfo::theirBallPlacement(){ return ballPlacement() && !(getState() & _stateColor); }

bool GameInfo::timeOut() { return getState() == TIMEOUT; }

//Halt!//
bool GameInfo::canMove() { return (getState() != HALTED); }

bool GameInfo::allowedNearBall() {
    return gameOn() || (getState() & _stateColor); }


bool GameInfo::canKickBall() {
    return gameOn() || (ourRestart() && (getState() & READY));
}

GameInfo::RefProcessedState GameInfo::processedState() {
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
            if(ourBallPlacement())
                return STATE_OURBALLPLACEMENT;
        } else if(theirRestart()) {
            if(theirDirectKick())
                return STATE_THEIRDIRECTKICK;
            if(theirIndirectKick())
                return STATE_THEIRINDIRECTKICK;
            if(theirPenaltyKick())
                return STATE_THEIRPENALTY;
            if(theirKickoff())
                return STATE_THEIRKICKOFF;
            if(theirBallPlacement())
                return STATE_THEIRBALLPLACEMENT;
        } else
            return STATE_GAMEOFF;
    }

    // Should never reach here
    std::cout << ">> GameInfo: undefined state... something wrong!\n";

    return STATE_UNDEFINED;
}

