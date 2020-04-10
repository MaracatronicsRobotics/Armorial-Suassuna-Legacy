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

#include <include/timer.h>
#include <iostream>

class PID
{
public:
    PID();
    PID(double kp, double ki, double kd, double max, double min);
    ~PID();
    void setPIDParameters(double kp, double kd, double ki, double max, double min);
    double calculate(double desired, double actual);
private:
    Timer *_timer; // timer to get _dt
    double _kp, _ki, _kd, _dt; // pid terms
    double _max, _min; // max and min values
    double _integral, _pre_error; // save last error and integral sum
};

#endif // PID_H
