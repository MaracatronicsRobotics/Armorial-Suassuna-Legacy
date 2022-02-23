#ifndef BALLPLAY_H
#define BALLPLAY_H

#include <proto/messages.pb.h>
#include <src/constants/constants.h>
// include worldMap when its done
// #include <src/entities/world/worldMap.h>

class BallPlay
{
public:
    BallPlay(Constants *constants, World *world);

    void resetPlay(bool initialState = false);
    bool isBallInPlay();

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    World *_world;
    World* getWorld();

    // Internal
    bool _ballInPlay;
    Position _initialBallPosition;
};

#endif // BALLPLAY_H
