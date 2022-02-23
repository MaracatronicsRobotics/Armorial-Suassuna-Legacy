#include "gameinfo.h"

GameInfo::GameInfo(Constants *constants) {
    // Take constants pointer
    _constants = constants;

    // Set default value
    _state = GAME_OFF;

    _stateColor = (getConstants()->teamColor() == Colors::BLUE) ? TEAM_BLUE : TEAM_YELLOW;
}

bool GameInfo::gameOn() {
    return (getState() == GAME_ON);
}

bool GameInfo::kickoff() {
    return (getState() & KICKOFF);
}

bool GameInfo::ourKickoff() {
    return (kickoff() && (getState() & _stateColor));
}

bool GameInfo::theirKickoff() {
    return (kickoff() && !(getState() & _stateColor));
}

bool GameInfo::penaltyKick() {
    return (getState() & PENALTY);
}

bool GameInfo::ourPenaltyKick(){
    return (penaltyKick() && (getState() & _stateColor));
}

bool GameInfo::theirPenaltyKick(){
    return (penaltyKick() && !(getState() & _stateColor));
}

bool GameInfo::directKick() {
    return (getState() & DIRECT_KICK);
}

bool GameInfo::ourDirectKick() {
    return (directKick() && (getState() & _stateColor));
}

bool GameInfo::theirDirectKick() {
    return (directKick() && !(getState() & _stateColor));
}

bool GameInfo::indirectKick() {
    return (getState() & INDIRECT_KICK);
}

bool GameInfo::ourIndirectKick() {
    return (indirectKick() && (getState() & _stateColor));
}

bool GameInfo::theirIndirectKick() {
    return (indirectKick() && !(getState() & _stateColor));
}

bool GameInfo::freeKick() {
    return (directKick() || indirectKick());
}

bool GameInfo::ourFreeKick() {
    return (ourDirectKick() || ourIndirectKick());
}

bool GameInfo::theirFreeKick() {
    return (theirDirectKick() || theirIndirectKick());
}

bool GameInfo::timeOut() {
    return (getState() & TIMEOUT);
}

bool GameInfo::canMove() {
    return (getState() != HALT);
}

bool GameInfo::ballPlacement() {
    return (getState() & BALL_PLACEMENT);
}

bool GameInfo::ourBallPlacement() {
    return (ballPlacement() && (getState() & _stateColor));
}

bool GameInfo::theirBallPlacement() {
    return (ballPlacement() && !(getState() & _stateColor));
}

bool GameInfo::allowedNearBall() {
    return (gameOn() || (getState() & _stateColor));
}

bool GameInfo::canKickBall() {
    return (gameOn() || ((getState() & _stateColor) && (getState() & READY)));
}

void GameInfo::processCommand(Referee_Command cmd) {
    switch(cmd) {
        case Referee_Command_FORCE_START:{
            setState(GAME_ON);
        } break;
        case Referee_Command_NORMAL_START:{
            if(getState() & NOT_READY) { // For PRE commands
                setState(getState() & ~NOT_READY);
                setState(getState() | READY);
            }
            else {
                setState(GAME_ON);
            }
        } break;
        case Referee_Command_BALL_PLACEMENT_BLUE:{
            setState(BALL_PLACEMENT | TEAM_BLUE | READY);
        } break;
        case Referee_Command_BALL_PLACEMENT_YELLOW:{
            setState(BALL_PLACEMENT | TEAM_YELLOW | READY);
        } break;
        case Referee_Command_PREPARE_KICKOFF_BLUE:{
            setState(KICKOFF | TEAM_BLUE | NOT_READY);
        } break;
        case Referee_Command_PREPARE_KICKOFF_YELLOW:{
            setState(KICKOFF | TEAM_YELLOW | NOT_READY);
        } break;
        case Referee_Command_PREPARE_PENALTY_BLUE:{
            setState(PENALTY | TEAM_BLUE | NOT_READY);
        } break;
        case Referee_Command_PREPARE_PENALTY_YELLOW:{
            setState(PENALTY | TEAM_YELLOW | NOT_READY);
        } break;
        case Referee_Command_DIRECT_FREE_BLUE:{
            setState(DIRECT_KICK | TEAM_BLUE | READY);
        } break;
        case Referee_Command_DIRECT_FREE_YELLOW:{
            setState(DIRECT_KICK | TEAM_YELLOW | READY);
        } break;
        case Referee_Command_INDIRECT_FREE_BLUE:{
            setState(INDIRECT_KICK | TEAM_BLUE | READY);
        } break;
        case Referee_Command_INDIRECT_FREE_YELLOW:{
            setState(INDIRECT_KICK | TEAM_YELLOW | READY);
        } break;
        case Referee_Command_HALT:{
            setState(HALT);
        } break;
        case Referee_Command_GOAL_BLUE:{
            setState(HALT);
        } break;
        case Referee_Command_GOAL_YELLOW:{
            setState(HALT);
        } break;
        case Referee_Command_TIMEOUT_BLUE:{
            setState(TIMEOUT);
        } break;
        case Referee_Command_TIMEOUT_YELLOW:{
            setState(TIMEOUT);
        } break;
        case Referee_Command_STOP: {
            setState(GAME_OFF);
        } break;
    }
}

void GameInfo::setBallKicked() {
    if(getState() & READY) {
        setState(GAME_ON);
        std::cout << Text::purple("[GAMEINFO] ", true) + Text::bold("Ball KICKED, game state set to GAME_ON.") + '\n';
    }
}

int GameInfo::getState() {
    return _state;
}

void GameInfo::setState(int state) {
    _state = state;
}

Constants* GameInfo::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at GameInfo") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
