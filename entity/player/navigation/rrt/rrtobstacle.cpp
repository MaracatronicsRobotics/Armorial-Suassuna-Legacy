#include "rrtobstacle.h"

RRTObstacle::RRTObstacle(const Position &pos, float radius) {
    _pos = pos;
    _radius = radius;
}

RRTObstacle::~RRTObstacle() {

}

void RRTObstacle::setPos(const Position &pos) {
    _pos = pos;
}

void RRTObstacle::setPos(float x, float y) {
    _pos.setPosition(x, y, 0.0);
}

void RRTObstacle::setRadius(float radius) {
    _radius = radius;
}

Position RRTObstacle::getPos() const {
    return _pos;
}

float RRTObstacle::getRadius() const {
    return _radius;
}
