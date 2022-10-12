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

#define TICK_RATE 1.0/60.0

#include <algorithm>

PID::PID() {
    init();
}

PID::PID(float P, float I, float D, float F) {
    init();
    setP(P);
    setI(I);
    setD(D);
    setF(F);
}

void PID::setP(float P) {
    _P = P;
    checkSigns();
}

void PID::setI(float I) {
    if (_I != 0) {
        _errorSum = _errorSum * _I / I;
    }
    if (_maxIOutput != 0) {
        _maxError = _maxIOutput / I;
    }
    _I = I;
    checkSigns();
}

void PID::setD(float D) {
    _D = D;
    checkSigns();
}

void PID::setF(float F) {
    _F = F;
    checkSigns();
}

void PID::setPID(float P, float I, float D) {
    _P = P;
    _I = I;
    _D = D;
    checkSigns();
}

void PID::setMaxIOutput(float maxIOutput) {
    _maxIOutput = maxIOutput;
    if (_I != 0) {
        _maxError = _maxIOutput / _I;
    }
}

void PID::setOutputLimits(float output) {
    setOutputLimits(-output, output);
}

void PID::setOutputLimits(float min, float max) {
    if (max < min) return;
    _maxOutput = max;
    _minOutput = min;

    // Ensure the bounds of the I term are within the bounds of the allowable output swing
    if (_maxIOutput == 0 || _maxIOutput > (max - min)) {
        setMaxIOutput(max - min);
    }
}

void PID::setDirection(bool reversed) {
    _reversed = reversed;
}

void PID::setSetpoint(float setpoint) {
    _setpoint = setpoint;
}

void PID::setOutputRampRate(float rate) {
    _outputRampRate = rate;
}

void PID::setSetpointRange(float range) {
    _setpointRange = range;
}

void PID::setOutputFilter(float filter) {
    if (filter == 0 || ((0 < filter) && (filter < 1))) {
        _outputFilter = filter;
    }
}

void PID::reset() {
    _firstRun = true;
    _errorSum = 0;
}

float PID::getOutput() {
    return getOutput(_lastActual, _setpoint);
}

float PID::getOutput(float actual) {
    return getOutput(actual, _setpoint);
}

float PID::getOutput(float actual, float setpoint) {
    float output;
    float Poutput;
    float Ioutput;
    float Doutput;
    float Foutput;

    _setpoint = setpoint;

    // Ramp the setpoint used for calculations if user has opted to do so
    if (_setpointRange != 0) {
        setpoint = std::clamp(setpoint, actual - _setpointRange, actual + _setpointRange);
    }

    // Do the simple parts of the calculations
    float error = setpoint - actual;

    // Calculate F output. Notice, this->depends only on the setpoint, and not the error.
    Foutput = _F * setpoint;

    // Calculate P term
    Poutput = _P * error;

    // If this->is our first time running this-> we don't actually _have_ a previous input or output.
    // For sensor, sanely assume it was exactly where it is now.
    // For last output, we can assume it's the current time-independent outputs.
    if (_firstRun) {
        _lastActual = actual;
        _lastOutput = Poutput + Foutput;
        _firstRun = false;
    }

    // Calculate D Term
    // Note, this->is negative. this->actually "slows" the system if it's doing
    // the correct thing, and small values helps prevent output spikes and overshoot

    Doutput = -_D * (actual - _lastActual) * TICK_RATE;
    _lastActual = actual;

    // The Iterm is more complex. There's several things to factor in to make it easier to deal with.
    // 1. maxIoutput restricts the amount of output contributed by the Iterm.
    // 2. prevent windup by not increasing errorSum if we're already running against our max Ioutput
    // 3. prevent windup by not increasing errorSum if output is output=maxOutput
    Ioutput = _I * _errorSum / TICK_RATE;
    if (_maxIOutput != 0) {
        Ioutput = std::clamp(Ioutput, -_maxIOutput, _maxIOutput);
    }

    // And, finally, we can just add the terms up
    output = Foutput + Poutput + Ioutput + Doutput;

    // Figure out what we're doing with the error.
    if (_minOutput != _maxOutput && !((_minOutput < output) && (output < _maxOutput))) {
        _errorSum = error;
        // reset the error sum to a sane level
        // Setting to current error ensures a smooth transition when the P term
        // decreases enough for the I term to start acting upon the controller
        // From that point the I term will build up as would be expected
    } else if (_outputRampRate != 0 && !(((_lastOutput - _outputRampRate) < output) && (output < (_lastOutput + _outputRampRate)))) {
        _errorSum = error;
    } else if (_maxIOutput != 0) {
        _errorSum = std::clamp(_errorSum + error, -_maxError, _maxError);
        // In addition to output limiting directly, we also want to prevent I term
        // buildup, so restrict the error directly
    } else {
        _errorSum += error;
    }

    // Restrict output to our specified output and ramp limits
    if (_outputRampRate != 0) {
        output = std::clamp(output, _lastOutput - _outputRampRate, _lastOutput + _outputRampRate);
    }
    if (_minOutput != _maxOutput) {
        output = std::clamp(output, _minOutput, _maxOutput);
    }
    if (_outputFilter != 0) {
        output = _lastOutput * _outputFilter + output * (1 - _outputFilter);
    }

    _lastOutput = output;

    return output;
}

void PID::checkSigns() {
    if (_reversed) {  // all values should be below zero
        if (_P > 0) _P *= -1;
        if (_I > 0) _I *= -1;
        if (_D > 0) _D *= -1;
        if (_F > 0) _F *= -1;
    } else {  // all values should be above zero
        if (_P < 0) _P *= -1;
        if (_I < 0) _I *= -1;
        if (_D < 0) _D *= -1;
        if (_F < 0) _F *= -1;
    }
}

void PID::init() {
    _P = 0;
    _I = 0;
    _D = 0;
    _F = 0;

    _maxIOutput = 0;
    _maxError = 0;
    _errorSum = 0;
    _maxOutput = 0;
    _minOutput = 0;
    _setpoint = 0;
    _lastActual = 0;
    _firstRun = true;
    _reversed = false;
    _outputRampRate = 0;
    _lastOutput = 0;
    _outputFilter = 0;
    _setpointRange = 0;
}
