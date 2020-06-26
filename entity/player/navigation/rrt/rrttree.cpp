#include "rrttree.h"

RRTTree::RRTTree() {
    _root = NULL;
    _tree.clear();
    _stackOfNodes.clear();
}

RRTTree::~RRTTree() {
    while(!_tree.isEmpty()) {
        delete _tree.takeLast();
    }

    while(!_stackOfNodes.isEmpty()) {
        delete _stackOfNodes.takeLast();
    }
}

void RRTTree::append(RRTNode *leaf) {
    _tree.append(leaf);

    if(_root == NULL) {
        _root = leaf;
        return;
    }

    RRTNode* subTreeRoot = _root;

    while(true) {

        // Compare by X
        if(subTreeRoot->getCompareByX()) {
            // If node x > position x
            if(subTreeRoot->getPosition().x() >= leaf->getPosition().x()) {
                // If there no node in the left, than add a leaf
                if(subTreeRoot->getNodeL() == NULL) {
                    leaf->setCompareByX(false);
                    subTreeRoot->setNodeL(leaf);
                    return;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeL();
                    continue;
                }
            // If node x < position x
            } else {
                // If there no node in the rigth, than add a leaf
                if(subTreeRoot->getNodeR() == NULL) {
                    leaf->setCompareByX(false);
                    subTreeRoot->setNodeR(leaf);
                    return;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeR();
                    continue;
                }
            }
        }

        // Compare by Y
        else {

            // If node y > position y
            if(subTreeRoot->getPosition().y() >= leaf->getPosition().y()) {
                // If there no node in the left, than add a leaf
                if(subTreeRoot->getNodeL() == NULL) {
                    subTreeRoot->setNodeL(leaf);
                    leaf->setCompareByX(true);
                    return;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeL();
                    continue;
                }
            // If node y < position y
            } else {
                // If there no node in the rigth, than add a leaf
                if(subTreeRoot->getNodeR() == NULL) {
                    subTreeRoot->setNodeR(leaf);
                    leaf->setCompareByX(true);
                    return;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeR();
                    continue;
                }
            }
        }

    } // end While

}

RRTNode* RRTTree::getNN(Position myPos) {
    // Check if there is at least a node to check
    if(_root == NULL) {
        return NULL;
    }

    RRTNode* subTreeRoot = _root;
    Position nearPosSoFar;

    // Search for the local where leaf should be append
    while(true) {

        // Compare by X
        if(subTreeRoot->getCompareByX()) {
            // If node x > position x
            if(subTreeRoot->getPosition().x() >= myPos.x()) {
                // If there no node in the left
                if(subTreeRoot->getNodeL() == NULL) {
                    nearPosSoFar = subTreeRoot->getPosition();
                    break;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeL();
                    continue;
                }
            // If node x < position x
            } else {
                // If there no node in the rigth
                if(subTreeRoot->getNodeR() == NULL) {
                    nearPosSoFar = subTreeRoot->getPosition();
                    break;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeR();
                    continue;
                }
            }
        }

        // Compare by Y
        else {
            // If node y > position y
            if(subTreeRoot->getPosition().y() >= myPos.y()) {
                // If there no node in the left
                if(subTreeRoot->getNodeL() == NULL) {
                    nearPosSoFar = subTreeRoot->getPosition();
                    break;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeL();
                    continue;
                }
            // If node y < position y
            } else {
                // If there no node in the rigth
                if(subTreeRoot->getNodeR() == NULL) {
                    nearPosSoFar = subTreeRoot->getPosition();
                    break;
                // Else, continue to search depth in the tree
                } else {
                    subTreeRoot = subTreeRoot->getNodeR();
                    continue;
                }
            }
        }

    } // end While

    float bestSoFar = squareDistance(myPos, nearPosSoFar);
    float xMax = myPos.x() + bestSoFar;
    float xMim = myPos.x() - bestSoFar;
    float yMax = myPos.y() + bestSoFar;
    float yMim = myPos.y() - bestSoFar;

    RRTNode* nearNode = subTreeRoot;
    subTreeRoot = _root;
    QList<RRTNode*> nodesStack;

    // While there is more nodes to check in the stack and subTreeRoot isn't NULL
    while( !(subTreeRoot == NULL && nodesStack.isEmpty()) ) {

        // If subTreeRoot is NULL, search from nodesStack
        if(subTreeRoot == NULL)
            subTreeRoot = nodesStack.takeLast();

        // Update bestSoFar
        float dist = squareDistance(subTreeRoot->getPosition(), myPos);
        if(dist < bestSoFar) {
            bestSoFar = dist;
            nearNode = subTreeRoot;
        }

        // Check if this node is a compareByX
        if(subTreeRoot->getCompareByX()) {

            // Check if is needed to go left or rigth
            if(subTreeRoot->getPosition().x() >= xMim) {

                // Save pointer for right node to check later
                if(subTreeRoot->getNodeR() != NULL  && subTreeRoot->getPosition().x() < xMax)
                    nodesStack.append(subTreeRoot->getNodeR());

                // Save pointer for left node to check now
                subTreeRoot = subTreeRoot->getNodeL();
            } else if(subTreeRoot->getPosition().x() < xMax) {

                // Save pointer for left node to check now
                subTreeRoot = subTreeRoot->getNodeR();
            }

        // CompareByY
        } else {

            // Check if is needed to go left or rigth
            if(subTreeRoot->getPosition().y() >= yMim) {

                // Save pointer for right node to check later
                if(subTreeRoot->getNodeR() != NULL  && subTreeRoot->getPosition().y() < yMax)
                    nodesStack.append(subTreeRoot->getNodeR());

                // Save pointer for left node to check now
                subTreeRoot = subTreeRoot->getNodeL();
            } else if(subTreeRoot->getPosition().y() < yMax) {

                // Save pointer for left node to check now
                subTreeRoot = subTreeRoot->getNodeR();
            }
        }

    }

    return nearNode;
}

QList<Position> RRTTree::getPath() {
    QList<Position> path;
    RRTNode* node = _tree.last();

    // Append all point to path
    while( node != NULL ) {
        path.append(node->getPosition());
        node = node->getNodeNN();
    }

    return path;
}
