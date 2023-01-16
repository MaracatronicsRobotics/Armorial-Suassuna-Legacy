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


#ifndef LOSSFILTER_H
#define LOSSFILTER_H

#include <QReadWriteLock>

#include <Armorial/Utils/Timer/Timer.h>

class LossFilter
{
public:
    /*!
     * \brief LossFilter class constructor, which mark the filter as unitialized.
     */
    LossFilter();

    /*!
     * \brief Start the loss filter, reseting the timer.
     */
    void startLoss();

    /*!
     * \return True if the filter is initialized and False otherwise.
     */
    bool isInitialized();

    /*!
     * \return True if the loss time has been reached and False otherwise.
     */
    bool checkLoss();

    /*!
     * \brief Get the maximum loss time to maintain a field object.
     * \return A float value containing the loss time.
     */
    float getLossTime();

    /*!
     * \brief Static method that sets the loss time.
     * \param lossTime The new loss time value.
     */
    static void setLossTime(float lossTime);

    /*!
     * \brief Setups the first iteration of the loss filter, marking the object as loss.
     */
    void setFirstIt();

private:
    // Timer
    Utils::Timer _timer;

    // Params
    bool _isInitialized;
    bool _firstIt;

    // Filter time
    static float _filterTime;

    // Mutex to change values
    static QReadWriteLock _filterMutex;
};

#endif // LOSSFILTER_H
