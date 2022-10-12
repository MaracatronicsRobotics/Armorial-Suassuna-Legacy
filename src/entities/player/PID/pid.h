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

/*!
 * \brief The PID class
 */
class PID
{
public:
    /*!
     * \brief PID
     */
    PID();

    /*!
     * \brief PID
     * \param P The proportional param.
     * \param I The integral param.
     * \param D The derivative param.
     * \param F The feed forward gain.
     */
    PID(float P, float I, float D, float F = 1.0f);
    float _maxIOutput;
    float _maxError;
    float _errorSum;

    /*!
     * \brief PID param setters
     */
    void setP(float P);
    void setI(float I);
    void setD(float D);
    void setF(float F);
    void setPID(float P, float I, float D);

    /*!
     * \brief PID auxiliar setters
     */
    void setMaxIOutput(float maxIOutput);
    void setOutputLimits(float output);
    void setOutputLimits(float min, float max);
    void setDirection(bool reversed);
    void setSetpoint(float setpoint);
    void setOutputRampRate(float rate);
    void setSetpointRange(float range);
    void setOutputFilter(float filter);

    /*!
     * \brief reset
     */
    void reset();

    /*!
     * \brief Getters for the desired setpoint.
     */
    float getOutput();
    float getOutput(float actual);
    float getOutput(float actual, float setpoint);

protected:
    void checkSigns();
    void init();

private:
    float _P;
    float _I;
    float _D;
    float _F;


    float _maxOutput;
    float _minOutput;

    float _setpoint;

    float _lastActual;

    bool _firstRun;
    bool _reversed;

    float _outputRampRate;
    float _lastOutput;

    float _outputFilter;

    float _setpointRange;
};

#endif // PID_H
