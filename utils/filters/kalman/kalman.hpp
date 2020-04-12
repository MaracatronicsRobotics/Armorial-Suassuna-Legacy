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

#ifndef KALMANFILTER2D_H

#define KALMANFILTER2D_H

#include <QString>
#include <GEARSystem/gearsystem.hh>
#include <include/timer.h>
#include "kalmanstate.h"
#include "matrix.h"

#include <const/constants.h>

class KalmanFilter2D {

private:

    Timer _timer;

    // Initial state

    bool _has1stPosition, _has2ndPosition;

    // State and covariance matrices

    KalmanState _X, _Y;

    Matrix _Px, _Py;

    // Model

    Matrix _A;
    Matrix _H;
    Matrix _Q;
    Matrix _R;

    // Model config

    static constexpr float _p = 1;
    static constexpr float _sigma_a = 0.01; // affects Q
    static constexpr float _r = 0.000283; // affects R

    // Private methods

    void updateMatrices(const float T);
    bool enabled;

public:

    KalmanFilter2D();

    QString name();

    void iterate(const Position &pos);

    void predict();

    Position getPosition() const;

    Velocity getVelocity()  const;

    Velocity getAcceleration() const;

    void setEnabled(bool _enable);

    bool getEnabled();

};
#endif
