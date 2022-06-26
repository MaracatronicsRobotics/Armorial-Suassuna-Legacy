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

PID::PID() {
    init();
}

PID::PID(float P, float I, float D) {
    init();
    _P = P;
    _I = I;
    _D = D;

    _timer.start();
}

PID::PID(float P, float I, float D, float F) {
    init();
    _P = P;
    _I = I;
    _D = D;
    _F = F;

    _timer.start();
}

PID::PID(std::tuple<float, float, float> pid) : PID(std::get<0>(pid), std::get<1>(pid), std::get<2>(pid)) {}

PID::PID(std::tuple<float, float, float, float> pid) : PID(std::get<0>(pid), std::get<1>(pid), std::get<2>(pid), std::get<3>(pid)) {}

void PID::init() {
    _P = 0;
    _I = 0;
    _D = 0;
    _F = 0;

    setMaxIOutput(0.0f);
    setOutputLimits(0.0f);
    setOutputFilter(0.0f);
    setOutputRampRate(0.0f);
    _minOutput = 0.0f;
    _maxOutput = 0.0f;
    _outputRampRate = 0.0f;
    _maxError = 0.0f;
    _errorSum = 0.0f;
    _lastError = 0.0f;
    _lastActual = 0.0f;
    setSetpoint(0.0f);
    setSetpointRange(0.0f);
    setDirection(false);
    _firstRun = true;
}

void PID::setP(float P) {
    _P = P;
    checkSigns();
}

void PID::setI(float I) {
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

void PID::setPID(float P, float I, float D, float F) {
    _P = P;
    _I = I;
    _D = D;
    _F = F;
    checkSigns();
}

void PID::setPID(std::tuple<float, float, float> pid) {
    this->setPID(std::get<0>(pid), std::get<1>(pid), std::get<2>(pid));
}

void PID::setPID(std::tuple<float, float, float> pid, float F) {
    this->setPID(std::get<0>(pid), std::get<1>(pid), std::get<2>(pid), F);
}

void PID::setMaxIOutput(float maxIOutput) {
    _maxIOutput = maxIOutput;
    if (_I != 0.0f) {
        _maxError = _maxIOutput / _I;
    }
}

void PID::setOutputLimits(float outputLimits) {
    setOutputLimits(-outputLimits, outputLimits);
}

void PID::setOutputLimits(float minOutputLimits, float maxOutputLimits) {
    if (maxOutputLimits < minOutputLimits) return;

    _maxOutput = maxOutputLimits;
    _minOutput = minOutputLimits;

    if (_maxIOutput == 0 || _maxIOutput > (maxOutputLimits - minOutputLimits)) {
        setMaxIOutput(maxOutputLimits - minOutputLimits);
    }
}

void PID::setOutputRampRate(float outputRampRate) {
    _outputRampRate = outputRampRate;
}

void PID::setOutputFilter(float outputFilter) {
    if (outputFilter == 0 || bounded(outputFilter, 0, 1)) {
        _outputFilter = outputFilter;
    }
}

void PID::setDirection(bool reversed) {
    _reversed = reversed;
}

void PID::setSetpoint(float setpoint) {
    _setpoint = setpoint;
}

void PID::setSetpointRange(float setpointRange) {
    _setpointRange = setpointRange;
}

float PID::getOutput() {
    return getOutput(_lastActual, _setpoint);
}

float PID::getOutput(float actual) {
    return getOutput(actual, _setpoint);
}

float PID::getOutput(float actual, float setpoint) {
    _timer.stop();

    setSetpoint(setpoint);
    if (_setpointRange != 0.0f) {
        setpoint = clamp(setpoint, actual - _setpointRange, actual + _setpointRange);
    }

    float error = setpoint - actual;

    float Foutput = _F * setpoint;
    float Poutput = _P * error;
    _errorSum += error * _timer.getSeconds();
    float Ioutput = _I * _errorSum;
    if (_maxIOutput != 0.0f) {
        Ioutput = clamp(Ioutput, -_maxIOutput, _maxIOutput);
    }

    if (_firstRun) {
        _lastError = error;
        _lastOutput = Poutput;
        _firstRun = false;
    }
    float derivative = (error - _lastError) / _timer.getSeconds();
    float Doutput = _D * derivative;
    float output = Foutput + Poutput + Ioutput + Doutput;

    if (_minOutput != _maxOutput && !bounded(output, _minOutput, _maxOutput)) {
        _errorSum = error;
    } else if (_outputRampRate != 0.0f && !bounded(output, _lastOutput - _outputRampRate, _lastOutput + _outputRampRate)) {
        _errorSum = error;
    } else if (_maxIOutput != 0.0f) {
        _errorSum = clamp(_errorSum + error, -_maxError, _maxError);
    } else {
        _errorSum += error;
    }

    if (_outputRampRate != 0.0f) {
        output = clamp(output, _lastOutput - _outputRampRate, _lastOutput + _outputRampRate);
    }
    if (_minOutput != _maxOutput) {
        output = clamp(output, _minOutput, _maxOutput);
    }
    if (_outputFilter != 0.0f) {
        output = _lastOutput * _outputFilter + output * (1 - _outputFilter);
    }

    _lastError = error;
    _lastOutput = output;

    _timer.start();

    return output;
}

void PID::reset() {
    _firstRun = true;
    _errorSum = 0;
    _timer.start();
}

float PID::clamp(float value, float min, float max) {
    if (value > max) {
        return max;
    }
    if (value < min) {
        return min;
    }
    return value;
}

bool PID::bounded(float value, float min, float max) {
    return ((min < value) && (value < max));
}

void PID::checkSigns() {
    if (_reversed) {
        if (_P > 0) _P *= -1;
        if (_I > 0) _I *= -1;
        if (_D > 0) _D *= -1;
        if (_F > 0) _F *= -1;
    } else {
        if (_P < 0) _P *= -1;
        if (_I < 0) _I *= -1;
        if (_D < 0) _D *= -1;
        if (_F < 0) _F *= -1;
    }
}
