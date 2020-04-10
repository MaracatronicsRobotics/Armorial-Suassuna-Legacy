/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "pid.h"
#include <stdio.h>
#include <math.h>

PID::PID(){
    _kp = 0.0;
    _kd = 0.0;
    _ki = 0.0;

    _max = 0.0;
    _min = 0.0;

    _pre_error = 0.0;
    _integral = 0.0;

    _timer = new Timer();
    _timer->start();
}

PID::PID(double kp, double ki, double kd, double max, double min){
    _kp = kp;
    _ki = ki;
    _kd = kd;

    _max = max;
    _min = min;

    _pre_error = 0.0;
    _integral = 0.0;

    _timer = new Timer();
    _timer->start();
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

    _pre_error = 0.0;
    _integral = 0.0;

    _timer->start();
}

double PID::calculate(double desired, double actual){
    if(fabs(desired) >= 10.0){
        desired = 0.0;
    }
    // get time interval
    _timer->stop();

    _dt = _timer->timeusec();
    _dt = _dt / 1000.0;

    _timer->start();

    if(_dt == 0.0) return 0.0; // if _dt == 0

    // error
    double error = desired - actual;

    if(fabs(error) <= 0.1){
        _integral = 0.0;
    }

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

    // Save error to previous error
    _pre_error = error;

    return result;
}
