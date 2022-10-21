#include "role_spingk.h"

Role_spinGK::Role_spinGK()
{

}

void Role_spinGK::configure() {
    // Starting behaviors
    _behavior_default = new Behavior_Default();
    _behavior_moveTo = new Behavior_MoveTo();

    // Adding behaviors to behaviors list
    addBehavior(BEHAVIOR_DEFAULT, _behavior_default);
    addBehavior(BEHAVIOR_MOVETO, _behavior_moveTo);

    _currState = MOVETO;

    _spinOrientation = (Constants::teamPlaySide() == Common::Enums::Side::SIDE_LEFT); // True == CCW
}

void Role_spinGK::run() {
    int factor = 1;
    if (Constants::teamPlaySide() == Common::Enums::Side::SIDE_RIGHT) {
        factor = -1;
    }
    Geometry::Vector2D gkPosition(getWorldMap()->getField().ourGoalCenter().x() + factor * 0.06f, 0.0f);
    Geometry::Vector2D lookingPosition(gkPosition.x(), 2.0f);

    _behavior_moveTo->enableRotation(false);

    switch (_currState) {
    case(MOVETO):{
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->enableAntiStuck(true);
        _behavior_moveTo->setPosition(gkPosition);
        setBehavior(BEHAVIOR_MOVETO);

        if(player()->getPosition().dist(gkPosition) < 0.1){
            _currState = ROTATE;
        }
        break;
    }
    case(SPIN):{
        if(player()->isClockwiseSpin()){
            _behavior_moveTo->setSpinOrientation(true);
        } else {
            _behavior_moveTo->setSpinOrientation(false);
        }

        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->setSpinSpeed(255);
        _behavior_moveTo->enableSpin(true);
        _behavior_moveTo->enableAntiStuck(false);
        setBehavior(BEHAVIOR_MOVETO );

        if(fabs((lookingPosition - player()->getPosition()).angle() - player()->getOrientation().value()) > 0.3f){
            _currState = ROTATE;
        }
        if (!getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition())) {
            _currState = MOVETO;
        }
        break;
    }
    case(ROTATE):{
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->setSpinSpeed(5);
        _behavior_moveTo->enableSpin(true);
        _behavior_moveTo->setSpinOrientation(true);
        _behavior_moveTo->enableAntiStuck(false);
        setBehavior(BEHAVIOR_MOVETO);

        if (fabs((lookingPosition - player()->getPosition()).angle() - player()->getOrientation().value()) < 0.07f){
            _currState = CHASE;
        }
        if (!getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition())) {
            _currState = MOVETO;
        }
        break;
    }
    case(CHASE):{
        _behavior_moveTo->setForcebleMotion(true);
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->enableAntiStuck(false);

        Geometry::LineSegment lineDefense(Geometry::Vector2D(gkPosition.x(), 0.25f), Geometry::Vector2D(gkPosition.x(), -0.25f));
        Geometry::Vector2D ballVelocity = getWorldMap()->getBall().getVelocity();
        Geometry::LineSegment ballLine(getWorldMap()->getBall().getPosition(), ballVelocity.stretchToLength(2));

        std::vector<Geometry::Vector2D> interceptList;
        Geometry::Vector2D interceptPosition;
        if (lineDefense.doesIntersect(ballLine)) {
            interceptList = lineDefense.intersects(ballLine);
            interceptPosition = interceptList[0];
            interceptList.clear();
        } else {
            Geometry::LineSegment bigLineDefense(Geometry::Vector2D(gkPosition.x(), 10.0f), Geometry::Vector2D(gkPosition.x(), -10.0f));
            if (bigLineDefense.intersects(ballLine).size() > 0) {
                if (bigLineDefense.intersects(ballLine)[0].y() > 0.25f) {
                    interceptPosition = Geometry::Vector2D(gkPosition.x(), 0.25);
                } else {
                    interceptPosition = Geometry::Vector2D(gkPosition.x(), -0.25);
                }
            } else {
                interceptPosition = player()->getPosition();
            }
        }
        float relation = fabs(gkPosition.x() / 0.5 * (gkPosition.x() +
                         fabs(gkPosition.x() - getWorldMap()->getBall().getPosition().x())));
        interceptPosition = Geometry::Vector2D(interceptPosition.x(), relation *interceptPosition.y());
        if(interceptPosition.y() > player()->getPosition().y()){
            _behavior_moveTo->setLeftWheelPower(255);
            _behavior_moveTo->setRightWheelPower(255);
        } else {
            _behavior_moveTo->setLeftWheelPower(-255);
            _behavior_moveTo->setRightWheelPower(-255);
        }

        if(player()->getPosition().dist(interceptPosition) < 0.05){
            _behavior_moveTo->setLeftWheelPower(0);
            _behavior_moveTo->setRightWheelPower(0);
        }
        setBehavior(BEHAVIOR_MOVETO);

        if (!getWorldMap()->getField().ourPenaltyArea().contains(player()->getPosition())) {
            _currState = MOVETO;
        } else {
            if (player()->getPosition().dist(getWorldMap()->getBall().getPosition()) < 0.1) {
                _currState = SPIN;
            }
        }
    }
        break;
    case(WAIT):{
        _behavior_moveTo->setForcebleMotion(true);
        _behavior_moveTo->setPosition(player()->getPosition());
        _behavior_moveTo->enableSpin(false);
        setBehavior(BEHAVIOR_MOVETO);
    }
    }
}
