#ifndef NODE_H
#define NODE_H

#include <GEARSystem/Types/types.hh>
#include <cstdlib>

class RRTNode {
public:
    RRTNode();
    RRTNode(Position pos);

    // Setters
    void setNodeL (RRTNode* node) { _nodeL  = node; }
    void setNodeR (RRTNode* node) { _nodeR  = node; }
    void setNodeNN(RRTNode* node) { _nodeNN = node; }
    void setPosition(Position pos) { _pos = pos; }
    void setCompareByX(bool compareByX) { _compareByX = compareByX; }

    // Getters
    RRTNode* getNodeL()  { return _nodeL; }
    RRTNode* getNodeR()  { return _nodeR; }
    RRTNode* getNodeNN() { return _nodeNN; }
    Position getPosition() { return _pos; }
    bool getCompareByX() { return _compareByX; }

private:
    // Bool for control if this node will position it's leaves compare x or y
    bool _compareByX;

    // Pointers for left, rigth and nearest leaf
    RRTNode* _nodeL;
    RRTNode* _nodeR;
    RRTNode* _nodeNN;

    // Position stored
    Position _pos;
};

#endif // NODE_H
