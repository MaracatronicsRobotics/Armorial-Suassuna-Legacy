#include "role_default.h"

Role_Default::Role_Default() {

}

QString Role_Default::name() {
    return "Role_Default";
}

void Role_Default::initializeBehaviours() {
    addBehaviour(BEHAVIOR_GOTOLOOKTO, _bhv_gotolookto = new Behaviour_GoToLookTo());
}

void Role_Default::configure() {

}

void Role_Default::run() {
    Position ballPosition = getWorldMap()->getBall().ballposition();
    Position theirGoal = getWorldMap()->getLocations()->theirGoal();

    Position behindBall = Utils::threePoints(ballPosition, theirGoal, 0.15, -M_PI, false);

    _bhv_gotolookto->setPositionToGo(behindBall);
    _bhv_gotolookto->setReferencePosition(ballPosition);
    _bhv_gotolookto->setPositionToLook(theirGoal);

    setBehaviour(BEHAVIOR_GOTOLOOKTO);
}
