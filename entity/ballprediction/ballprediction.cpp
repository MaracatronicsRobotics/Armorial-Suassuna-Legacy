#include "ballprediction.h"

QString BallPrediction::name(){
    return "Ball Prediction";
}

BallPrediction::BallPrediction() : Entity(ENT_BALLPREDICTION)
{

}

void BallPrediction::setTeam(MRCTeam *team){
    _team = team;
}

void BallPrediction::setMinDataSize(int qt){
    _minDataSize = qt;
}

void BallPrediction::setMinVelocity(double minVelocity){
    _minVelocity = minVelocity;
}

void BallPrediction::initialization(){
    // Clear vars
    _posBall.clear();
    _velBall.clear();
    _dtBall.clear();
    _minVelocity = 0.3;

    // Start Timer
    _timer.start();

    std::cout << "[BALLPREDICTION] Thread started.\n";
}

void BallPrediction::loop(){
    // precisa saber se ngm tem posse (chute)
    // bola com velocidade suficiente (chute)

    if(/*nobodyHasPoss() && */hasSufficientVelocity()){
        // Stop timer and get _dt
        _timer.stop();
        _dtBall.push_back(_timer.timemsec());

        // Lock mutex to get position and velocity
        _ballMutex.lock();
        Position ballPos = _team->loc()->ball();
        Velocity ballVel = _team->loc()->ballVelocity();
        _ballMutex.unlock();

        // Save in list
        _posBall.push_back(ballPos);
        _velBall.push_back(ballVel);

        // Start timer to get next _dt
        _timer.start();
    }else{
        if(_posBall.size() > _minDataSize && _velBall.size() > _minDataSize && _dtBall.size() > _minDataSize){
            std::cout << "[BALLPREDICTION] Trained.\n";
            // treino
        }

        _posBall.clear();
        _velBall.clear();
        _dtBall.clear();
    }
}

void BallPrediction::finalization(){
    std::cout << "[BALLPREDICTION] Thread ended.\n";
}

bool BallPrediction::nobodyHasPoss(){
    return(_team->hasBallPossession() || _team->opTeam()->hasBallPossession());
}

bool BallPrediction::hasSufficientVelocity(){
    _ballMutex.lock();
    Velocity ballVell = _team->loc()->ballVelocity();
    _ballMutex.unlock();

    return(ballVell.abs() >= _minVelocity);
}
