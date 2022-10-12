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

#include "anglepid.h"

#include <algorithm>

float AnglePID::getOutput(const Geometry::Angle& target_angle, const Geometry::Angle& current_angle) {
    // Calculate error; note that crossing the 'border' between two angles
    double dir = current_angle.rotateDirection(target_angle) ? 1.0 : -1.0;
    double error = dir * current_angle.shortestAngleDiff(target_angle);

    // Proportional term
    double Pout = _P * error;

    // Integral term
    _integral += error * _dt;
    double Iout = _I * _integral;

    // Derivative term
    double derivative = (error - _previous_error) / _dt;
    double Dout = _D * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    output = std::clamp(output, _min, _max);
    // Save error to previous error
    _previous_error = error;

    return output;
}
