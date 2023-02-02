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


#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <map>

#include <Armorial/Common/Types/Traits/Traits.h>
#include <Armorial/Geometry/Vector2D/Vector2D.h>
#include <Armorial/Math/Matrix/Matrix.h>
#include <Armorial/Utils/Timer/Timer.h>

/*!
 * \brief The KalmanFilter class
 */
class KalmanFilter
{
    typedef enum {
        PREDICTED,
        SYSTEM
    } STATES;

public:
    /*!
     * \brief KalmanFilter constructor, which initializes all the matrixes with the correct
     * shape and also make a initial prediction of the system to enable the startup.
     */
    KalmanFilter();

    /*!
     * \tparam T The given type of the input, which needs to have coordinates.
     * \brief Update the kalman state and uncertainty matrixes with the given input
     * \param position The given input.
     */
    template<typename T>
    std::enable_if_t<Common::Types::has_coordinates_v<T>, void> update(const T& position) {
        // Set current measurement
        _z.set(0, 0, position.x());
        _z.set(1, 0, position.y());

        // Compute kalman gain
        _K = (_P[PREDICTED] * _H.transposed()) * ((_H * _P[PREDICTED] * _H.transposed() + _R).inverse().value());

        // Estimate the current state of the system
        _x[SYSTEM] = _x[PREDICTED] + _K * (_z - (_H * _x[PREDICTED]));

        // Compute the uncertainty for the current state of the system
        _P[SYSTEM] = (Math::Matrix::identity(_K.lines()) - (_K * _H)) * _P[PREDICTED] * (Math::Matrix::identity(_K.lines()) - (_K * _H)).transposed() + (_K * _R * _K.transposed());

        // Get prediction (time is passed in seconds)
        prediction(_timer.getSeconds());

        // Start timer for the next prediction
        _timer.start();
    }

    /*!
     * \return The position in the current state.
     */
    [[nodiscard]] Geometry::Vector2D getPosition();

    /*!
     * \return The position in the predicted state.
     */
    [[nodiscard]] Geometry::Vector2D getPredictedPosition();

    /*!
     * \return The velocity in the current state.
     */
    [[nodiscard]] Geometry::Vector2D getVelocity();

    /*!
     * \return The velocity in the predicted state.
     */
    [[nodiscard]] Geometry::Vector2D getPredictedVelocity();

    /*!
     * \return The acceleration in the current state.
     */
    [[nodiscard]] Geometry::Vector2D getAcceleration();

    /*!
     * \return The acceleration in the predicted state.
     */
    [[nodiscard]] Geometry::Vector2D getPredictedAcceleration();

private:
    /*!
     * \brief Update the _F and _Q matrixes using the given time variation (dt).
     * \param dt The given time variation since the last measurement (this is obtained by the
     * timer of the KalmanFilter class).
     */
    void updateTransitionAndNoiseFromDT(const float &dt);

    /*!
     * \brief Computes the prediction state and its uncertainty using the given time variation (dt).
     * \param dt The given time variation since the last measurement (this is obtained by the
     * timer of the KalmanFilter class).
     */
    void prediction(const float &dt);

    // System constraints
    static float _acc_std_dev;           // Affects Q
    static float _meas_err_std_dev;      // Affects R
    static float _p_initial_uncertainty; // Affects P[SYSTEM] at first iteration

    // Kalman matrixes
    std::map<STATES, Math::Matrix> _x;         // Map for predicted state and system state
    std::map<STATES, Math::Matrix> _P;         // Map for uncertainty of system and uncertainty of prediction
    Math::Matrix _Q;                           // Process noise matrix     (need recalc for delta_T)   (affected by _acc_std_dev)
    Math::Matrix _F;                           // State transition matrix  (need recalc for delta_T)
    Math::Matrix _H;                           // Observation matrix
    Math::Matrix _R;                           // Measurement uncertainty
    Math::Matrix _K;                           // Kalman gain
    Math::Matrix _z;                           // Measurement vector       (updated by the given position)

    // Timer to estimate dt
    Utils::Timer _timer;
};

#endif // KALMANFILTER_H
