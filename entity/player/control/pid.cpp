#include "pid.h"
#include <stdio.h>
#include <math.h>

PID::PID(){
    _kp = 0;
    _kd = 0;
    _ki = 0;

    _max = 0;
    _min = 0;

    _pre_error = 0;
    _integral = 0;

    _timer = new MRCTimer(100);
    _timer->update();
}

PID::PID(double kp, double kd, double ki, double max, double min){
    _kp = kp;
    _kd = kd;
    _ki = ki;

    _max = max;
    _min = min;

    _pre_error = 0;
    _integral = 0;

    _timer = new MRCTimer(100);
    _timer->update();
}

PID::~PID(){
    delete _timer;
}

void PID::setPIDParameters(double kp, double kd, double ki, double max, double min){
    _kp = kp;
    _kd = kd;
    _ki = ki;

    _max = max;
    _min = min;

    _pre_error = 0;
    _integral = 0;

    _timer->update();
}

double PID::calculate(double desired, double actual){
    // get time interval
    _dt = _timer->getTimeInMilliSeconds();
    _timer->update();

    // error
    double error = desired - actual;

    // Proportional term
    double P = _kp * error;

    // Integral term
    _integral += error * _dt;
    double I = _ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / _dt;
    double D = _kd * derivative;

    // Total PID
    double result = P + I + D;

    // Restrict
    if(result > _max){
        result = _max;
    }else if(result < _min){
        result = _min;
    }

    return result;
}
