#include "ballprediction.h"

QString BallPrediction::name(){
    return "Ball Prediction";
}

BallPrediction::BallPrediction() : Entity(ENT_BALLPREDICTION)
{
    arq = fopen("test.csv", "w+");
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
        _dtBall.push_back(_timer.timensec());

        // Lock mutex to get position and velocity
        _ballMutex.lock();
        Position ballPos = _team->loc()->ball();
        Velocity ballVel = _team->loc()->ballVelocity();
        _ballMutex.unlock();

        // Save in list
        _posBall.push_back(ballPos);
        _velBall.push_back(ballVel);

        fprintf(arq, "%.12lf,%.12lf,%.12lf\n", ballVel.abs(), (_timer.timensec() - _dtBall.first())/1e6, WR::Utils::distance(_posBall.first(), ballPos));
    }else{
        int sz = _posBall.size();
/*
        for(int x = 1; x < sz; x+=10){
            Position pos = _posBall[x];
            double time = _dtBall[x];
            for(int y = x + 1; y <= x + 1 + 10; y++){ // do with the next 10 data
                if(abs((_dtBall[y] - _dtBall[x])/1e6) >= 500) break;
                fprintf(arq, "%.6lf,%.6lf,%.6lf\n", _velBall[y].abs(), abs((_dtBall[y] - _dtBall[x])/1e6), WR::Utils::distance(pos, _posBall[y]));
            }
        }
*/
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
