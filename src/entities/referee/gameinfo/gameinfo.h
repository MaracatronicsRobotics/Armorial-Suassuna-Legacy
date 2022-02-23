#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <proto/ssl_gc_referee_message.pb.h>
#include <src/entities/referee/refereeflags.h>
#include <src/constants/constants.h>

class GameInfo
{
public:
    GameInfo(Constants *constants);

    // Getters
    bool gameOn();

    bool kickoff();
    bool ourKickoff();
    bool theirKickoff();

    bool penaltyKick();
    bool ourPenaltyKick();
    bool theirPenaltyKick();

    bool directKick();
    bool ourDirectKick();
    bool theirDirectKick();

    bool indirectKick();
    bool ourIndirectKick();
    bool theirIndirectKick();

    bool freeKick();
    bool ourFreeKick();
    bool theirFreeKick();

    bool timeOut();

    bool canMove();

    bool ballPlacement();
    bool ourBallPlacement();
    bool theirBallPlacement();

    bool allowedNearBall();
    bool canKickBall();

    // Updaters
    void processCommand(Referee_Command cmd);
    void setBallKicked();

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // Internal
    int _state;
    int _stateColor;
    void setState(int state);
    int getState();

};

#endif // GAMEINFO_H
