#include "ballsensor.h"

#include <utils/utils.hh>

BallSensor::BallSensor(SSLReferee *ref, WorldMap *wm,  MRCConstants *mrcconstants) : Entity(ENT_BALLSENSOR){
    _ref = ref;
    _wm = wm;
    _mrcconstants=mrcconstants;

    _lastBallPos.setUnknown();
}

QString BallSensor::name() {
    return "BALL_SENSOR";
}

void BallSensor::initialization() {
    std::cout << MRCConstants::defaultBold << "[BALL_SENSOR] " << MRCConstants::green << "Thread started.\n" << MRCConstants::reset;
}

void BallSensor::loop() {
    if(getConstants()==NULL) return;
    if(_lastBallPos.isUnknown())
        _lastBallPos = _wm->ballPosition(0);
    else {
        Position pos = _wm->ballPosition(0);

        float dist = WR::Utils::distance(pos, _lastBallPos);
        if(dist >= _mrcconstants->getDistToConsiderBallMovement()) {
            _lastBallPos = pos;
            _ref->setBallKicked();
        }
    }
}

void BallSensor::finalization() {
    std::cout << MRCConstants::defaultBold << "[BALL_SENSOR] " << MRCConstants::green << "Thread ended.\n" << MRCConstants::reset;
}


MRCConstants *BallSensor::getConstants() {
    if(_mrcconstants==NULL)
        std::cout << MRCConstants::red << "[ERROR] " << MRCConstants::reset << name().toStdString() << ", requesting getConstants(), _mrcconstants not initialized!\n";
    return _mrcconstants;
}
