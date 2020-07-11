#include "ballsensor.h"

#include <utils/utils.hh>

BallSensor::BallSensor(SSLReferee *ref, WorldMap *wm) : Entity(ENT_BALLSENSOR){
    _ref = ref;
    _wm = wm;

    _lastBallPos.setUnknown();
}

QString BallSensor::name() {
    return "BALL_SENSOR";
}

void BallSensor::initialization() {
    std::cout << MRCConstants::defaultBold << "[BALL_SENSOR] " << MRCConstants::green << "Thread started.\n" << MRCConstants::reset;
}

void BallSensor::loop() {
    if(_lastBallPos.isUnknown())
        _lastBallPos = _wm->ballPosition(0);
    else {
        Position pos = _wm->ballPosition(0);

        float dist = WR::Utils::distance(pos, _lastBallPos);
        if(dist >= MRCConstants::_distToConsiderBallMovement) {
            _lastBallPos = pos;
            _ref->setBallKicked();
        }
    }
}

void BallSensor::finalization() {
    std::cout << MRCConstants::defaultBold << "[BALL_SENSOR] " << MRCConstants::green << "Thread ended.\n" << MRCConstants::reset;
}
