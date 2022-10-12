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

#ifndef ANGLEPID_H
#define ANGLEPID_H

#include <Armorial/Geometry/Angle/Angle.h>

/*!
 * \brief The AnglePID class
 */
class AnglePID
{
public:
    /*!
     * \brief AnglePID
     */
    AnglePID(double P, double I, double D, double max_ang_vel, double dt) : _P{P}, _I{I}, _D{D}, _min{-max_ang_vel}, _max{max_ang_vel}, _previous_error{0.0}, _integral{0.0}, _dt{dt} {}

    /*!
     * \brief getOutput
     * \param target_angle
     * \param current_angle
     * \return
     */
    float getOutput(const Geometry::Angle& target_angle, const Geometry::Angle& current_angle);

private:
    double _P;
    double _I;
    double _D;
    double _min;
    double _max;
    double _previous_error;
    double _integral;
    double _dt;
};

#endif // ANGLEPID_H
