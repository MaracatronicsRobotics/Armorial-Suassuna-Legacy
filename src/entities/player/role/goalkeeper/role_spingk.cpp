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

    bool _spinOrientation = (Constants::teamPlaySide() == Common::Enums::Side::SIDE_LEFT); // True == CCW
}

void Role_spinGK::run() {
    Geometry::Vector2D goalCenter = getWorldMap()->getField().ourGoalCenter();
    bool inOurGoal = (getWorldMap()->getField().ourField().contains(getWorldMap()->getBall().getPosition()));
    switch (_currState) {
    case(MOVETO):{
        _behavior_moveTo->enableRotation(false);
        _behavior_moveTo->enableSpin(false);
        _behavior_moveTo->setForcebleMotion(false);
        _behavior_moveTo->enableAntiStuck(true);
        _behavior_moveTo->setPosition(goalCenter);
        setBehavior(BEHAVIOR_MOVETO);

        if(player()->getPosition().dist(goalCenter) < 0.1){
            _currState = IDLE;
        }
        break;
    }
    case(SPIN):{
        bool orientation; //CW

        if(getWorldMap()->getBall().getPosition().y() > 0){
            orientation = (true == _spinOrientation);
        }else{
            orientation = (false == _spinOrientation);
        }

        _behavior_moveTo->setSpinOrientation(orientation);
        _behavior_moveTo->enableSpin(true);
        setBehavior(BEHAVIOR_MOVETO );

        if(!inOurGoal){
            _currState = IDLE;
        }
    }
    case(IDLE):{
        //std::cout << "peitin do teu pai\n";
        if(inOurGoal){
            _currState = SPIN;
        }else{
            _behavior_moveTo->setLeftWheelPower(0);
            _behavior_moveTo->setRightWheelPower(0);
            _behavior_moveTo->enableRotation(false);
            _behavior_moveTo->enableSpin(false);
            _behavior_moveTo->enableAntiStuck(false);
            _behavior_moveTo->setForcebleMotion(true);
            setBehavior(BEHAVIOR_MOVETO);
        }
    }
    }
}
