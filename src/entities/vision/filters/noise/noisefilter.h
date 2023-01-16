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

#ifndef NOISEFILTER_H
#define NOISEFILTER_H

#include <QReadWriteLock>

#include <Armorial/Utils/Timer/Timer.h>

class NoiseFilter
{
public:
    /*!
     * \brief NoiseFilter default constructor, which mark the filter as unitialized.
     */
    NoiseFilter();

    /*!
     * \brief Start the noise filter, reseting the timer.
     */
    void startNoise();

    /*!
     * \return True if the filter is initialized and False otherwise.
     */
    bool isInitialized();

    /*!
     * \return True if the noise time has been reached and False otherwise.
     */
    bool checkNoise();

    /*!
     * \brief Get the maximum noise time to discard a new field object.
     * \return A float value containing the noise time.
     */
    float getNoiseTime();

    /*!
     * \brief Static method that sets the noise time.
     * \param noiseTime The new noise time value.
     */
    static void setNoiseTime(float noiseTime);

private:
    // Timer
    Utils::Timer _timer;

    // Params
    bool _isInitialized;
    static float _filterTime;

    // Mutex to change values
    static QReadWriteLock _filterMutex;
};

#endif // NOISEFILTER_H
