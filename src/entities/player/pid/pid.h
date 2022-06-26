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

#ifndef PID_H
#define PID_H

#include <src/utils/timer/timer.h>
#include <tuple>

class PID
{
public:
    PID();
    PID(float P, float I, float D);
    PID(float P, float I, float D, float F);
    PID(std::tuple<float, float, float> pid);
    PID(std::tuple<float, float, float, float> pid);

    // Constant setters
    void setP(float P);
    void setI(float I);
    void setD(float D);
    void setF(float F);
    void setPID(float P, float I, float D);
    void setPID(float P, float I, float D, float F);
    void setPID(std::tuple<float, float, float> pid);
    void setPID(std::tuple<float, float, float> pid, float F);

    // Output setters
    void setMaxIOutput(float maxIOutput);
    void setOutputLimits(float outputLimits);
    void setOutputLimits(float minOutputLimits, float maxOutputLimits);
    void setOutputRampRate(float outputRampRate);
    void setOutputFilter(float outputFilter);

    // Output getters
    float getOutput();
    float getOutput(float actual);
    float getOutput(float actual, float setpoint);

    // Direction and set point
    void setDirection(bool reversed);
    void setSetpoint(float setpoint);
    void setSetpointRange(float setpointRange);

    // Reset
    void reset();

private:
    // Utils functions
    static float clamp(float value, float min, float max);
    static bool bounded(float value, float min, float max);
    void checkSigns();
    void init();

    // Constants
    float _P;
    float _I;
    float _D;
    float _F;

    // Output constants
    float _maxIOutput;
    float _minOutput;
    float _maxOutput;
    float _outputRampRate;

    // Internal control
    bool _firstRun;
    bool _reversed;
    float _maxError;
    float _errorSum;
    float _lastError;
    float _lastActual;
    float _lastOutput;
    float _setpoint;
    float _outputFilter;
    float _setpointRange;
    Timer _timer;
};

#endif // PID_H
