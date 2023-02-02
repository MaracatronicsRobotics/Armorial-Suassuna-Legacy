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

#include "kalmanfilter.h"

#include <math.h>
#include <Armorial/Libs/magic_enum/include/magic_enum.hpp>

float KalmanFilter::_acc_std_dev           = 0.1;
float KalmanFilter::_meas_err_std_dev      = 0.001;
float KalmanFilter::_p_initial_uncertainty = 0.1;

KalmanFilter::KalmanFilter() {
    // Start P for system and predicted
    magic_enum::enum_for_each<STATES>([this] (STATES state) {
        Math::Matrix _P_shape(6, 6);
        _P.insert({state, _P_shape});
    });

    // Initialize P for system (set diag with initial uncertainty)
    // Note: high initial uncertainty values will result in more 'weight' for the
    // measurement value in the first iteration, but will cost more iterations
    // to converge.
    _P[SYSTEM] = Math::Matrix::diag(6, _p_initial_uncertainty);

    // Start P for system and predicted
    magic_enum::enum_for_each<STATES>([this] (STATES state) {
        Math::Matrix _x_shape(6, 1);
        _x.insert({state, _x_shape});
    });

    // Start F (state transition matrix) and Q (process noise matrix) sizes
    _F.setSize(6, 6);
    _Q.setSize(6, 6);

    // Start R (measurement uncertainty)
    /*
     * [ _meas_err_std_dev²            0         ]
     * [         0            _meas_err_std_dev² ]
     *
     */
    _R = Math::Matrix::diag(2, 1);
    _R *= pow(_meas_err_std_dev, 2);

    // Start H (observation matrix to use only x and y inputs)
    /*
     *  [ 1 0 0 0 0 0 ]
     *  [ 0 0 0 1 0 0 ]
     */
    _H.setSize(2, 6);
    _H.set(0, 0, 1);
    _H.set(1, 3, 1);

    // Initialize the size of the measurement vector (which will be updated with the given positions)
    _z.setSize(2, 1);

    // Cast a initial prediction (update the initial uncertainty matrixes)
    prediction(1.0 / 60.0);
}

Geometry::Vector2D KalmanFilter::getPosition() {
    return Geometry::Vector2D(_x[SYSTEM].get(0, 0), _x[SYSTEM].get(3, 0));
}

Geometry::Vector2D KalmanFilter::getPredictedPosition() {
    return Geometry::Vector2D(_x[PREDICTED].get(0, 0), _x[PREDICTED].get(3, 0));
}

Geometry::Vector2D KalmanFilter::getVelocity() {
    return Geometry::Vector2D(_x[SYSTEM].get(1, 0), _x[SYSTEM].get(4, 0));
}

Geometry::Vector2D KalmanFilter::getPredictedVelocity() {
    return Geometry::Vector2D(_x[PREDICTED].get(1, 0), _x[PREDICTED].get(4, 0));
}

Geometry::Vector2D KalmanFilter::getAcceleration() {
    return Geometry::Vector2D(_x[SYSTEM].get(2, 0), _x[SYSTEM].get(5, 0));
}

Geometry::Vector2D KalmanFilter::getPredictedAcceleration() {
    return Geometry::Vector2D(_x[PREDICTED].get(2, 0), _x[PREDICTED].get(5, 0));
}

void KalmanFilter::updateTransitionAndNoiseFromDT(const float &dt) {
    // Updating transition matrix
    {
        _F = Math::Matrix::diag(6, 1);
        _F.set(0, 1, dt);
        _F.set(0, 2, 0.5f * pow(dt, 2));
        _F.set(1, 2, dt);

        _F.set(3, 4, dt);
        _F.set(3, 5, 0.5f * pow(dt, 2));
        _F.set(4, 5, dt);
    }

    // Updating noise matrix
    {
        _Q.set(0, 0, pow(dt, 4)/4);
        _Q.set(0, 1, pow(dt, 3)/2);
        _Q.set(0, 2, pow(dt, 2)/2);
        _Q.set(3, 3, pow(dt, 4)/4);
        _Q.set(3, 4, pow(dt, 3)/2);
        _Q.set(3, 5, pow(dt, 2)/2);

        _Q.set(1, 0, pow(dt, 3)/2);
        _Q.set(1, 1, pow(dt, 2));
        _Q.set(1, 2, dt);
        _Q.set(4, 3, pow(dt, 3)/2);
        _Q.set(4, 4, pow(dt, 2));
        _Q.set(4, 5, dt);

        _Q.set(2, 0, pow(dt, 2)/2);
        _Q.set(2, 1, dt);
        _Q.set(2, 2, 1);
        _Q.set(5, 3, pow(dt, 2)/2);
        _Q.set(5, 4, dt);
        _Q.set(5, 5, 1);

        _Q *= pow(_acc_std_dev, 2);
    }
}

void KalmanFilter::prediction(const float &dt) {
    // Update _F (state transition) and _Q (process noise) matrixes based on the given DT
    updateTransitionAndNoiseFromDT(dt);

    // Get prediction for state and also calculate the predicted uncertainty
    _x[PREDICTED] = _F * _x[SYSTEM];
    _P[PREDICTED] = _F * _P[SYSTEM] * _F.transposed() + _Q;
}
