#include "rrtnode.h"

RRTNode::RRTNode() {
    _compareByX = true;
    _nodeL = NULL;
    _nodeR = NULL;
    _nodeNN = NULL;
    _pos.setPosition(0.0, 0.0, 0.0);
}

RRTNode::RRTNode(Position pos) {
    _compareByX = true;
    _nodeL = NULL;
    _nodeR = NULL;
    _nodeNN = NULL;
    _pos = pos;
}
