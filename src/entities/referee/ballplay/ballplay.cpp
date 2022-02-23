#include "ballplay.h"

#include <src/utils/text/text.h>

BallPlay::BallPlay(Constants *constants, World *worldMap) {
    _constants = constants;
    _worldMap = worldMap;
}

void BallPlay::resetPlay(bool initialState) {
    _ballInPlay = initialState;
    _initialBallPosition = getWorldMap()->getBall().getPosition();
}

bool BallPlay::isBallInPlay() {
    Position actualBallPosition = getWorldMap()->getBall().getPosition();

    if(Utils::distance(_initialBallPosition, actualBallPosition) >= getConstants()->minDistToConsiderBallMovement()) {
        _ballInPlay = true;
    }

    return _ballInPlay;
}

Constants* BallPlay::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at BallPlay") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

WorldMap* BallPlay::getWorldMap() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("WorldMap with nullptr value at BallPlay") + '\n';
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
