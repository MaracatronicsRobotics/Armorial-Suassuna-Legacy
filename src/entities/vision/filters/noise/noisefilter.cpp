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

#include "noisefilter.h"

float NoiseFilter::_filterTime = 300;
QReadWriteLock NoiseFilter::_filterMutex = QReadWriteLock();

NoiseFilter::NoiseFilter() {
    _isInitialized = false;
}

void NoiseFilter::startNoise() {
    _isInitialized = true;
    _timer.start();
}

bool NoiseFilter::isInitialized() {
    return _isInitialized;
}

bool NoiseFilter::checkNoise() {
    if(_timer.getMilliseconds() >= getNoiseTime()) {
        return true;
    }
    else {
        return false;
    }
}

float NoiseFilter::getNoiseTime() {
    _filterMutex.lockForRead();
    float filterTime = _filterTime;
    _filterMutex.unlock();

    return filterTime;
}

void NoiseFilter::setNoiseTime(float noiseTime) {
    _filterMutex.lockForWrite();
    _filterTime = noiseTime;
    _filterMutex.unlock();
}
