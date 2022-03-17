#ifndef BALLPLAY_H
#define BALLPLAY_H

#include <src/utils/utils.h>
#include <src/constants/constants.h>
#include <src/entities/worldmap/worldmap.h>

#include <proto/messages.pb.h>

class BallPlay
{
public:
    BallPlay(Constants *constants, WorldMap *worldMap);

    void resetPlay(bool initialState = false);
    bool isBallInPlay();

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorldMap();

    // Internal
    bool _ballInPlay;
    Position _initialBallPosition;
};

#endif // BALLPLAY_H
